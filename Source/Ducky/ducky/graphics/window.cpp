#include "ducky_pch.h"
#include "window.h"
#include "utils\debug.h"
#ifdef DUCKY_WINDOWS
#include "GL\wglew.h"
#endif

namespace ducky { namespace graphics {

	Window* Window::Instance = nullptr;

	Window::Window(const WindowDesc& wd)
	{
		Instance = this;
		m_Desc = wd;

		Uint32 flags = SDL_WINDOW_OPENGL;
		if (wd.resizable)
			flags |= SDL_WINDOW_RESIZABLE;
		if (wd.fullscreen == 1)
			flags |= SDL_WINDOW_FULLSCREEN;
		else if (wd.fullscreen == 2)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		else if (wd.fullscreen == 3)
			flags |= SDL_WINDOW_BORDERLESS;
		
		if (wd.mouseGrabbed)
		{
			flags |= SDL_WINDOW_INPUT_FOCUS;
			flags |= SDL_WINDOW_INPUT_GRABBED;
			flags |= SDL_WINDOW_MOUSE_CAPTURE;
			flags |= SDL_WINDOW_MOUSE_FOCUS;
		}

		SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, ((wd.mouseGrabbed == 0) ? "0" : "1"));

#ifndef DUCKY_WEB
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG | (wd.debugContext == 1 ? SDL_GL_CONTEXT_DEBUG_FLAG : 0));
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		if (wd.openglMajorVersion != 0)
		{
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, wd.openglMajorVersion);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, wd.openglMinorVersion);
		}

		if (wd.msaa != 0)
		{
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, wd.msaa);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		}
#endif

		SDL_SetRelativeMouseMode((SDL_bool)(wd.mouseGrabbed == 1));


		m_Window = SDL_CreateWindow(wd.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wd.width, wd.height, flags);

		m_Context = SDL_GL_CreateContext(m_Window);
#ifndef DUCKY_WEB
		glewExperimental = GL_TRUE;
		GLenum r = glewInit();
		glGetError();

		SDL_GL_SetSwapInterval(wd.vsync);
