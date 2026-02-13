#include "Hooks.h"
#include "tweaks/ChangeDebugMenuKey.h"
#include "tweaks/ChangeUIActivationKeys.h"
#include "tweaks/FixCrashLogger.h"
#include "tweaks/InterpolationBufferMs.h"
#include "tweaks/NoAnimationDelay.h"
#include "tweaks/UIActivationFix.h"

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
