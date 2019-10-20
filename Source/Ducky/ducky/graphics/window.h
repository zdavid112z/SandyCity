#pragma once

#include "ducky_pch.h"
#include <SDL\SDL.h>
//#undef main
#include "opengl.h"
#include "input.h"

namespace ducky { namespace graphics {

	struct WindowDesc
	{
		int16 width;
		int16 height;
		string title;
		// 0 = windowed, 1 = fullscreen, 2 = desktop fullscreen, 3 = borderless
		int8 fullscreen = 0;
		// 0 = off, 1 = on, 2 = half
		int8 vsync = 1;
		bool resizable = false;
		// 0 = off, 2, 4, 8, 16
		int8 msaa = 0;
		int8 openglMajorVersion = 0;
		int8 openglMinorVersion = 0;
		int8 mouseGrabbed = 0;
		int8 debugContext = 1;
	};

	class Window 
	{
	public:
		static Window* Instance;
	public:
		Window(const WindowDesc&);
		~Window();

		void SetDisplayMode(uint width, uint height, int refreshRate);
		void Update();
		const WindowDesc& GetDesc();

		void SetTimeFactor(float);
		float GetTimeFactor();
		float GetDeltaTimeRaw();
		float GetDeltaTime();
		// in miliseconds
		uint32 GetTime();
		// in miliseconds
		uint32 GetAbsoluteTime();
		void ResetTimer();
		void PrintSystemSpecs();
		Input* GetInput();
		void Close();

	protected:
		WindowDesc m_Desc;
		Input* m_Input;
		SDL_Window* m_Window;
		SDL_GLContext m_Context;
		uint32 m_Time;
		float m_DeltaTime;
		float m_TimeFactor = 1;
	};

} }