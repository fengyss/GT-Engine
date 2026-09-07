// tests/Event/EventTests.cpp
#include <gtest/gtest.h>
#include "GT/Events/Event.h"

#include "GT/Events/KeyEvent.h"
#include "GT/Events/MouseEvent.h"
#include "GT/Events/ApplicationEvent.h"

#include "GT/Core/KeyCodes.h"
#include "GT/Core/MouseButtonCodes.h"

#include "GT/Math/Random.h"

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

class EventSystemTest : public ::testing::Test
{
public:
protected:
    std::vector<Event*> m_Events = {
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

    std::vector<int> m_keyCodes = { 
		    Key::Space ,
			Key::Apostrophe , /* ' */
			Key::Comma , /* , */
			Key::Minus , /* - */
			Key::Period , /* . */
			Key::Slash , /* / */

			Key::D0 , /* 0 */
			Key::D1 , /* 1 */
			Key::D2 , /* 2 */
			Key::D3 , /* 3 */
			Key::D4 , /* 4 */
			Key::D5 , /* 5 */
			Key::D6 , /* 6 */
			Key::D7 , /* 7 */
			Key::D8 , /* 8 */
			Key::D9 , /* 9 */

			Key::Semicolon , /* ; */
			Key::Equal , /* = */

			Key::A ,
			Key::B ,
			Key::C ,
			Key::D ,
			Key::E ,
			Key::F ,
			Key::G ,
			Key::H ,
			Key::I ,
			Key::J ,
			Key::K ,
			Key::L ,
			Key::M ,
			Key::N ,
			Key::O ,
			Key::P ,
			Key::Q ,
			Key::R ,
			Key::S ,
			Key::T ,
			Key::U ,
			Key::V ,
			Key::W ,
			Key::X ,
			Key::Y ,
			Key::Z ,

			Key::LeftBracket ,  /* [ */
			Key::Backslash ,  /* \ */
			Key::RightBracket ,  /* ] */
			Key::GraveAccent ,  /* ` */

			Key::World1 , /* non-US #1 */
			Key::World2 , /* non-US #2 */

			/* Function keys */
			Key::Escape ,
			Key::Enter ,
			Key::Tab ,
			Key::Backspace ,
			Key::Insert ,
			Key::Delete ,
			Key::Right ,
			Key::Left ,
			Key::Down ,
			Key::Up ,
			Key::PageUp ,
			Key::PageDown ,
			Key::Home ,
			Key::End ,
			Key::CapsLock ,
			Key::ScrollLock ,
			Key::NumLock ,
			Key::PrintScreen ,
			Key::Pause ,
			Key::F1 ,
			Key::F2 ,
			Key::F3 ,
			Key::F4 ,
			Key::F5 ,
			Key::F6 ,
			Key::F7 ,
			Key::F8 ,
			Key::F9 ,
			Key::F10 ,
			Key::F11 ,
			Key::F12 ,
			Key::F13 ,
			Key::F14 ,
			Key::F15 ,
			Key::F16 ,
			Key::F17 ,
			Key::F18 ,
			Key::F19 ,
			Key::F20 ,
			Key::F21 ,
			Key::F22 ,
			Key::F23 ,
			Key::F24 ,
			Key::F25 ,

			Key::KP0 ,
			Key::KP1 ,
			Key::KP2 ,
			Key::KP3 ,
			Key::KP4 ,
			Key::KP5 ,
			Key::KP6 ,
			Key::KP7 ,
			Key::KP8 ,
			Key::KP9 ,
			Key::KPDecimal ,
			Key::KPDivide ,
			Key::KPMultiply ,
			Key::KPSubtract ,
			Key::KPAdd ,
			Key::KPEnter ,
			Key::KPEqual ,

			Key::LeftShift ,
			Key::LeftControl ,
			Key::LeftAlt ,
			Key::LeftSuper ,
			Key::RightShift ,
			Key::RightControl ,
			Key::RightAlt ,
			Key::RightSuper ,
			Key::Menu  
    };

	std::vector<int> m_mouseCodes = {
		Mouse::Button0,
		Mouse::Button1,
		Mouse::Button2,
		Mouse::Button3,
		Mouse::Button4,
		Mouse::Button5,
		Mouse::Button6,
		Mouse::Button7,

		Mouse::ButtonLeft,
		Mouse::ButtonRight,
		Mouse::ButtonMiddle,
		Mouse::ButtonLast,
	};
};

TEST_F(EventSystemTest, DispatchCalled)
{
    bool handled = false;

    auto callback = [&](Event& e) {
        handled = true;
        return true;
        };

	
    for(auto& event : m_Events)
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

TEST_F(EventSystemTest, KeyEventDispatchedCorrectly) {

    int capturedKeyCode = 0;

    for (auto keyCode : m_keyCodes)
    {
        {
            KeyPressedEvent event(keyCode);
            EventDispatcher dispatcher(event);
            dispatcher.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& e) {

                capturedKeyCode = e.GetKeyCode();
                return true;
                });


            EXPECT_EQ(capturedKeyCode, keyCode);
            EXPECT_FALSE(event.IsRepeat());
        }

        {
            KeyPressedEvent event(keyCode, true);
            EventDispatcher dispatcher(event);
            dispatcher.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& e) {

                capturedKeyCode = e.GetKeyCode();
                return true;
                });


