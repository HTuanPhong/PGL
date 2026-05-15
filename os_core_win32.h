#ifndef OS_CORE_WIN32_H
#define OS_CORE_WIN32_H

#define NOMINMAX
#undef function // window have a function word somewhere in there >: (
#include <windows.h>
#define function static

typedef struct OS_W32_State {
  OS_SystemInfo  system_info;
  OS_ProcessInfo process_info;
  u64            microsecond_resolution;
} OS_W32_State;

global OS_W32_State os_w32_state = { 0 };

#endif
