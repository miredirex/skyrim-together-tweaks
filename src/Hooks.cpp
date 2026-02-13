#include "Hooks.h"
#include "tweaks/Tweaks.h"

namespace hooks
{
    void InstallPreloadHooks()
    {
        UIActivationFix::Install();
    }

    void Install()
    {
        FixCrashLogger::Install(); // Doesn't do anything as of now
        ChangeUIActivationKeys::Install();
        ChangeDebugMenuKey::Install();
        InterpolationBufferMs::Install();
        NoAnimationDelay::Install();
    }
}
