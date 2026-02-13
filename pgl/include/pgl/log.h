#ifndef PGL_LOG_H
#define PGL_LOG_H

#include <SDL3/SDL.h>

void _LogImpl(SDL_LogPriority priority, const char *fmt, ...);

#define LogInfo(fmt, ...)  \
  _LogImpl(                \
    SDL_LOG_PRIORITY_INFO, \
    "[INFO][%s:%d] " fmt,  \
    __FILE__,              \
    __LINE__,              \
    ##__VA_ARGS__)

#define LogErr(fmt, ...)    \
  _LogImpl(                 \
    SDL_LOG_PRIORITY_ERROR, \
    "[ERROR][%s:%d] " fmt,  \
    __FILE__,               \
    __LINE__,               \
    ##__VA_ARGS__)
#endif // PGL_LOG_H