            EXPECT_EQ(capturedKeyCode, keyCode);
            EXPECT_TRUE(event.IsRepeat());
        }

        {
            KeyReleasedEvent event(keyCode);
            EventDispatcher dispatcher(event);
            dispatcher.Dispatch<KeyReleasedEvent>([&](KeyReleasedEvent& e) {
                e.Handled = true;
                capturedKeyCode = e.GetKeyCode();
                return true;
                });

            EXPECT_EQ(capturedKeyCode, keyCode);
        }

        {
            KeyPressedEvent event(keyCode);
            EventDispatcher dispatcher(event);
            dispatcher.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& e) {
                e.Handled = true;
                capturedKeyCode = e.GetKeyCode();
                return true;
                });

            EXPECT_EQ(capturedKeyCode, keyCode);
        }
    }
}

TEST_F(EventSystemTest, MouseEventDispatchedCorrectly) {

	int capturedMouseCode = 0;
	float capturedx, capturedy;

	for (auto mouseCode : m_mouseCodes)
	{
		{
			MouseButtonPressedEvent event(mouseCode);
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<MouseButtonPressedEvent>([&](MouseButtonPressedEvent& e) {

				capturedMouseCode = e.GetMouseButton();
				return true;
				});

			EXPECT_EQ(capturedMouseCode, mouseCode);
		}

		{
			MouseButtonReleasedEvent event(mouseCode);
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<MouseButtonReleasedEvent>([&](MouseButtonReleasedEvent& e) {

				capturedMouseCode = e.GetMouseButton();
				return true;
				});

			EXPECT_EQ(capturedMouseCode, mouseCode);
		}


		float x = Random::Range(-300.0, 300.0), y = Random::Range(-300.0, 300.0);
		{
			MouseMovedEvent event(x,y);
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<MouseMovedEvent>([&](MouseMovedEvent& e) {
				capturedx = e.GetX();
				capturedy = e.GetY();
				return true;
				});
			EXPECT_FLOAT_EQ(capturedx, x);
			EXPECT_FLOAT_EQ(capturedy, y);
		}

		{
			MouseScrolledEvent event(x, y);
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<MouseScrolledEvent>([&](MouseScrolledEvent& e) {
				capturedx = e.GetXOffset();
				capturedy = e.GetYOffset();
				return true;
				});
			EXPECT_FLOAT_EQ(capturedx, x);
			EXPECT_FLOAT_EQ(capturedy, y);
		}
	}
}