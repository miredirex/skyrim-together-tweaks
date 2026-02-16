#include "Tweaks.h"
#include <algorithm>

static std::string g_skyrimTogetherVersion;

struct CrashLoggerLogHeaderInfo
{
    static void hook(spdlog::logger* a_log, std::string_view title, std::string_view timePrefix)
    {
        real(a_log, title, timePrefix);
        a_log->info("\nSkyrim Together Version:\n\t{}", g_skyrimTogetherVersion);
    }

    static inline decltype(&hook) real;
};

void AppendVersionInCrashLogs::Install()
{
#if 0
    if (!settings::bAppendVersionInCrashLogs.GetValue())
        return;

    const HMODULE crashLoggerModule = GetModuleHandleW(L"CrashLogger.dll");
    if (!crashLoggerModule)
    {
        spdlog::warn("{}: CrashLoggerSSE is not installed, skipping bAppendVersionInCrashLogs patch", Plugin::NAME);
        return;
    }

    auto      addrCrashLoggerDll = reinterpret_cast<uintptr_t>(crashLoggerModule);
    uintptr_t addrLogHeaderInfo = memscan::FindPattern(memscan::aob::CrashLogger_log_common_header_info, addrCrashLoggerDll);

    if (!addrLogHeaderInfo)
    {
        spdlog::warn("{}: Couldn't find pattern for `log_common_header_info`, skipping bAppendVersionInCrashLogs patch", Plugin::NAME);
        return;
    }

    auto skyrimTogetherVersion = reinterpret_cast<const char*>(memscan::FindString("Build: v1.", 0x180000000));

    g_skyrimTogetherVersion = std::string{ skyrimTogetherVersion };
    std::replace(g_skyrimTogetherVersion.begin(), g_skyrimTogetherVersion.end(), '\n', ' ');

    MH_STATUS status = MH_CreateHookTyped(
        addrLogHeaderInfo,
        CrashLoggerLogHeaderInfo::hook,
        &CrashLoggerLogHeaderInfo::real
    );

    if (status == MH_OK)
    {
        spdlog::info("{}: installed ST version in crash logs tweak", Plugin::NAME);
    }
    else
    {
        spdlog::error("Failed to create hook to CrashLoggerSSE. MH_Status: {}", static_cast<uint8_t>(status));
    }
#endif
}
