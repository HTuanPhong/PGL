#ifndef PGL_GRAPHIC_H
#define PGL_GRAPHIC_H

#include "pgl/memory.h"
#include "pgl/string.h"

#if defined(__APPLE__)
#  include <TargetConditionals.h>
#endif

#if (defined(__EMSCRIPTEN__) || defined(__ANDROID__))
#  include <GLES3/gl3.h>
#elif (defined(__APPLE__) && (TARGET_OS_IOS || TARGET_OS_TV))
#  include <OpenGLES/ES3/gl.h>
#else // desktop
#  define DESKTOP_GL 1
#  include <glad/glad.h>
#endif

#if !defined(DESKTOP_GL)
#  define DESKTOP_GL 0
#endif

GLuint CreateShaderProgram(Scratch    *scratch,
                           const char *vertSrc,
                           const char *fragSrc,
                           String     *errLog);

#endif // PGL_GRAPHIC_H
