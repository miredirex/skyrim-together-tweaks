#include "Hooks.h"
#include "tweaks/UIActivationFix.h"

namespace hooks
{
    void InstallPreloadHooks()
    {
        UIActivationFix::Install();
    }

    void Install()
    {
        // ...
    }
}
