#pragma once

#ifdef DUCKY_WINDOWS
#include <GL/glew.h>
#elif defined(DUCKY_WEB)
#include <emscripten\emscripten.h>
#include <GLES3\gl3.h>
#endif