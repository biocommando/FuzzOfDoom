// This is the main DLL file.

#include "stdafx.h"
#include "TestVST.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {
	return new TestVST(audioMaster);
}

TestVST::TestVST(audioMasterCallback audioMaster) :
	AudioEffectX(audioMaster, 0, NUM_PARAMS), dcFilter(DCCutFilter(2))
{
	setNumInputs(2); // stereo in
	setNumOutputs(2); // stereo out
	setUniqueID('1fz7');	// identify
	addParam(P_GAIN, "Gain");
	addParam(P_GATE, "Gate");
	addParam(P_BLEND, "Blend");
	addParam(P_TONE, "Tone");
	addParam(P_INVERT, "Invert", 0);
	out2[0] = out2[1] = out2[2] = out2[3] = 0;
	muteNextFrame = false;
	updateParams();
}

TestVST::~TestVST()
{
}

void TestVST::addParam(int index, char* name, float value)
{
	params[index].name = name;
	params[index].value = value;
}

float TestVST::getParameter(VstInt32 index)
{
	if (index >= NUM_PARAMS)
		return 0;
	return (float)params[index].value;
}
void TestVST::setParameter(VstInt32 index, float value)
{
	if (index >= NUM_PARAMS)
		return;
	params[index].value = value;
	updateParams();
}
void TestVST::getParameterName(VstInt32 index, char* label)
{
	if (index >= NUM_PARAMS)
		return;
	strcpy_s(label, 8, params[index].name.c_str());
}
void TestVST::getParameterDisplay(VstInt32 index, char* text)
{
	if (index >= NUM_PARAMS)
		return;
	if (index == P_INVERT)
	{
		if (params[index].value < 0.5)
			strcpy_s(text, 8, "Off");
		else
			strcpy_s(text, 8, "On");
	}
	else
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
	float pow2Cut = params[P_TONE].value * params[P_TONE].value;
	cutCalc = pow2Cut / (0.3183099f + pow2Cut);
	gate = 0.1f * params[P_GATE].value;
	invert = params[P_INVERT].value < 0.5 ? 0 : (float)1;
	gain = params[P_GAIN].value * 10;
}

float fuzz(float i, float mix, float th, float inv)
{
	float b = 0, imix;
	if (i > th) b = 1;
	else if (i <= th) b = -1;
	imix = mix * (1 - inv * 2);
	return i * (1 - mix) + b * imix;
}

float filter(float in, float cutCalc, float out2)
{
	return out2 + cutCalc * (in - out2);
}

void TestVST::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) {
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
				float fuzzOutput = fuzz(input * gain, params[P_BLEND].value, gate, invert);
				fuzzOutput = fuzzOutput > 1 ? 1 : fuzzOutput;
				fuzzOutput = fuzzOutput < -1 ? -1 : fuzzOutput;
				out2[channel * 2] = filter(fuzzOutput, cutCalc, out2[channel * 2]);
				out2[channel * 2 + 1] = filter(out2[channel * 2], cutCalc, out2[channel * 2 + 1]);
				outputs[channel][i] = dcFilter.process(out2[channel * 2 + 1], channel);
			}
			else
				outputs[channel][i] = 0;
		}
	}
	muteNextFrame = allZero;
}