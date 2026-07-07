#pragma once

#include "GT/Core/Layer.h"

#include "GT/Events/ApplicationEvent.h"
#include "GT/Events/KeyEvent.h"
#include "GT/Events/MouseEvent.h"

namespace GT
{
	enum  ImGuiTheme
	{
		Dark,
		SoftLight,
		Cyberpunk,
		Unreal,
		VSCode,
	};
	class HAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();
		void SetTheme(ImGuiTheme theme);

		void ApplySoftLightTheme();
		void ApplyCyberpunkTheme();
		void ApplyUnrealTheme();
		void ApplyVSCodeTheme();
		void ApplyDarkTheme();

		void BlockEvents(bool block) { m_Block = block; }

	private:
		float m_Time = 0.0f;
		bool m_Block = false;
	};

}