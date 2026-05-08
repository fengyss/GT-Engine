#include "gtpch.h"
#include <GT.h>


#include "EditorLayer.h"

// -------Entry Point------------
#include "GT/Core/EntryPoint.h"
// ------------------------------

class GTEditor : public GT::Application
{
public:
	GTEditor()
	{
		GT_CORE_INFO("GTEditor Created!");

		PushLayer(new GT::EditorLayer());
		
	}
	GTEditor(const GT::ApplicationSpecification& specification)
		: GT::Application(specification)
	{
		GT_CORE_INFO("GTEditor Created!");
		PushLayer(new GT::EditorLayer());
	}

	~GTEditor()
	{
	}
private:
};

GT::Application* GT::CreateApplication(ApplicationCommandLineArgs args)
{

	ApplicationSpecification spec;
	spec.Name = "GTEditor";
	spec.WorkingDirectory = "./";
	spec.CommandLineArgs = args;

	return new GTEditor(spec);
}