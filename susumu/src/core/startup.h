#pragma once

#ifdef SU_PLATFORM_WINDOWS

extern susumu::Game* susumu::CreateGame();

int main(int argc, char** argv)
{
	susumu::Log::Init();
	SU_CORE_WARN("initialized core logger");
	SU_INFO("initialized client logger");

	auto game = susumu::CreateGame();
	game->Run();
	delete game;

	return 0;
}

#endif
