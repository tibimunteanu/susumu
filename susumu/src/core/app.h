#pragma once

#include "core.h"
#include "events/event.h"
#include "core/events/app_event.h"
#include "window.h"

namespace susumu {

	class SU_API App
	{
	public:
		App();
		virtual ~App();

		void Run();
		void OnEvent(Event& e);
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	//to be defined in client
	App* CreateApp();

}
