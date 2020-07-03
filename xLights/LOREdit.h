#pragma once

/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/smeighan/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/smeighan/xLights/blob/master/License.txt
 **************************************************************/

#include <vector>
#include "Color.h"
#include <wx/xml/xml.h>

class Model;

enum class loreditType
{
    CHANNELS,
    TRACKS,
    NONE
};

struct LOREditEffect
{
    loreditType type;
    uint32_t startMS;
    uint32_t endMS;
    uint16_t startIntensity;
    uint16_t endIntensity;
    xlColor startColour;
    xlColor endColour;
    bool left;
    std::string effectType;
    std::vector<std::string> effectSettings;
    std::vector<std::string> otherSettings;
    bool pixelChannels;

    std::string GetPalette() const;
    std::string GetxLightsEffect() const;
    std::string GetSettings(std::string& palette) const;

    // Used to rescale a parameter to a broader scale.
    // Assumes the range is different but the translation is direct.
    // If this is not the case then set the source Min/Max to the range that does map to the targetMin/Max and the conversion will
    // clamp original values outside the supported range to the largest practical in the target
    static float Rescale(float original, float sourceMin, float sourceMax, float targetMin, float targetMax);
    static wxString RescaleWithRangeI(wxString r, wxString vcName, float sourceMin, float sourceMax, float targetMin, float targetMax, wxString& vc, float targetRealMin, float targetRealMax);
    static wxString RescaleWithRangeF(wxString r, wxString vcName, float sourceMin, float sourceMax, float targetMin, float targetMax, wxString& vc, float targetRealMin, float targetRealMax);
    std::string GetBlend() const;
};

class LOREdit {
    wxXmlDocument& _input_xml;
    int _frequency = 20;

    std::vector<LOREditEffect> GetChannelEffectsForNode(int targetRow, int targetCol, wxXmlNode* prop, int offset) const;

    public:
    LOREdit(wxXmlDocument &input_xml, int frequency);
    virtual ~LOREdit() {};

    std::vector<std::string> GetTimingTracks() const;
    std::vector<std::pair<uint32_t, uint32_t>> GetTimings(const std::string& timingTrackName, int offset = 0) const;
    int GetModelLayers(const std::string& model) const;
    int GetModelStrands(const std::string& model) const;
    int GetModelChannels(const std::string& model, int& rows, int& cols) const;
    loreditType GetSequencingType(const std::string& model) const;
    std::vector<std::string> GetModelsWithEffects() const;
    std::vector<LOREditEffect> GetTrackEffects(const std::string& model, int layer, int offset = 0) const;
    std::vector<LOREditEffect> GetChannelEffects(const std::string& model, int channel, Model* m, int offset) const;
    std::vector<LOREditEffect> GetChannelEffects(const std::string& model, int channel, int nodes, int offset) const;
    std::vector<LOREditEffect> AddEffects(wxXmlNode* track, bool left, int offset) const;
    static void GetLayers(const std::string& settings, int& ll1, int& ll2);
};
