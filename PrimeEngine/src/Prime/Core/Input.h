#pragma once
#include "Prime/Core/ServiceLocator/IService.h"
#include <Keyboard.h>
#include <Mouse.h>

using ButtonState = Mouse::ButtonStateTracker::ButtonState;

namespace Prime
{
	class Input : public Prime::IService
	{
		struct MousePos
		{
			int x = 0;
			int y = 0;
		};


	public:
		Input();
		void Poll();

		inline Keyboard* GetKeyboard() { return m_keyboard.get(); }
		inline Mouse* GetMouse() { return m_mouse.get(); }

		bool IsKeyReleased(Keyboard::Keys key);
		bool IsKeyPressed(Keyboard::Keys key);

		bool IsRMBPressed();
		bool IsLMBPressed();
		bool IsMMBPressed();

		bool IsRMBHeld();
		bool IsLMBHeld();
		bool IsMMBHeld();

		bool IsRMBReleased();
		bool IsLMBReleased();
		bool IsMMBReleased();

		int GetScrollWheel();

		MousePos GetMousePos();
		int GetMouseHorizontal();
		int GetMouseVertical();

		inline void SetMouseMode(Mouse::Mode mode) { m_mouse->SetMode(mode); }

	private:
		std::unique_ptr<Keyboard>       m_keyboard;
		std::unique_ptr<Mouse>          m_mouse;

		Keyboard::State                 m_kbState;
		Mouse::State                    m_mouseState;

		Keyboard::KeyboardStateTracker  m_kbTracker;
		Mouse::ButtonStateTracker       m_mouseTracker;
	};

}