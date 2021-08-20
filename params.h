
#pragma once

constexpr int total_number_of_parameters = 7;

constexpr int idx_Gain = 0;
constexpr int idx_Gate = 1;
constexpr int idx_Gate_mode = 2;
constexpr int idx_Blend = 3;
constexpr int idx_Tone = 4;
constexpr int idx_Invert = 5;
constexpr int idx_Oversampling = 6;

constexpr int reserved_id_version = 0;
constexpr int reserved_id_build_date = 1;
constexpr int reserved_id_program_name = 2;


// Group lengths

constexpr const char *getNameForParam(int idx, bool fullName)
{
    if (idx == 0) return fullName ? "Gain" : "gain";
    if (idx == 1) return fullName ? "Gate" : "gate";
    if (idx == 2) return fullName ? "Gate mode" : "gtmode";
    if (idx == 3) return fullName ? "Blend" : "blend";
    if (idx == 4) return fullName ? "Tone" : "tone";
    if (idx == 5) return fullName ? "Invert" : "invert";
    if (idx == 6) return fullName ? "Oversampling" : "ovrsmpl";
    return fullName ? "unknown full" : "unknown";
}

constexpr int getSaveIdForParam(int idx)
{
    switch(idx)
    {
        case 0: return -846078570;
        case 1: return -155909801;
        case 2: return 539099352;
        case 3: return -91474566;
        case 4: return -611292873;
        case 5: return 174671080;
        case 6: return -427149963;
        default: return 0;
    }
}

constexpr int getNumberOfOptions(int index)
{
    if (index == 2) return 6;
    if (index == 5) return 2;
    if (index == 6) return 7;
    return 0;
}

constexpr const char *getOptionLabel(int index, int selection)
{
    if (index == 2)
    {
        if (selection == 0) return "greater";
        if (selection == 1) return "less";
        if (selection == 2) return "pow2gt";
        if (selection == 3) return "pow2lt";
        if (selection == 4) return "sin";
        if (selection == 5) return "bitwise";
    }
    if (index == 5)
    {
        if (selection == 0) return "off";
        if (selection == 1) return "on";
    }
    if (index == 6)
    {
        if (selection == 0) return "1";
        if (selection == 1) return "2";
        if (selection == 2) return "4";
        if (selection == 3) return "16";
        if (selection == 4) return "32";
        if (selection == 5) return "64";
        if (selection == 6) return "128";
    }
    return "ERROR";
}
