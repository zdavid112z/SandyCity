#include "ducky_pch.h"
#include "input.h"

namespace ducky { namespace graphics {
		 
	Input::Input(const InputDesc& id)
	{
		m_Desc = id;
		memset(m_Keys, 0, sizeof(m_Keys));
		memset(m_MouseButtons, 0, sizeof(m_MouseButtons));
	}

	Input::~Input()
	{

	}

	void Input::Update()
	{
		m_MouseDelta = glm::vec2(0);
		for (int i = 0; i < m_NumKeys; i++)
		{
			if (m_Keys[i] == BUTTON_JUST_PRESSED)
				m_Keys[i] = BUTTON_PRESSED;
			if (m_Keys[i] == BUTTON_JUST_RELEASED)
				m_Keys[i] = BUTTON_RELEASED;
		}

		for (int i = 0; i < m_NumMouseButtons; i++)
		{
			if (m_MouseButtons[i] == BUTTON_JUST_PRESSED)
				m_MouseButtons[i] = BUTTON_PRESSED;
			if (m_MouseButtons[i] == BUTTON_JUST_RELEASED)
				m_MouseButtons[i] = BUTTON_RELEASED;
		}

		while (SDL_PollEvent(&m_Event))
		{
			switch (m_Event.type)
			{
			case SDL_MOUSEMOTION:
				if (m_Desc.mouseRelative)
				{
					m_MouseDelta.x = (float)m_Event.motion.xrel / (float)m_Desc.width;
					m_MouseDelta.y = (float)m_Event.motion.yrel / (float)m_Desc.height;
					m_MousePosition.x += m_MouseDelta.x;
					m_MousePosition.y += m_MouseDelta.y;
				}
				else
				{
					m_MousePosition.x = (float)m_Event.motion.x / (float)m_Desc.width;
					m_MousePosition.y = (float)m_Event.motion.y / (float)m_Desc.height;
					m_MouseDelta.x = (float)m_Event.motion.xrel / (float)m_Desc.width;
					m_MouseDelta.y = (float)m_Event.motion.yrel / (float)m_Desc.height;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				m_MouseButtons[m_Event.button.button] = BUTTON_JUST_PRESSED;
				break;
			case SDL_MOUSEBUTTONUP:
				m_MouseButtons[m_Event.button.button] = BUTTON_JUST_RELEASED;
				break;
			case SDL_MOUSEWHEEL:
				m_MouseWheel.x = m_Event.wheel.x;
				m_MouseWheel.y = m_Event.wheel.y;
				break;
			case SDL_KEYDOWN:
				if (m_Event.key.repeat == 0)
				{
					m_Keys[m_Event.key.keysym.scancode] = BUTTON_JUST_PRESSED;
				}
				break;
			case SDL_KEYUP:
				if (m_Event.key.repeat == 0)
				{
					m_Keys[m_Event.key.keysym.scancode] = BUTTON_JUST_RELEASED;
				}
				break;
			case SDL_QUIT:
				m_ShouldClose = true;
				break;
			case SDL_WINDOWEVENT:
				switch (m_Event.window.event)
				{
				case SDL_WINDOWEVENT_FOCUS_LOST:
					m_HasFocus = false;
					if (m_Desc.mouseRelative)
						SDL_SetRelativeMouseMode(SDL_FALSE);
					break;
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					m_HasFocus = true;
					if (m_Desc.mouseRelative)
						SDL_SetRelativeMouseMode(SDL_TRUE);
					break;
				}
				break;
			}
		}
	}

	const InputDesc& Input::GetDesc()
	{
		return m_Desc;
	}

	void Input::Close()
	{
		m_ShouldClose = true;
	}

	bool Input::ShouldClose()
	{
		return m_ShouldClose;
	}

	bool Input::HasFocus()
	{
		return m_HasFocus;
	}

	uint16 Input::GetNumKeys()
	{
		return m_NumKeys;
	}

	uint16 Input::GetNumMouseButtons()
	{
		return m_NumMouseButtons;
	}

	ButtonState Input::GetKeyState(uint16 i)
	{
		return m_Keys[i];
	}

	ButtonState Input::GetMouseButtonState(uint16 i)
	{
		return m_MouseButtons[i];
	}

	bool Input::IsKeyDown(uint16 i)
	{
		return (m_Keys[i] & 2) != 0;
	}

	bool Input::IsMouseButtonDown(uint16 i)
	{
		return (m_MouseButtons[i] & 2) != 0;
	}

	const glm::i32vec2& Input::GetMouseWheel()
	{
		return m_MouseWheel;
	}

	const glm::vec2 Input::GetMousePosition()
	{
		return m_MousePosition;
	}

	const glm::vec2 Input::GetMouseDelta()
	{
		return m_MouseDelta;
	}

} }