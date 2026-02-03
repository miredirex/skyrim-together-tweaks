#include "Hooks.h"
#include "tweaks/UIActivationFix.h"

namespace Hooks
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
