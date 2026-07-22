#pragma once

namespace GT {

	// This class is responsible for registering the C++ functions and components that can be called from C# scripts.
	// it allows for seamless integration between the C++ engine and the C# scripting environment.
	class ScriptGlue
	{
	public:
		static void RegisterComponents();
		static void RegisterFunctions();
	};



}