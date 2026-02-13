#include "NoAnimationDelay.h"
#include "MemoryScan.h"
#include "Settings.h"
#include "ScopedMemoryWrite.h"

void NoAnimationDelay::Install()
{
    bool disableAnimationDelay = settings::bNoAnimationDelay.GetValue();

    if (!disableAnimationDelay)
        return;

    auto tickCmpInst = reinterpret_cast<uint8_t*>(memscan::FindPatternInST(memscan::aob::AnimationSystem_Update_tickCmp));

    if (!tickCmpInst)
    {
        auto err = std::format(
            "Pattern AnimationSystem_Update_tickCmp wasn't found: {}. Exiting",
            memscan::aob::AnimationSystem_Update_tickCmp
        );
        stl::report_and_fail(err);
    }

    ScopedMemoryWrite _{ tickCmpInst, 16 };
    std::memset(tickCmpInst, 0x90, 10); // No-op `cmp` and `ja` - 10 bytes total

    spdlog::info("{}: installed no animation delay tweak", Plugin::NAME);
}
