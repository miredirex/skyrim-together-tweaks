#include "Tweaks.h"
#include <algorithm>

struct Frame
{
    const void* addr;
};

struct Callstack
{
    uint8_t                pad0[0x18];
    std::span<Frame const> frames;
};

struct CrashLoggerPrintProbableCallstack
{
    static void hook(Callstack* a_this, spdlog::logger* a_log, std::span<void*> a_modules)
    {
        real(a_this, a_log, a_modules);
        a_log->info("PROBABLE CALL STACK (intercepted by Skyrim Together Tweaks):");
        const auto frameCount = std::min(a_this->frames.size(), std::size_t(32));
        for (std::size_t i = 0; i < frameCount; ++i)
        {
            a_log->info("\t0x{:X}", (uintptr_t)a_this->frames[i].addr);
        }
    }

    static inline decltype(&hook) real;
};

// Trying to solve https://discord.com/channels/247835175860305931/717070423254368286/1375140709132275742
// TODO: ...but it's not working, the issue is probably elsewhere
void FixCrashLogger::Install()
{
#if 0
    if (!settings::bFixCrashLogger.GetValue())
    {
        return;
    }

    const HMODULE crashLoggerModule = GetModuleHandleW(L"CrashLogger.dll");
    if (!crashLoggerModule)
    {
        spdlog::warn("{}: CrashLoggerSSE is not installed, skipping bFixCrashLogger patch", Plugin::NAME);
        return;
    }

    auto      addrCrashLoggerDll = reinterpret_cast<uintptr_t>(crashLoggerModule);
    uintptr_t addrPrintCallStack = memscan::FindPattern(memscan::aob::CrashLogger_print_probable_callstack, addrCrashLoggerDll);

    if (!addrPrintCallStack)
    {
        stl::report_and_fail("addrPrintCallStack not found"sv);
    }

    MH_STATUS status = MH_CreateHookTyped(
        addrPrintCallStack,
        CrashLoggerPrintProbableCallstack::hook,
        &CrashLoggerPrintProbableCallstack::real
    );

    if (status != MH_OK)
    {
        spdlog::error("Failed to create `print_probable_callstack` hook. MH_Status: {}", (uint8_t)status);
        return;
    }
#endif
}
