#include "DCCutFilter.h"


DCCutFilter::DCCutFilter(unsigned int channels)
{
	cutCalc = 0.3183099 / (0.3183099 + 0.0005);
	this->channels = channels > 16 ? 16 : channels;
	for (int i = 0; i < this->channels; i++) {
		o2[i] = i2[i] = 0;
	}
}


DCCutFilter::~DCCutFilter()
{
}

float DCCutFilter::processSample(float input, double *i2, double *o2)
{
	*o2 = cutCalc * (input + *o2 - *i2);
	*i2 = input;
	return (float)*o2;
}

float DCCutFilter::process(float input, unsigned int channel)
{
	if (channel >= channels) channel = channels - 1;
	return processSample(input, &i2[channel], &o2[channel]);
}

void DCCutFilter::processAllChannels(float *input, float *output)
{
	for (int i = 0; i < channels; i++)
	{
		output[i] = processSample(input[i], &i2[i], &o2[i]);
	}
}
