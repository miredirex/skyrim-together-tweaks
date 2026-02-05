#pragma once

namespace settings
{
    // General
    static REX::TOML::Bool bEnableF2Fix("General", "bEnableF2Fix", true);
    static REX::TOML::Bool bFixCrashLogger("General", "bFixCrashLogger", true);
    static REX::TOML::U32  uUiKeyCode("General", "uUiKeyCode", 0x71);
    static REX::TOML::Bool bEnableConsole("General", "bEnableConsole", false);
    static REX::TOML::Bool bDisableRCtrl("General", "bDisableRCtrl", false);

    // Experiments
    static REX::TOML::U32  uInterpolationBufferMs("Experiments", "uInterpolationBufferMs", 300);
    static REX::TOML::Bool bNoAnimationDelay("Experiments", "bNoAnimationDelay", false);
    static REX::TOML::F32  bMobsAggroMultiplier("Experiments", "bMobsAggroMultiplier", false);

    inline void Load()
    {
        const auto toml = REX::TOML::SettingStore::GetSingleton();
        toml->Init(
            "Data/SKSE/Plugins/SkyrimTogetherTweaks.toml",
            "Data/SKSE/Plugins/SkyrimTogetherTweaksCustom.toml");
        toml->Load();
    }
}
