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
    }

    uintptr_t FindPatternInST(std::string_view a_pattern);
}
