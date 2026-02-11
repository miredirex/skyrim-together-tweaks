#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define DIRECTINPUT_VERSION 0x0800

#include <RE/Skyrim.h>
#include <REX/REX/Singleton.h>
#include <SKSE/SKSE.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <xbyak/xbyak.h>

#include <MinHook.h>

#define DLLEXPORT __declspec(dllexport)

using namespace std::literals;
using namespace RE::literals;

namespace logger = SKSE::log;

namespace stl
{
    using namespace SKSE::stl;
}

template <typename T>
MH_STATUS MH_CreateHookTyped(uintptr_t target, T* detour, T** original = nullptr)
{
    return MH_CreateHook(
        reinterpret_cast<void*>(target),
        reinterpret_cast<void*>(detour),
        reinterpret_cast<void**>(original)
    );
}

#include "Plugin.h"
