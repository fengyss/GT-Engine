// tests/Event/EventTests.cpp
#include <gtest/gtest.h>
#include "GT/Events/Event.h"

#include "GT/Events/KeyEvent.h"
#include "GT/Events/MouseEvent.h"
#include "GT/Events/ApplicationEvent.h"

#include "GT/Core/KeyCodes.h"
#include "GT/Core/MouseButtonCodes.h"

using namespace GT;

// check is dispatch correctly call callback, if event type match, otherwise not call callback
#define EXPECT_TRUE_OR_FALSE(condition, event, type, eventclass, message) \
    dispatcher.Dispatch<eventclass>(callback); \
    if (event->GetEventType() == type) { \
        EXPECT_TRUE(condition) << message; \
    } else { \
        EXPECT_FALSE(condition) << message; \
    } \
    condition = false;

TEST(EventDispatcherTest, DispatchCalled)
{
    bool handled = false;

    auto callback = [&](Event& e) {
        handled = true;
        return true;
        };

    std::vector<Event*> events = {
        new WindowResizeEvent(800, 600),
        new WindowCloseEvent(),
		new WindowFocusEvent(),
		new WindowLostFocusEvent(),
		new WindowMovedEvent(100, 200),

        new AppTickEvent(),
		new AppUpdateEvent(),
		new AppRenderEvent(),

        new KeyPressedEvent(Key::F, false),
		new KeyReleasedEvent(Key::A),
		new KeyTypedEvent(Key::B),

        new MouseMovedEvent(100.0f, 200.0f),
		new MouseButtonPressedEvent(Mouse::ButtonLeft),
		new MouseButtonReleasedEvent(Mouse::ButtonRight),
		new MouseScrolledEvent(0.0f, -1.0f)
	};

    for(auto event : events)
    {
        EventDispatcher dispatcher(*event);

        EXPECT_TRUE_OR_FALSE(handled, event, EventType::WindowResize, WindowResizeEvent, "Dispatch not call callback correctly!");
        EXPECT_TRUE_OR_FALSE(handled, event, EventType::WindowClose, WindowCloseEvent, "Dispatch not call callback correctly!");
        EXPECT_TRUE_OR_FALSE(handled, event, EventType::WindowFocus, WindowFocusEvent, "Dispatch not call callback correctly!");
        EXPECT_TRUE_OR_FALSE(handled, event, EventType::WindowLostFocus, WindowLostFocusEvent, "Dispatch not call callback correctly!");
        EXPECT_TRUE_OR_FALSE(handled, event, EventType::WindowMoved, WindowMovedEvent, "Dispatch not call callback correctly!");

        EXPECT_TRUE_OR_FALSE(handled, event, EventType::AppTick, AppTickEvent, "Dispatch not call callback correctly!");
        EXPECT_TRUE_OR_FALSE(handled, event, EventType::AppUpdate, AppUpdateEvent, "Dispatch not call callback correctly!");
        EXPECT_TRUE_OR_FALSE(handled, event, EventType::AppRender, AppRenderEvent, "Dispatch not call callback correctly!");

        EXPECT_TRUE_OR_FALSE(handled, event, EventType::KeyPressed, KeyPressedEvent, "Dispatch not call callback correctly!");
        EXPECT_TRUE_OR_FALSE(handled, event, EventType::KeyReleased, KeyReleasedEvent, "Dispatch not call callback correctly!");
        EXPECT_TRUE_OR_FALSE(handled, event, EventType::KeyTyped, KeyTypedEvent, "Dispatch not call callback correctly!");

        EXPECT_TRUE_OR_FALSE(handled, event, EventType::MouseMoved, MouseMovedEvent, "Dispatch not call callback correctly!");
        EXPECT_TRUE_OR_FALSE(handled, event, EventType::MouseButtonPressed, MouseButtonPressedEvent, "Dispatch not call callback correctly!");
        EXPECT_TRUE_OR_FALSE(handled, event, EventType::MouseButtonReleased, MouseButtonReleasedEvent, "Dispatch not call callback correctly!");
        EXPECT_TRUE_OR_FALSE(handled, event, EventType::MouseScrolled, MouseScrolledEvent, "Dispatch not call callback correctly!");

    }
}