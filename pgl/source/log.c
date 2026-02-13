#include <SDL3/SDL.h>
#include <pgl/log.h>
#include <stdarg.h>

void _LogImpl(SDL_LogPriority priority, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  SDL_LogMessageV(
    SDL_LOG_CATEGORY_APPLICATION,
    priority,
    fmt,
    args);

  va_end(args);
}
