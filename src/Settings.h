#pragma once

#include <REX/REX/INI.h>

namespace settings
{
    // [General]
    static REX::INI::Bool bEnableF2Fix("General", "bEnableF2Fix", true);
    static REX::INI::Bool bEnableConsole("General", "bEnableConsole", false);
    static REX::INI::Bool bAppendVersionInCrashLogs("General", "bAppendVersionInCrashLogs", true);

    // [Keys]
    static REX::INI::U32  uUiKeyCode("Keys", "uUiKeyCode", 0x71);
    static REX::INI::U32  uDebugMenuKeyCode("Keys", "uDebugMenuKeyCode", 0x72);
    static REX::INI::Bool bDisableRCtrl("Keys", "bDisableRCtrl", false);

    // [Experimental]
    static REX::INI::U32  uInterpolationBufferMs("Experimental", "uInterpolationBufferMs", 300);
    static REX::INI::Bool bNoAnimationDelay("Experimental", "bNoAnimationDelay", false);

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
