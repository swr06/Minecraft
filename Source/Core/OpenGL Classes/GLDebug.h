#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <string>
#include <iostream>

#define DebugGL CheckOpenGLError(__FILE__, __LINE__);
#define DebugGLFunction(x) x; CheckOpenGLError(__FILE__, __LINE__);

GLenum CheckOpenGLError(const char* File, int Line);