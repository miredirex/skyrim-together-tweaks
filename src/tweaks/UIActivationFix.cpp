#include "UIActivationFix.h"
#include "MemoryScan.h"

struct ApplyNvidiaFix
{
    static void hook(void**) { return; } // No-op that fn
};

struct IsDllBlocked
{
    static bool hook(std::wstring_view a_dll)
    {
        // Custom DLL blacklist
        const wchar_t* const kDllBlocklist[] = {
            L"crashhandler64.dll",
            L"fraps64.dll",
            L"SpecialK64.dll",
            L"ReShade64_SpecialK64.dll",
        };
        for (const wchar_t* dllEntry : kDllBlocklist)
        {
            if (std::wcscmp(a_dll.data(), dllEntry) == 0)
                return true;
        }

        return false;
    }
};

void UIActivationFix::Install()
{
    // TODO: TOML setting `bEnableF2Fix`
    // TODO: `FindPatternInST` sometimes hangs, investigate

    auto& trampoline = SKSE::GetTrampoline();

    uintptr_t addrApplyNvidiaFix = memscan::FindPatternInST(memscan::aob::TiltedOnlineApp_ApplyNvidiaFix);
    if (addrApplyNvidiaFix)
        trampoline.write_branch<5>(addrApplyNvidiaFix, ApplyNvidiaFix::hook);

    uintptr_t addrIsDllBlocked = memscan::FindPatternInST(memscan::aob::DllBlocklist_IsDllBlocked);
    if (addrIsDllBlocked)
        trampoline.write_branch<5>(addrIsDllBlocked, IsDllBlocked::hook);

    spdlog::info("Installed UI activation fix");
}