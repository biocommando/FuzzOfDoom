#pragma once
class DCCutFilter
{
private:
	double o2[16], i2[16], cutCalc;
	int channels;
	float processSample(float input, double *i2, double *o2);
public:
	float process(float input, unsigned int channel = 0);
	void processAllChannels(float *input, float *output);
	DCCutFilter(unsigned int channels = 1);
	~DCCutFilter();
};

