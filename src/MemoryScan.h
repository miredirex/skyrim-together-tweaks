#include "PCH.h"

namespace memscan
{
    namespace aob
    {
        const std::string_view TiltedOnlineApp_BeginMain = "48 89 5C 24 08 48 89 6C 24 18 56 57 41 56 48 83 EC 40 4C 8B F1";
        const std::string_view TiltedOnlineApp_ApplyNvidiaFix = "48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 D9 48 81 EC F0 00 00 00 C7";
        const std::string_view TiltedOnlineApp_ctor = "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 41 54 41 55 41 56 41 57 48 8D AC 24 50 FE";
        const std::string_view DllBlocklist_IsDllBlocked = "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 60 48";
        const std::string_view RunTiltedInit_g_appInstanceMov = "48 8B 1D ?? ?? ?? ?? 48 89 3D ?? ?? ?? ?? 48 85 DB";

        const std::string_view DInputHook_Get = "48 83 EC 38 8B 0D ?? ?? ?? ?? 65 48 8B 04 25 58 00 00 00 BA";
        const std::string_view DInputHook_Get_v180 = "40 53 48 83 EC 40 B9";

        const std::string_view DInputHook_SetToggleKeys = "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 40 4C 8B F2";
        const std::string_view DInputHook_SetToggleKeys_v180 = "48 89 5C 24 10 48 89 6C 24 18 48 89 4C 24 08 56 57 41 56 48 83 EC 50 48 8B FA 48 8B 05";

        // if ( v8 == 0xA3 || v8 == 0x71 )
        const std::string_view InputService_ProcessKeyboard_KeysIf = "41 3B C5 74 14 83 F8 71 74 0F 66 83 FB 1B";

        const std::string_view CrashLogger_print_probable_callstack = "40 53 56 57 41 54 41 55 41 56 41 57 48 81 EC 20 02 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 10 02 00 00 4C";
    }

    uintptr_t FindPatternInST(std::string_view a_pattern);
    uintptr_t FindPattern(std::string_view a_pattern, uintptr_t a_start);
}
