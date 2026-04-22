#ifndef PGL_GRAPHIC_H
#define PGL_GRAPHIC_H

#include <pgl/defines.h>
#include <pgl/memory.h>
#include <pgl/string.h>

#if PLATFORM_EMSCRIPTEN || PLATFORM_ANDROID
#  include <GLES3/gl3.h>
#elif PLATFORM_IOS
#  include <OpenGLES/ES3/gl.h>
#elif PLATFORM_WINDOWS || PLATFORM_LINUX || PLATFORM_MACOS
#  include <glad/glad.h>
#endif

#define GLSL(src) cstr_from_lit(#src)

GLuint shader_program_create(Scratch *scratch,
                             CStr     vert_src,
                             CStr     frag_src,
                             StrBuf  *log_out);

#endif // PGL_GRAPHIC_H
