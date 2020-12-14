#pragma once

#ifdef SUSUMU_PLATFORM_WINDOWS

extern susumu::Game* susumu::CreateGame();

int main(int argc, char** argv)
{
	printf("susumu started!");
	auto game = susumu::CreateGame();
	game->Run();
	delete game;

	return 0;
}

#endif
