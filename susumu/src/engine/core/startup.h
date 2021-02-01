#pragma once
#include "engine/core/base.h"

#ifdef SU_PLATFORM_WINDOWS

extern susumu::App* susumu::CreateApp();

int main(int argc, char** argv)
{
    susumu::Log::Init();

    auto app = susumu::CreateApp();
    app->Run();
    delete app;

    return 0;
}

#endif
