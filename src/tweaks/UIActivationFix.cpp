#include "Tweaks.h"
#include "util/DInputStubs.h"

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

util::TIDirectInputA_CreateDevice* g_stHookCreateDevice = nullptr;

// Security software (e.g. Bitdefender) can replace IDirectInput8A::CreateDevice in the vtable so ST's hook on it never runs
// and the UI never opens; wrapping the object and calling ST's hook directly avoids that
struct HookDirectInput8Create
{
    static HRESULT WINAPI hook(HINSTANCE a_instance, DWORD a_version, REFIID a_iid, LPVOID* a_out, LPUNKNOWN a_outer)
    {
        const HRESULT result = original(a_instance, a_version, a_iid, a_out, a_outer);
        if (result == DI_OK && *a_out && IsEqualIID(a_iid, util::kIID_IDirectInput8A))
        {
            *a_out = static_cast<IDirectInput8A*>(new util::DirectInput8AWrapper(static_cast<IDirectInput8A*>(*a_out), g_stHookCreateDevice));
            logger::info("wrapped IDirectInput8A");
        }
        return result;
    }

    static inline decltype(&hook) original = nullptr;
};

static void InstallDirectInputWrapper()
{
    uintptr_t addrHookDirectInput8Create = memscan::FindPatternInST(memscan::aob::DInputHook_HookDirectInput8Create);
    uintptr_t addrHookCreateDevice = memscan::FindPatternInST(memscan::aob::DInputHook_HookIDirectInputA_CreateDevice);
    if (!addrHookDirectInput8Create || !addrHookCreateDevice)
    {
        logger::warn("DInputHook patterns weren't found, not installing the DirectInput wrapper");
        return;
    }

    g_stHookCreateDevice = reinterpret_cast<util::TIDirectInputA_CreateDevice*>(addrHookCreateDevice);
    MH_CreateHookTyped(addrHookDirectInput8Create, HookDirectInput8Create::hook, &HookDirectInput8Create::original);
}

uint64_t g_startTime = 0;

inline ULONGLONG GetSecondsElapsed() { return (GetTickCount64() - g_startTime) / 1000; }

LONG WINAPI UiFixVectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
    auto isNvCameraPresent = static_cast<bool>(GetModuleHandleW(L"NvCamera64.dll"));
    auto isEarlyCrash = GetSecondsElapsed() < 5;

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
            MB_OK
        );

        if (res == IDOK)
            TerminateProcess(GetCurrentProcess(), 1);
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

void UIActivationFix::Install()
{
    if (!settings::bEnableF2Fix.GetValue())
    {
        logger::info("bEnableF2Fix is set to false, not installing fix");
        return;
    }

    g_startTime = GetTickCount64();

    uintptr_t addrApplyNvidiaFix = memscan::FindPatternInST(memscan::aob::TiltedOnlineApp_ApplyNvidiaFix);
    if (addrApplyNvidiaFix)
    {
        MH_CreateHookTyped(addrApplyNvidiaFix, ApplyNvidiaFix::hook);
    }

    uintptr_t addrIsDllBlocked = memscan::FindPatternInST(memscan::aob::DllBlocklist_IsDllBlocked);
    if (addrIsDllBlocked)
    {
        MH_CreateHookTyped(addrIsDllBlocked, IsDllBlocked::hook);
    }

    // Install the exception handler only if we hooked successfully
    if (addrApplyNvidiaFix && addrIsDllBlocked)
    {
        AddVectoredExceptionHandler(1, UiFixVectoredExceptionHandler);
    }

    InstallDirectInputWrapper();

    logger::info("installed UI activation fix");
}
