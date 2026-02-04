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

uint64_t g_startTime = 0;

inline ULONGLONG GetSecondsElapsed() { return (GetTickCount64() - g_startTime) / 1000; }

LONG WINAPI UiFixVectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
    auto isNvCameraPresent = static_cast<bool>(GetModuleHandleW(L"NvCamera64.dll"));
    auto isEarlyCrash = GetSecondsElapsed() < 10;

    if (!isNvCameraPresent || !isEarlyCrash)
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    PEXCEPTION_RECORD pRecord = pExceptionInfo->ExceptionRecord;

    if (pRecord && pRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
    {
        int32_t res = MessageBoxW(
            NULL,
            L"Skyrim Together has crashed. Please disable \"Game filters and Photo mode\" in the"
            " NVIDIA App/GeForce Experience and try again. Photo mode is also known as NVIDIA Ansel or NvCamera64.dll",
            L"Skyrim Together Tweaks",
            MB_OK);

        if (res == IDOK)
            TerminateProcess(GetCurrentProcess(), 1);
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

void UIActivationFix::Install()
{
    g_startTime = GetTickCount64();
    // TODO: TOML setting `bEnableF2Fix`
    // TODO: `FindPatternInST` sometimes hangs, investigate

    auto& trampoline = SKSE::GetTrampoline();

    uintptr_t addrApplyNvidiaFix = memscan::FindPatternInST(memscan::aob::TiltedOnlineApp_ApplyNvidiaFix);
    if (addrApplyNvidiaFix)
    {
        trampoline.write_branch<5>(addrApplyNvidiaFix, ApplyNvidiaFix::hook);
    }

    uintptr_t addrIsDllBlocked = memscan::FindPatternInST(memscan::aob::DllBlocklist_IsDllBlocked);
    if (addrIsDllBlocked)
    {
        trampoline.write_branch<5>(addrIsDllBlocked, IsDllBlocked::hook);
    }

    // Install the exception handler only if we hooked successfully
    if (addrApplyNvidiaFix && addrIsDllBlocked)
    {
        AddVectoredExceptionHandler(1, UiFixVectoredExceptionHandler);
    }

    spdlog::info("Installed UI activation fix");
}
