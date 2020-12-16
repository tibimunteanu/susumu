#pragma once

#include "core.h"
#include "window.h"
#include "core/layer_stack.h"
#include "core/events/event.h"
#include "core/events/app_event.h"

namespace susumu {

	class SU_API App
	{
	public:
		App();
		virtual ~App();

		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static App& Get() { return *s_Instance; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		static App* s_Instance;
	};

	//to be defined in client
	App* CreateApp();

}
