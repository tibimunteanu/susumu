#include "supch.h"
#include "app.h"

#include "core/events/app_event.h"
#include "core/log.h"

#include <GLFW/glfw3.h>

namespace susumu {

	App::App()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	App::~App()
	{
	}

	void App::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Window->OnUpdate();
		}
	}
}