#endif

		if (wd.fullscreen == 2)
		{
			int w, h;
			SDL_GetWindowSize(m_Window, &w, &h);
			m_Desc.width = w;
			m_Desc.height = h;
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		if (wd.msaa != 0)
		{
#ifndef DUCKY_WEB
			glEnable(GL_MULTISAMPLE);
#endif
		}

		InputDesc id;
		id.mouseRelative = wd.mouseGrabbed == 1;
		id.width = m_Desc.width;
		id.height = m_Desc.height;
		m_Input = new Input(id);

		m_DeltaTime = 0;
		m_Time = SDL_GetTicks();
	}

	Window::~Window()
	{
		SDL_DestroyWindow(m_Window);
		SDL_GL_DeleteContext(m_Context);
	}

	void Window::SetDisplayMode(uint width, uint height, int refreshRate)
	{
#ifndef DUCKY_WEB
		SDL_DisplayMode dm;
		dm.driverdata = 0;
		dm.format = SDL_PIXELFORMAT_RGB24;
		dm.refresh_rate = refreshRate;
		dm.w = width;
		dm.h = height;
		m_Desc.width = width;
		m_Desc.height = height;
		SDL_SetWindowDisplayMode(m_Window, &dm);
#endif
	}

	void Window::Update()
	{
		m_Input->Update();

		SDL_GL_SwapWindow(m_Window);

		uint32 t = SDL_GetTicks();
		m_DeltaTime = float(t - m_Time) / 1000.0f;
		m_Time = t;
	}

	const WindowDesc& Window::GetDesc()
	{
		return m_Desc;
	}

	void Window::SetTimeFactor(float f)
	{
		m_TimeFactor = f;
	}

	float Window::GetTimeFactor()
	{
		return m_TimeFactor;
	}

	float Window::GetDeltaTimeRaw()
	{
		return m_DeltaTime;
	}

	float Window::GetDeltaTime()
	{
		return m_DeltaTime * m_TimeFactor;
	}

	uint32 Window::GetTime()
	{
		return m_Time;
	}

	uint32 Window::GetAbsoluteTime()
	{
		return SDL_GetTicks();
	}

	void Window::ResetTimer()
	{
		m_Time = SDL_GetTicks();
	}
	
	void Window::PrintSystemSpecs()
	{
#ifndef DUCKY_WEB
		int numExtensions;
		glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

		Debug::Printer << ENDL;
		Debug::Printer << INFO << TLOG << "Extensions: " << numExtensions << ENDL;
		for (int i = 0; i < numExtensions; i++)
			Debug::Printer << TLOG << (const char*)glGetStringi(GL_EXTENSIONS, i) << ENDL;

		Debug::Printer << WARN << ENDL;
		Debug::Printer << TLOG << "System specs:" << ENDL;
		Debug::Printer << TLOG << "--------------------------------------" << ENDL;
		Debug::Printer << TLOG << ENDL;
		Debug::Printer << TLOG << "Renderer: " << (const char*)glGetString(GL_RENDERER) << ENDL;
		Debug::Printer << TLOG << "Vendor:   " << (const char*)glGetString(GL_VENDOR) << ENDL;

#ifdef GL_NVX_gpu_memory_info
		GLint nTotalMemoryInKB = 0;
		glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX,
			&nTotalMemoryInKB);

		GLint nCurAvailMemoryInKB = 0;
		glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX,
			&nCurAvailMemoryInKB);

		int nTotalMemoryInMB = nTotalMemoryInKB / 1024;
		int nTotalMemoryInGB = nTotalMemoryInMB / 1024;

		int nCurAvailMemoryInMB = nCurAvailMemoryInKB / 1024;
		int nCurAvailMemoryInGB = nCurAvailMemoryInMB / 1024;

		Debug::Printer << TLOG << "Total memory: " << nTotalMemoryInGB << "," << nTotalMemoryInMB % 1024 << " GB" << ENDL;
		Debug::Printer << TLOG << "Available memory: " << nCurAvailMemoryInGB << "," << nCurAvailMemoryInMB % 1024 << " GB" << ENDL;

#elif defined(GL_ATI_meminfo)

		GLuint uNoOfGPUs = wglGetGPUIDsAMD(0, 0);
		GLuint* uGPUIDs = new GLuint[uNoOfGPUs];
		wglGetGPUIDsAMD(uNoOfGPUs, uGPUIDs);

		GLuint uTotalMemoryInMB = 0;
		wglGetGPUInfoAMD(uGPUIDs[0],
			WGL_GPU_RAM_AMD,
			GL_UNSIGNED_INT,
			sizeof(GLuint),
			&uTotalMemoryInMB);
		
		GLint nCurAvailMemoryInKB = 0;
		glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI,
			&nCurAvailMemoryInKB);

		int nTotalMemoryInGB = nTotalMemoryInMB / 1024;

		int nCurAvailMemoryInMB = nCurAvailMemoryInKB / 1024;
		int nCurAvailMemoryInGB = nCurAvailMemoryInMB / 1024;

		Debug::Printer << TLOG << "Total memory: " << nTotalMemoryInGB << "," << nTotalMemoryInMB % 1024 << " GB" << ENDL;
		Debug::Printer << TLOG << "Available memory: " << nCurAvailMemoryInGB << "," << nCurAvailMemoryInMB % 1024 << " GB" << ENDL;

#endif

		Debug::Printer << TLOG << "OpenGL Version: " << (const char*)glGetString(GL_VERSION) << ENDL;
		Debug::Printer << TLOG << "Shading language version: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << ENDL;
		
		Debug::Printer << ENDL;
#endif
	}

	Input* Window::GetInput()
	{
		return m_Input;
	}

	void Window::Close()
	{
		m_Input->Close();
	}

} }