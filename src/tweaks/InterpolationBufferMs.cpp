#include "ScopedMemoryWrite.h"
#include "Tweaks.h"

const uint32_t DEFAULT_MS_IN_SKYRIM_TOGETHER = 300;

void InterpolationBufferMs::Install()
{
    uint32_t desiredInterpolationBufferMs = settings::uInterpolationBufferMs.GetValue();

    if (desiredInterpolationBufferMs == DEFAULT_MS_IN_SKYRIM_TOGETHER)
        return;

    if (desiredInterpolationBufferMs < 10)
    {
        stl::report_and_fail("Do not set \"uInterpolationBufferMs\" to such low values! Exiting");
    }

    auto tickInst = reinterpret_cast<uint8_t*>(memscan::FindPatternInST(memscan::aob::CharacterService_RunRemoteUpdates_tick));

    if (!tickInst)
    {
        auto err = std::format(
            "Pattern CharacterService_RunRemoteUpdates_tick wasn't found: {}. Exiting",
            memscan::aob::CharacterService_RunRemoteUpdates_tick
        );
        stl::report_and_fail(err);
    }

    uint32_t* interpolationBufferMsValue = reinterpret_cast<uint32_t*>(tickInst + 2);

    ScopedMemoryWrite _{ interpolationBufferMsValue, 8 };
    *interpolationBufferMsValue = desiredInterpolationBufferMs;

    spdlog::info(
        "{}: installed interpolation buffer tweak ({}ms -> {}ms)",
        Plugin::NAME,
        DEFAULT_MS_IN_SKYRIM_TOGETHER,
        desiredInterpolationBufferMs
    );
}
