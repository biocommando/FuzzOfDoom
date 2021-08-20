#pragma once
#include "aeffguieditor.h"
#include "TestVst.h"
#include "CBitmapLoader.h"
#include "UiCommon.h"
#include <vector>

extern CColor frontColor, menuBgColor;

class Gui : public AEffGUIEditor, public CControlListener
{
    std::vector<Knob *> knobs;
    COptionMenu *presetList = nullptr;
    COptionMenu *presetActionList = nullptr;
    CBitmap *knobBackground = nullptr;
    CTextEdit *currentPresetNameEdit = nullptr;
    Integer0To100Formatter defaultFormatter;
    SelectionFormatter selectionFormatter;

    std::string currentPresetName = "<Preset name>";

    int presetNumber = -1;
    bool useRoundedValues = true;

    TestVST *synth()
    {
        return (TestVST*)effect;
    }

    Knob *addKnob(CFrame *xframe, int x, int y, int idx, int tag)
    {
        const CColor cBg = kBlackCColor, cFg = frontColor;
        GRID_RECT(knobRect, x, y, KNOB_SIZE, KNOB_SIZE);

        Knob *knob;
        if (getNumberOfOptions(idx) > 0)
            knob = new Knob(knobRect, this, tag, knobBackground, nullptr, idx, selectionFormatter);
        else
            knob = new Knob(knobRect, this, tag, knobBackground, nullptr, idx, defaultFormatter);
        ADD_TEXT("", x, y + 0.7, KNOB_SIZE, TEXT_H, knob->label = label);
        xframe->addView(knob);
        knob->setValue(synth()->getParameter(idx));
        knobs.push_back(knob);
        return knob;
    }

    void setColors(CParamDisplay *ctrl, bool setFrameColor = true)
    {
        const CColor cBg = kBlackCColor, cFg = frontColor;
        ctrl->setBackColor(cBg);
        if (setFrameColor)
            ctrl->setFrameColor(cFg);
        ctrl->setFontColor(cFg);
    }

public:
	Gui(void *ptr) : AEffGUIEditor(ptr)
    {
    }
    ~Gui()
    {
    }

    bool open(void *ptr);
    void valueChanged(CControl *control);

    void close()
    {
        auto xframe = frame;
        frame = 0;
        delete xframe;
    }

    void setParameter(int id, float value)
    {
        if (!frame)
            return;
        for (int i = knobs.size() - 1; i >= 0; i--)
        {
            if (knobs[i]->paramId == id)
            {
                knobs[i]->setValue(value);
                return;
            }
        }
    }
};