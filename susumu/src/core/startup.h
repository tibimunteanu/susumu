#pragma once

#ifdef SU_PLATFORM_WINDOWS

extern susumu::App* susumu::CreateApp();

int main(int argc, char** argv)
{
	susumu::Log::Init();
	SU_CORE_WARN("initialized core logger");
	SU_INFO("initialized app logger");

	auto app = susumu::CreateApp();
	app->Run();
	delete app;

	return 0;
}

#endif
