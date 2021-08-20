#include "Gui.h"

CColor frontColor = {77, 77, 77, 255};
CColor menuBgColor = {11, 11, 11, 255};

constexpr int tagParam = 202;

bool Gui::open(void *ptr)
{
    CRect frameSize(0, 0, GRID_X(5) + LEFT_MARGIN, GRID_Y(6.5));
    CColor cBg = kBlackCColor, cFg = kWhiteCColor;
    ERect *wSize;
    getRect(&wSize);

    wSize->top = wSize->left = 0;
    wSize->bottom = (VstInt16)frameSize.bottom;
    wSize->right = (VstInt16)frameSize.right;

    auto xframe = new CFrame(frameSize, ptr, this);

	// Image source https://www.maxpixel.net/Skulls-Transparent-Cutout-Fantasy-Background-6488338
    knobBackground = CBitmapLoader::load("FuzzOfDoomKnob.bmp");

    xframe->setBackgroundColor(cBg);

    // Image source https://www.maxpixel.net/Bone-Skull-Weird-Skull-And-Crossbones-Death-715766
    auto backgroundImage = CBitmapLoader::load("FuzzOfDoom.bmp");

    CRect logoRect(0, 0, backgroundImage->getWidth(), backgroundImage->getHeight());
    auto logo = new CView(logoRect);
    logo->setBackground(backgroundImage);
    xframe->addView(logo);

    knobs.clear();

	auto labelOffset = (GRID_SIZE - knobBackground->getWidth()) / 2.0 / GRID_SIZE;
	ADD_TEXT(getNameForParam(idx_Gain, true), 0 - labelOffset, 0.7, GRID_SIZE, TEXT_H, );
	addKnob(xframe, 0, 1, idx_Gain, tagParam);

	ADD_TEXT(getNameForParam(idx_Gate, true), 2 - labelOffset, 0.7, GRID_SIZE, TEXT_H, );
	addKnob(xframe, 2, 1, idx_Gate, tagParam);

	ADD_TEXT(getNameForParam(idx_Gate_mode, true), 4 - labelOffset, 0.7, GRID_SIZE, TEXT_H, );
	addKnob(xframe, 4, 1, idx_Gate_mode, tagParam);

	ADD_TEXT(getNameForParam(idx_Blend, true), 0 - labelOffset, 2.7, GRID_SIZE, TEXT_H, );
	addKnob(xframe, 0, 3, idx_Blend, tagParam);

	ADD_TEXT(getNameForParam(idx_Tone, true), 4 - labelOffset, 2.7, GRID_SIZE, TEXT_H, );
	addKnob(xframe, 4, 3, idx_Tone, tagParam);

	ADD_TEXT(getNameForParam(idx_Invert, true), 0 - labelOffset, 4.7, GRID_SIZE, TEXT_H, );
	addKnob(xframe, 0, 5, idx_Invert, tagParam);

	ADD_TEXT("Oversamp.", 4 - labelOffset, 4.7, GRID_SIZE, TEXT_H, );
	addKnob(xframe, 4, 5, idx_Oversampling, tagParam);

    //ADD_TEXT("v " VERSION_STRING " build " BUILD_DATE, 12, 13.25, 4 * GRID_SIZE, TEXT_H, label->setHoriAlign(kRightText));
    //ADD_TEXT("(c) 2021 Joonas Salonpaa", 12, 13.5, 4 * GRID_SIZE, TEXT_H, label->setHoriAlign(kRightText));
    //ADD_TEXT("github.com/biocommando/FmSynth", 12, 13.75, 4 * GRID_SIZE, TEXT_H, label->setHoriAlign(kRightText));

    /*auto logoBmp = loadBitmap("TranSynLogo.bmp");

        CRect logoRect(wSize->right / 2 - 234, 5, wSize->right + 234, 50);
        auto logo = new CView(logoRect);
        logo->setBackground(logoBmp);
        xframe->addView(logo);*/

    knobBackground->forget();
	backgroundImage->forget();

    frame = xframe;

    return true;
}

void Gui::valueChanged(CControl *control)
{
    if (!frame)
        return;
    auto tag = control->getTag();

    if (tag == tagParam)
    {
        auto knob = (Knob *)control;
        const auto idx = knob->paramId;

		float value = knob->getValue();
        auto paramid = knob->paramId;

        synth()->setParameterAutomated(idx, value);
        knob->setLabel(value);
    }
}
