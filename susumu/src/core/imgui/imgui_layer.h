#pragma once

#include "core/layer.h"
#include "core/events/app_event.h"
#include "core/events/key_event.h"
#include "core/events/mouse_event.h"

namespace susumu {

	class SU_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}
