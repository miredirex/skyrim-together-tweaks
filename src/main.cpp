#include "Hooks.h"
#include <iostream>

bool g_preloaded = false;

void OnInit(SKSE::MessagingInterface::Message* a_msg)
{
    switch (a_msg->type)
    {
    case SKSE::MessagingInterface::kPostLoad:
        {
            spdlog::info("kPostLoad message");
        }
        break;
    default:
        break;
    }
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []()
{
    SKSE::PluginVersionData v;
    v.PluginVersion({ Version::MAJOR, Version::MINOR, Version::PATCH });
    v.PluginName(Version::PROJECT);
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

    *path /= fmt::format(FMT_STRING("{}.log"), Version::PROJECT);
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    auto log = std::make_shared<spdlog::logger>("global log"s, spdlog::sinks_init_list{ file_sink, console_sink });

    log->set_level(spdlog::level::info);
    log->flush_on(spdlog::level::info);

    spdlog::set_default_logger(std::move(log));
    spdlog::set_pattern("[%H:%M:%S] %v");

    logger::info(FMT_STRING("{} v{}"), Version::PROJECT, Version::NAME);
}

static void AllocSkyrimTogetherConsole()
{
    if (AllocConsole())
    {
        FILE* file = nullptr;
        freopen_s(&file, "CONOUT$", "w", stdout);
        assert(file != nullptr);
        SetConsoleTitleA("Skyrim Together Console");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    }
}

extern "C" __declspec(dllexport) void __stdcall Initialize()
{
    if (false /*true*/) // TODO: TOML setting `bEnableConsole`, and TODO: check if ST in release mode
    {
        AllocSkyrimTogetherConsole();
    }

    InitializeLog();
    logger::info("SkyrimTogetherTweaks v{}.{}.{} PreLoad"sv, Version::MAJOR, Version::MINOR, Version::PATCH);

    auto& trampoline = SKSE::GetTrampoline();
    trampoline.create(1 << 7, reinterpret_cast<void*>(0x18FFFFFFF)); // "displacement is out of range" if I don't provide `0x18FFFFFFF`
    Hooks::InstallPreloadHooks();

    g_preloaded = true;
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
#if 0
    if (!g_preloaded)
    {
        std::wostringstream messageBoxText;
        messageBoxText << L"ERROR: Skyrim Together Tweaks did not pre-load. Please verify the installation of d3dx9_42.dll from the Engine Fixes' Part 2. This file must reside in the main game folder alongside SkyrimSE.exe, or be properly installed with your mod manager's root folder functionality.\r\n"sv;
        messageBoxText << L"Skyrim will now close.";
        REX::W32::MessageBoxW(nullptr, messageBoxText.str().c_str(), L"Skyrim Together Tweaks", MB_OK);

        spdlog::default_logger()->flush();
        ::TerminateProcess(::GetCurrentProcess(), EXIT_SUCCESS);

        return false;
    }
#endif

    // InitializeLog();

    // SKSE::Init(a_skse, false);
    // SKSE::AllocTrampoline(1 << 7);
    (void*)a_skse;

    // const auto messaging = SKSE::GetMessagingInterface();
    // messaging->RegisterListener("SKSE", OnInit);

    return true;
}
