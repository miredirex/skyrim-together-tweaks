#include "Hooks.h"
#include "tweaks/UIActivationFix.h"
#include "tweaks/FixCrashLogger.h"

namespace hooks
{
    void InstallPreloadHooks()
    {
        UIActivationFix::Install();
    }

    void Install()
    {
        FixCrashLogger::Install();
    }
}
