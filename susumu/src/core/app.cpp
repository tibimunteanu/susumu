#include "app.h"

#include "core/events/app_event.h"
#include "core/log.h"

namespace susumu {

	App::App()
	{
	}

	App::~App()
	{
	}

	void App::Run()
	{
		WindowResizeEvent e(1280, 720);

		SU_TRACE(e);

		while (true);
	}
}
