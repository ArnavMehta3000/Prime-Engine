#include <pch.h>
#include "Input.h"

namespace Prime
{
	Input::Input()
	{
		m_keyboard = std::make_unique<Keyboard>();
		m_mouse = std::make_unique<Mouse>();

		m_mouse->SetWindow(GetActiveWindow());
	}

	void Input::Poll()
	{
		m_kbState = m_keyboard->GetState();
		m_kbTracker.Update(m_kbState);

		m_mouseState = m_mouse->GetState();
		m_mouseTracker.Update(m_mouseState);

		m_mouse->ResetScrollWheelValue();
	}

	bool Input::IsKeyReleased(Keyboard::Keys key)
	{
		if (m_kbTracker.IsKeyReleased(key))
			return true;
		else
			return false;
	}

	bool Input::IsKeyPressed(Keyboard::Keys key)
	{
		if (m_kbTracker.IsKeyPressed(key))
			return true;
		else
			return false;
	}

	bool Input::IsRMBPressed()
	{
		if (m_mouseTracker.rightButton == ButtonState::PRESSED)
			return true;
		else
			return false;
	}

	bool Input::IsLMBPressed()
	{
		if (m_mouseTracker.leftButton == ButtonState::PRESSED)
			return true;
		else
			return false;
	}

	bool Input::IsMMBPressed()
	{
		if (m_mouseTracker.middleButton == ButtonState::PRESSED)
			return true;
		else
			return false;
	}


	bool Input::IsRMBHeld()
	{
		if (m_mouseTracker.rightButton == ButtonState::HELD)
			return true;
		else
			return false;
	}

	bool Input::IsLMBHeld()
	{
		if (m_mouseTracker.leftButton == ButtonState::HELD)
			return true;
		else
			return false;
	}

	bool Input::IsMMBHeld()
	{
		if (m_mouseTracker.middleButton == ButtonState::HELD)
			return true;
		else
			return false;
	}


	bool Input::IsRMBReleased()
	{
		if (m_mouseTracker.rightButton == ButtonState::RELEASED)
			return true;
		else
			return false;
		MousePos p;

	}

	bool Input::IsLMBReleased()
	{
		if (m_mouseTracker.leftButton == ButtonState::RELEASED)
			return true;
		else
			return false;
	}

	bool Input::IsMMBReleased()
	{
		if (m_mouseTracker.middleButton == ButtonState::RELEASED)
			return true;
		else
			return false;
	}

	int Input::GetScrollWheel()
	{
		int s = m_mouseState.scrollWheelValue;

		if (s > 0)
			return 1;
		else if (s < 0)
			return -1;
		else
			return 0;

	}

	Input::MousePos Input::GetMousePos()
	{
		MousePos pos{};
		if (m_mouseState.positionMode == Mouse::MODE_RELATIVE)
		{
			return pos;
		}

		pos.x = m_mouseState.x;
		pos.y = m_mouseState.y;

		return pos;
	}

	int Input::GetMouseHorizontal()
	{
		if (m_mouseState.positionMode == Mouse::MODE_ABSOLUTE)
			return 0;

		if (m_mouseState.x > 0)
			return 1;
		else if (m_mouseState.x < 0)
			return -1;
		else
			return 0;
	}

	int Input::GetMouseVertical()
	{
		if (m_mouseState.positionMode == Mouse::MODE_ABSOLUTE)
			return 0;

		if (m_mouseState.y > 0)
			return 1;
		else if (m_mouseState.y < 0)
			return -1;
		else
			return 0;
	}
}