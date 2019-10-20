#pragma once

#include "ducky_pch.h"
#include <SDL\SDL.h>
#include <glm\glm.hpp>

namespace ducky { namespace graphics {

	struct InputDesc
	{
		uint16 width, height;
		bool mouseRelative = false;
	};

	enum ButtonState : byte
	{
		BUTTON_RELEASED = 0,
		BUTTON_JUST_RELEASED = 1,
		BUTTON_PRESSED = 2,
		BUTTON_JUST_PRESSED = 3
	};
	
	class Input
	{
	public:
		Input(const InputDesc&);
		~Input();

		void Update();

		const InputDesc& GetDesc();

		void Close();
		bool ShouldClose();
		bool HasFocus();
		uint16 GetNumKeys();
		uint16 GetNumMouseButtons();
		ButtonState GetKeyState(uint16);
		ButtonState GetMouseButtonState(uint16);
		bool IsKeyDown(uint16);
		bool IsMouseButtonDown(uint16);
		const glm::i32vec2& GetMouseWheel();
		const glm::vec2 GetMousePosition();
		const glm::vec2 GetMouseDelta();
	protected:
		InputDesc m_Desc;
		SDL_Event m_Event;
		bool m_ShouldClose = false;
		bool m_HasFocus = true;
		const uint16 m_NumKeys = SDL_NUM_SCANCODES;
		ButtonState m_Keys[SDL_NUM_SCANCODES];
		const uint16 m_NumMouseButtons = 16;
		ButtonState m_MouseButtons[16];
		glm::i32vec2 m_MouseWheel;
		glm::vec2 m_MousePosition;
		glm::vec2 m_MouseDelta;
	};

} }