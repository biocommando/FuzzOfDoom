// This is the main DLL file.

#include "stdafx.h"
#include "TestVST.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "GenericDto.h"
#include "Gui.h"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {
	return new TestVST(audioMaster);
}

TestVST::TestVST(audioMasterCallback audioMaster) :
	AudioEffectX(audioMaster, 0, total_number_of_parameters),
	muteNextFrame(false)
{
	setNumInputs(2); // stereo in
	setNumOutputs(2); // stereo out
	setUniqueID(1621747547); // identify
	programsAreChunks();
	for (int i = 0; i < total_number_of_parameters; i++)
	{
		params.push_back(Parameter(i, 0.5));
	}
	for (int ch = 0; ch < 2; ch++)
	{
		dcFilter.push_back(DcFilter(sampleRate));
		toneFilters.push_back(Filter(sampleRate * oversampling));
		toneFilters.push_back(Filter(sampleRate * oversampling));
		downsamplingFilters.push_back(Filter(sampleRate * oversampling));
		downsamplingFilters[ch].updateLowpass(sampleRate * 0.5);
	}
	params[idx_Invert].value = 0;
	params[idx_Oversampling].value = 0;
	params[idx_Gate_mode].value = 0;
	updateParams();
}

TestVST::~TestVST()
{
	if (chunk)
	{
		free(chunk);
	}
}

void TestVST::open()
{
	setEditor(new Gui(this));
}

VstInt32 TestVST::getChunk(void** data, bool isPreset)
{
	if (chunk)
	{
		free(chunk);
		chunk = nullptr;
	}
	std::string s = GenericDto::createInt(0, reserved_id_version).serialize();
	for (int i = 0; i < params.size(); i++)
	{
		auto param = &params[i];
		s += GenericDto::serializeParameter(param->value, i);
	}
	chunk = (char*)malloc(s.size() + 1);
	if (chunk)
	{
		memcpy(chunk, s.c_str(), s.size() + 1);
		*data = chunk;
		return s.size() + 1;
	}
	return 0;
}

VstInt32 TestVST::setChunk(void* data, VstInt32 byteSize, bool isPreset)
{
	auto dtos = GenericDto::deserializeAll(std::string((char*)data, byteSize));
	for (int i = 0; i < dtos.size(); i++)
	{
		const auto dto = &dtos[i];
		if (dto->id == reserved_id_version || dto->id == reserved_id_build_date)
		{
			// reserved ids, pass
		}
		else
		{
			for (int i = 0; i < params.size(); i++)
			{
				if (params[i].getId() == dto->id)
				{
					if (dto->getType() == 'i')
						params[i].value =
							Util::getSelectionValue(dto->iValue, getNumberOfOptions(i));
					else
						params[i].value = dto->fValue;
					break;
				}
			}
		}
	}
	updateParams();
	return 0;
}

float TestVST::getParameter(VstInt32 index)
{
	if (index >= total_number_of_parameters)
		return 0;
	return params[index].value;
}
void TestVST::setParameter(VstInt32 index, float value)
{
	if (index >= total_number_of_parameters)
		return;
	if (index == idx_Oversampling)
	{
		auto selection = Util::getSelection(value, getNumberOfOptions(idx_Oversampling));
		auto oldSelection = Util::getSelection(params[index].value, getNumberOfOptions(idx_Oversampling));

		if (selection != oldSelection)
		{
			oversampling = std::stoi(getOptionLabel(idx_Oversampling, selection));
			for (int ch = 0; ch < 2; ch++)
			{
				for (int i = 0; i < 2; i++)
				{
					toneFilters[ch * 2 + i].sampleRate = oversampling * sampleRate;
				}
				downsamplingFilters[ch].sampleRate = sampleRate * oversampling;
				downsamplingFilters[ch].updateLowpass(sampleRate * 0.5);
			}
		}
	}
	params[index].value = value;
	updateParams();
}
void TestVST::getParameterName(VstInt32 index, char* label)
{
	if (index >= total_number_of_parameters)
		return;
	strcpy_s(label, 8, getNameForParam(index, false));
}
void TestVST::getParameterDisplay(VstInt32 index, char* text)
{
	if (index >= total_number_of_parameters)
		return;
	sprintf_s(text, 8, "%d", (int)(100 * params[index].value));
}
void TestVST::getParameterLabel(VstInt32 index, char* label)
{
	strcpy_s(label, 8, "");
}
bool TestVST::getEffectName(char* name)
{
	strcpy_s(name, 32, "Digi Fuzz of Doom");
	return true;
}
bool TestVST::getProductString(char* text)
{
	strcpy_s(text, 64, "Digi Fuzz of Doom");
	return true;
}
bool TestVST::getVendorString(char* text)
{
	strcpy_s(text, 64, "(c) 2017-2021 Joonas Salonpaa");
	return true;
}

void TestVST::updateParams()
{
	float pow2Cut = params[idx_Tone].value;
	pow2Cut *= pow2Cut;
	auto cutFreqHz = sampleRate * 0.5 * pow2Cut;
	for (int i = 0; i < 4; i++)
		toneFilters[i].updateLowpass(cutFreqHz);

	gate = 0.1f * params[idx_Gate].value;
	invert = Util::getSelection(params[idx_Invert].value, getNumberOfOptions(idx_Invert)) == 1;
	gateMode = Util::getSelection(params[idx_Gate_mode].value, getNumberOfOptions(idx_Gate_mode));
	gain = params[idx_Gain].value * 10;
}

float TestVST::fuzz(const float input)
{
	float mix = params[idx_Blend].value;
	auto i = input * gain;
	float gated;
	switch (gateMode)
	{
	case 0:
		gated = i > gate ? 1 : -1;
		break;
	case 1:
		gated = i < gate ? 1 : -1;
		break;
	case 2:
		gated = i * i > gate ? 1 : -1;
		break;
	case 3:
		gated = i * i < gate ? 1 : -1;
		break;
	case 4:
		gated = sin(6.28 * i) > gate ? 1 : -1;
		break;
	case 5:
		gated = (((int)(i * 1000)) & 1) && i > gate ? 1 : -1;
		break;
	}
	const float imix = invert ? -mix : mix;
	return i * (1 - mix) + gated * imix;
}

float TestVST::processOneSample(float input, int channel)
{
	float out = fuzz(input);
	out = out > 1 ? 1 : out;
	out = out < -1 ? -1 : out;
	out = toneFilters[channel * 2].processLowpass(out);
	out = toneFilters[channel * 2 + 1].processLowpass(out);
	return out;
}

void TestVST::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
	bool allZero = true;
	// Real processing goes here
	for (int i = 0; i < sampleFrames; i++)
	{
		for (int channel = 0; channel < 2; channel++)
		{
			float input = inputs[channel][i];
			allZero = allZero && input == 0;
			if (!muteNextFrame)
			{
				float v;
				for (int i = 0; i < oversampling; i++)
				{
					v = processOneSample(input, channel);
					input = 0;
					v = downsamplingFilters[channel].processLowpass(v);
				}
				outputs[channel][i] = dcFilter[channel].process(v * oversampling);
			}
			else
				outputs[channel][i] = 0;
		}
	}
	muteNextFrame = allZero;
}