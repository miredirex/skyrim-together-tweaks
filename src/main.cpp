#include "Hooks.h"

void OnInit(SKSE::MessagingInterface::Message* a_msg)
{
    switch (a_msg->type) {
    case SKSE::MessagingInterface::kPostLoad:
        {
            Hooks::Install();
        }
        break;
    default:
        break;
    }
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v;
    v.PluginVersion(Version::MAJOR);
    v.PluginName("SkyrimTogetherTweaks");
    v.AuthorName("miredirex");
    v.UsesAddressLibrary();
    v.UsesUpdatedStructs();
    v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST });

    return v;
}();

void InitializeLog()
{
    auto path = logger::log_directory();
    if (!path) {
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

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
    InitializeLog();

    logger::info("Game version : {}", a_skse->RuntimeVersion().string());

    SKSE::Init(a_skse, false);

    SKSE::AllocTrampoline(1 << 7);

    const auto messaging = SKSE::GetMessagingInterface();
    messaging->RegisterListener("SKSE", OnInit);

    return true;
}
