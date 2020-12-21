#pragma once
#include "engine/core/core.h"

#ifdef SU_PLATFORM_WINDOWS

extern susumu::App* susumu::CreateApp();

int main(int argc, char** argv)
{
    susumu::Log::Init();

    SU_PROFILE_BEGIN_SESSION("Startup", "SusumuProfile-Startup.json");
    auto app = susumu::CreateApp();
    SU_PROFILE_END_SESSION();

    SU_PROFILE_BEGIN_SESSION("Runtine", "SusumuProfile-Runtime.json");
    app->Run();
    SU_PROFILE_END_SESSION();

    SU_PROFILE_BEGIN_SESSION("Shutdown", "SusumuProfile-Shutdown.json");
    delete app;
    SU_PROFILE_END_SESSION();

    return 0;
}

#endif
