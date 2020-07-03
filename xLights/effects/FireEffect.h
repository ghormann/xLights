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

#include "RenderableEffect.h"

#define FIRE_GROWTHCYCLES_MIN 0
#define FIRE_GROWTHCYCLES_MAX 200
#define FIRE_GROWTHCYCLES_DIVISOR 10

#define FIRE_HEIGHT_MIN 1
#define FIRE_HEIGHT_MAX 100

#define FIRE_HUE_MIN 0
#define FIRE_HUE_MAX 100

class FireEffect : public RenderableEffect
{
    public:
        FireEffect(int id);
        virtual ~FireEffect();
        virtual void SetDefaultParameters() override;
        virtual void Render(Effect *effect, SettingsMap &settings, RenderBuffer &buffer) override;
        virtual std::list<std::string> CheckEffectSettings(const SettingsMap& settings, AudioManager* media, Model* model, Effect* eff, bool renderCache) override;
protected:
    virtual bool needToAdjustSettings(const std::string &version) override;
    virtual void adjustSettings(const std::string &version, Effect *effect, bool removeDefaults = true) override;
    virtual wxPanel *CreatePanel(wxWindow *parent) override;
    private:
};
