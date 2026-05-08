#pragma once

#ifdef GT_PLATFORM_WINDOWS

extern GT::Application* GT::CreateApplication(GT::ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	//GT_CORE_INFO("GT Engine Starting..."); //uninnitialized logger

	GT::Log::Init();

	GT_PROFILE_BEGIN_SESSION("Startup", "GTProfile-Startup.json");
	auto app = GT::CreateApplication({ argc,argv });
	GT_PROFILE_END_SESSION();

	GT_PROFILE_BEGIN_SESSION("Runtime", "GTProfile-Runtime.json");
	app->Run();
	GT_PROFILE_END_SESSION();

	GT_PROFILE_BEGIN_SESSION("Shutdown", "GTProfile-Shutdown.json");
	delete app;
	GT_PROFILE_END_SESSION();
	return 0;
}
#endif
