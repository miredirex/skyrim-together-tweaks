#include "Hooks.h"
#include "Settings.h"
#include "tweaks/Tweaks.h"
#include <iostream>

bool g_preloaded = false;

void OnInit(SKSE::MessagingInterface::Message* a_msg)
{
    switch (a_msg->type)
    {
    case SKSE::MessagingInterface::kPostLoad:
        {
            hooks::Install();
            MH_EnableHook(MH_ALL_HOOKS);
        }
        break;
    default:
        break;
    }
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []()
{
    SKSE::PluginVersionData v;
    v.PluginVersion({ Plugin::MAJOR, Plugin::MINOR, Plugin::PATCH });
    v.PluginName(Plugin::NAME);
    v.AuthorName("miredirex");
    v.UsesAddressLibrary();
    v.UsesUpdatedStructs();
    v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST });

    return v;
}();

void InitializeLog()
{
    auto path = logger::log_directory();
    if (!path)
    {
        stl::report_and_fail("Failed to find standard logging directory"sv);
    }

    *path /= fmt::format(FMT_STRING("{}.log"), Plugin::NAME);
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
    auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

    log->set_level(spdlog::level::info);
    spdlog::set_pattern("%^[%H:%M:%S] SkyrimTogetherTweaks:%$ %v");

    logger::info(FMT_STRING("{} v{}"), Plugin::NAME, Plugin::VERSION_STRING);
}

extern "C" __declspec(dllexport) void __stdcall Initialize()
{
    settings::Load();
    if (settings::bEnableConsole.GetValue())
    {
        EnableConsole::Install();
    }

    InitializeLog();
    logger::info("SkyrimTogetherTweaks v{}.{}.{} PreLoad"sv, Plugin::MAJOR, Plugin::MINOR, Plugin::PATCH);

    MH_Initialize();
    hooks::InstallPreloadHooks();

    g_preloaded = true;
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
    if (!g_preloaded)
    {
        std::wostringstream messageBoxText;
        messageBoxText << L"ERROR: Skyrim Together Tweaks did not pre-load. Please verify the installation of d3dx9_42.dll from the Part 2 of SSE Engine Fixes. This file must reside in the main game folder alongside SkyrimSE.exe, or be properly installed with your mod manager's root folder functionality.\r\n\r\n"sv;
        messageBoxText << L"Skyrim will now close.";
        REX::W32::MessageBoxW(nullptr, messageBoxText.str().c_str(), L"Skyrim Together Tweaks", MB_OK);

        spdlog::default_logger()->flush();
        ::TerminateProcess(::GetCurrentProcess(), EXIT_SUCCESS);

        return false;
    }

    SKSE::Init(a_skse, false);

    const auto messaging = SKSE::GetMessagingInterface();
    messaging->RegisterListener("SKSE", OnInit);

    return true;
}
