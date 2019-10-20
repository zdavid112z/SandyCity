#pragma once

#if defined(DUCKY_WIN32) || defined(DUCKY_WIN64)
#define DUCKY_WINDOWS
#define DUCKY_OPENGL
#endif

#ifdef DUCKY_WEB
#define DUCKY_DISABLE_OPENAL
#define DUCKY_DISABLE_ASSIMP
#define DUCKY_OPENGL
#define DUCKY_GLES
#endif

#define DUCKY_USE_CONSOLE

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <glm\glm.hpp>

using std::string;
using std::vector;

typedef int8_t int8;
typedef uint8_t uint8;
typedef uint8_t byte;
typedef int16_t  int16;
typedef uint16_t ushort;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef uint32_t uint;
typedef int64_t int64;
typedef uint64_t uint64;

#define BIT(x) (1<<x)