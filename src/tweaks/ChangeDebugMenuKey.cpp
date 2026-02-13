#include "ChangeDebugMenuKey.h"
#include "MemoryScan.h"
#include "ScopedMemoryWrite.h"
#include "Settings.h"

void ChangeDebugMenuKey::Install()
{
    uint32_t vkDebugKeyCode = settings::uDebugMenuKeyCode.GetValue();

    if (vkDebugKeyCode == VK_F3)
        return;

    auto f3KeyStateInst = reinterpret_cast<uint8_t*>(memscan::FindPatternInST(memscan::aob::DebugService_OnUpdate_F3KeyState));
    if (!f3KeyStateInst)
    {
        auto err = std::format(
            "Pattern DebugService_OnUpdate_F3KeyState wasn't found: {}. Exiting",
            memscan::aob::DebugService_OnUpdate_F3KeyState
        );
        stl::report_and_fail(err);
    }

    uint32_t* debugKeyCodeValue = reinterpret_cast<uint32_t*>(f3KeyStateInst + 1);

    ScopedMemoryWrite _{ debugKeyCodeValue, 8 };
    *debugKeyCodeValue = vkDebugKeyCode;

    spdlog::info("{}: installed debug menu key tweak", Plugin::NAME);
}
