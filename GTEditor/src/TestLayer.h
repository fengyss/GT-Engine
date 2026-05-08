#pragma once
#include "GT.h"
#include "GT/Math/Math.h"

#include "GT/Renderer/Model.h"
namespace GT
{
	class TestLayer : public Layer
	{
	public:
		TestLayer();
		~TestLayer();
		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(GT::Timestep ts) override;
		void OnEvent(GT::Event& event) override;

		void OnImGuiRender() override;
	private:
		Ref<Model> m_Model;
		Ref<Shader> m_Shader;	
	};
}


