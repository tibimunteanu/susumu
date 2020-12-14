#pragma once

#include "core.h"

namespace susumu {

	class SUSUMU_API Game
	{
	public:
		Game();
		virtual ~Game();

		void Run();
	};

	//to be defined in client
	Game* CreateGame();

}
