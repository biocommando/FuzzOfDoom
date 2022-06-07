// TestVST.h
#pragma once
#include "audioeffectx.h"
#include "common.h"
#include "Parameter.h"
#include "Filter.h"

class TestVST : public AudioEffectX
{
private:
	std::vector<DcFilter> dcFilter;
	std::vector<Filter> toneFilters;
	std::vector<Filter> downsamplingFilters;

	float gate, gain;
	bool invert;
	int gateMode;

	bool muteNextFrame;
	std::vector<Parameter> params;
	int oversampling = 1;

	char* chunk = nullptr;

	void updateParams();

	float processOneSample(float input, int channel);
	float fuzz(float i);

public:
	TestVST(audioMasterCallback audioMaster);
	~TestVST();
	void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames);
	float getParameter(VstInt32 index);
	void setParameter(VstInt32 index, float value);
	void getParameterName(VstInt32 index, char *label);
	void getParameterDisplay(VstInt32 index, char *text);
	void getParameterLabel(VstInt32 index, char *label);
	bool getEffectName(char* name);
	bool getProductString(char* text);
	bool getVendorString(char* text);
	VstInt32 getChunk(void** data, bool isPreset);
	VstInt32 setChunk(void* data, VstInt32 byteSize, bool isPreset);
	void open();
};
