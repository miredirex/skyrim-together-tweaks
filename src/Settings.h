#pragma once

#include <REX/REX/INI.h>

namespace settings
{
    // General
    static REX::INI::Bool bEnableF2Fix("General", "bEnableF2Fix", true);
    // static REX::INI::Bool bFixCrashLogger("General", "bFixCrashLogger", true);
    static REX::INI::U32  uUiKeyCode("General", "uUiKeyCode", 0x71);
    static REX::INI::Bool bEnableConsole("General", "bEnableConsole", false);
    static REX::INI::Bool bDisableRCtrl("General", "bDisableRCtrl", false);

    // Experiments
    static REX::INI::U32  uInterpolationBufferMs("Experiments", "uInterpolationBufferMs", 300);
    static REX::INI::Bool bNoAnimationDelay("Experiments", "bNoAnimationDelay", false);
    static REX::INI::F32  bMobsAggroMultiplier("Experiments", "bMobsAggroMultiplier", false);

    inline void Load()
    {
        const auto ini = REX::INI::SettingStore::GetSingleton();
        ini->Init(
            "Data/SKSE/Plugins/SkyrimTogetherTweaks.ini",
            "Data/SKSE/Plugins/SkyrimTogetherTweaksCustom.ini"
        );
        ini->Load();
    }
}
