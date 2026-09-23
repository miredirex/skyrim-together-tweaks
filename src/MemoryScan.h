#pragma once

namespace memscan
{
    namespace aob
    {
        const std::string_view TiltedOnlineApp_BeginMain = "48 89 5C 24 08 48 89 6C 24 18 56 57 41 56 48 83 EC 40 4C 8B F1";
        const std::string_view TiltedOnlineApp_ApplyNvidiaFix = "48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 D9 48 81 EC F0 00 00 00 C7";
        const std::string_view TiltedOnlineApp_ctor = "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 41 54 41 55 41 56 41 57 48 8D AC 24 50 FE";
        const std::string_view TiltedOnlineApp_ctor_v182 = "48 89 5C 24 ?? 48 89 74 24 ?? 48 89 7C 24 ?? 55 41 54 41 55 41 56 41 57 48 8D 6C 24 ?? 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 48 8B D9 48 89 4C 24 ?? 48 89 4C 24";
        const std::string_view DllBlocklist_IsDllBlocked = "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 60 48";
        const std::string_view RunTiltedInit_g_appInstanceMov = "48 8B 1D ?? ?? ?? ?? 48 89 3D ?? ?? ?? ?? 48 85 DB";
        const std::string_view RunTiltedInit_g_appInstanceMov_v182 = "48 8B 1D ?? ?? ?? ?? 48 89 2D";

        const std::string_view DInputHook_Get = "48 83 EC 38 B9 68";
        const std::string_view DInputHook_Get_v180 = "40 53 48 83 EC 40 B9";

        const std::string_view DInputHook_SetToggleKeys = "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 40 4C 8B F2";
        const std::string_view DInputHook_SetToggleKeys_v180 = "48 89 5C 24 10 48 89 6C 24 18 48 89 4C 24 08 56 57 41 56 48 83 EC 50 48 8B FA 48 8B 05";

        // if ( v8 == 0xA3 || v8 == 0x71 )
        const std::string_view InputService_ProcessKeyboard_KeysIf = "41 3B C5 74 14 83 F8 71 74 0F 66 83 FB 1B";
        // if (GetAsyncKeyState(VK_F3) & 0x01)
        const std::string_view DebugService_OnUpdate_F3KeyState = "B9 72 00 00 00 FF 15 ?? ?? ?? ?? A8 01 74";

        const std::string_view CharacterService_RunRemoteUpdates_tick = "48 2D 2C 01 00 00 48 89 45 A0 49 8B 5D 00";
        const std::string_view AnimationSystem_Update_tickCmp = "4C 39 4D 10 0F 87 ?? ?? ?? ?? 48 8D";

        const std::string_view CrashLogger_log_common_header_info = "48 89 5C 24 20 55 56 57 41 56 41 57 48 8D 6C";
    }

    uintptr_t FindPatternInST(std::string_view a_pattern);
    uintptr_t FindPattern(std::string_view a_pattern, uintptr_t a_start);
    uintptr_t FindString(std::string_view a_string, uintptr_t a_start);
}
