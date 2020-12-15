#pragma once

#include "core.h"
#include "events/event.h"

namespace susumu {

	class SU_API App
	{
	public:
		App();
		virtual ~App();

		void Run();
	};

	//to be defined in client
	App* CreateApp();

}
