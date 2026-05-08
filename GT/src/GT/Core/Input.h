#pragma once

#include "GT/Core/Base.h"
#include "GT/Core/KeyCodes.h"
#include "GT/Core/MouseButtonCodes.h"

namespace GT {
	class HAZEL_API Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}