#ifndef OS_INCLUDE_H
#define OS_INCLUDE_H

#include "os.h"

#if PLATFORM_WINDOWS
#  include "win32/os_win32.h"
#elif PLATFORM_LINUX
#  include "linux/os_linux.h"
#elif PLATFORM_WEB
#  include "web/os_web.h"
#else
#  error OS layer not implemented for this operating system.
#endif

#endif // OS_INCLUDE_H
