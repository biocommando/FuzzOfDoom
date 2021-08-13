// TestVST.h
#include "audioeffectx.h"
#include "DCCutFilter.h"
#include <string>

#define NUM_PARAMS 5
#define P_GAIN 0
#define P_GATE 1
#define P_BLEND 2
#define P_TONE 3
#define P_INVERT 4

typedef struct{
	float value;
	std::string name;
} MinimalParameter;


class TestVST : public AudioEffectX
{
private:
	DCCutFilter dcFilter;
	float out2[4], cutCalc, gate, invert, gain;
	bool muteNextFrame;
	MinimalParameter params[NUM_PARAMS];
	void addParam(int index, char *name, float value = 0.5);
	void updateParams();
public:
	TestVST(audioMasterCallback audioMaster);
	~TestVST();
	void processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames);
	float getParameter(VstInt32 index);
	void setParameter(VstInt32 index, float value);
	void getParameterName(VstInt32 index, char *label);
	void getParameterDisplay(VstInt32 index, char *text);
	void getParameterLabel(VstInt32 index, char *label);
	bool getEffectName(char* name);
	bool getProductString(char* text);
	bool getVendorString(char* text);
};
