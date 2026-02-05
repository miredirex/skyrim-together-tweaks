#include "EnableConsole.h"
#include "MemoryScan.h"

static void SpawnSkyrimTogetherConsole()
{
    if (AllocConsole())
    {
        FILE* file = nullptr;
        freopen_s(&file, "CONOUT$", "w", stdout);
        assert(file != nullptr);
        SetConsoleTitleA("Skyrim Together Tweaks");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    }
}

struct TiltedOnlineApp;

void EnableConsole::Install()
{
    SpawnSkyrimTogetherConsole();

    uintptr_t addrCtor = memscan::FindPatternInST(memscan::aob::TiltedOnlineApp_ctor);
    uintptr_t addrGlobalInstanceMovInst = memscan::FindPatternInST(memscan::aob::RunTiltedInit_g_appInstanceMov);

    if (addrCtor && addrGlobalInstanceMovInst)
    {
        spdlog::info("{}: installing EnableConsole tweak...", Plugin::NAME);

        // Extract the 4-byte displacement (starts at index 3). Cast to int32_t because the offset can be negative
        int32_t displacement = *(int32_t*)(addrGlobalInstanceMovInst + 3);
        // Instruction address + length of this instruction (7) + displacement
        uintptr_t addrGlobalAppInstance = addrGlobalInstanceMovInst + 7 + displacement;

        // Call TiltedOnlineApp's constructor (again) to initialize ST's spdlog. It just works.
        using TTiltedOnlineApp_ctor = void(TiltedOnlineApp * apThis);
        auto ctor = reinterpret_cast<TTiltedOnlineApp_ctor*>(addrCtor);
        auto g_appInstance = reinterpret_cast<std::unique_ptr<TiltedOnlineApp>*>(addrGlobalAppInstance);

        ctor(/* this = */ g_appInstance->get());

        spdlog::info("{}: installed EnableConsole tweak", Plugin::NAME);
    }
}
