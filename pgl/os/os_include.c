#include "os.c"

#if PLATFORM_WINDOWS
#  include "win32/os_win32.c"
#elif PLATFORM_LINUX
#  include "linux/os_linux.c"
#elif PLATFORM_WEB
#  include "web/os_web.c"
#else
#  error OS layer not implemented for this operating system.
#endif
