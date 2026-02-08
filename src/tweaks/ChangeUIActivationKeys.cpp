#include "ChangeUIActivationKeys.h"
#include "MemoryScan.h"
#include "ScopedMemoryWrite.h"
#include "Settings.h"
#include "util/VkToDik.h"
#include <format>

using TDInputHook_SetToggleKeys = void(void* apThis, std::initializer_list<uint64_t>);

struct DInputHook;

void ChangeUIActivationKeys::Install()
{
    uint32_t vkKeyCode = settings::uUiKeyCode.GetValue();
    bool     isRCtrlDisabled = settings::bDisableRCtrl.GetValue();

    if (vkKeyCode == VK_F2 && !isRCtrlDisabled) // Skip defaults
        return;

    auto addrDInputHookGet = memscan::FindPatternInST(memscan::aob::DInputHook_Get);
    if (!addrDInputHookGet)
        addrDInputHookGet = memscan::FindPatternInST(memscan::aob::DInputHook_Get_v180);

    auto addrSetToggleKeys = memscan::FindPatternInST(memscan::aob::DInputHook_SetToggleKeys);
    if (!addrSetToggleKeys)
        addrSetToggleKeys = memscan::FindPatternInST(memscan::aob::DInputHook_SetToggleKeys_v180);

    if (!addrDInputHookGet || !addrSetToggleKeys)
    {
        auto err = std::format(
            "Either DInputHook_Get or DInputHook_SetToggleKeys wasn't found: {} or {}, exiting",
            memscan::aob::DInputHook_Get,
            memscan::aob::DInputHook_SetToggleKeys
        );
        stl::report_and_fail(err);
    }

    spdlog::info(
        "{}: installing activation keys tweak, key: {}; disabling right ctrl: {}",
        Plugin::NAME,
        vkKeyCode,
        isRCtrlDisabled
    );

    // Part 1: `SetToggleKeys(...)`

    auto dInputHookInstance = reinterpret_cast<DInputHook* (*)()>(addrDInputHookGet)();
    auto setToggleKeys = reinterpret_cast<TDInputHook_SetToggleKeys*>(addrSetToggleKeys);

    uint64_t dikKeyCode = util::MapVkToDik(vkKeyCode); // `SetToggleKeys` accepts DIK_ key codes
    if (isRCtrlDisabled)
    {
        setToggleKeys(dInputHookInstance, std::initializer_list{ dikKeyCode });
    }
    else
    {
        setToggleKeys(dInputHookInstance, std::initializer_list<uint64_t>{ dikKeyCode, util::MapVkToDik(VK_RCONTROL) });
    }

    // Part 2: `ProcessKeyboard` cmp instruction

    auto processKeyboardKeysIfInst = reinterpret_cast<uint8_t*>(memscan::FindPatternInST(memscan::aob::InputService_ProcessKeyboard_KeysIf));
    if (!processKeyboardKeysIfInst)
    {
        stl::report_and_fail("InputService_ProcessKeyboard_KeysIf pattern wasn't found, exiting");
    }

    ScopedMemoryWrite _{ processKeyboardKeysIfInst, 16 };

    uint8_t* rCtrlCheck = processKeyboardKeysIfInst;
    if (isRCtrlDisabled)
    {
        std::memset(rCtrlCheck, 0x90, 5); // No-op the cmp
    }

    uint8_t* f2KeyCheck = processKeyboardKeysIfInst + 5 + 2;
    *f2KeyCheck = static_cast<uint8_t>(vkKeyCode);

    spdlog::info("{}: installed activation keys tweak", Plugin::NAME);
}
