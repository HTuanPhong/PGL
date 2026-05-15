// Info

function OS_SystemInfo *
os_get_system_info(void) {
  return &os_w32_state.system_info;
}

function OS_ProcessInfo *
os_get_process_info(void) {
  return &os_w32_state.process_info;
}

// Memory

function void *
os_memory_reserve(u64 size) {
  void *result = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  return (result);
}

function void
os_memory_release(void *ptr, u64 size) {
  VirtualFree(ptr, 0, MEM_RELEASE);
}

// File System

// Abort

function void
os_abort(i32 exit_code) {
  ExitProcess(exit_code);
}

// Time

function u64
os_w32_unix_time_from_file_time(FILETIME file_time) {
  u64 win32_time = ((u64)file_time.dwHighDateTime << 32) | file_time.dwLowDateTime;
  u64 unix_time64 = ((win32_time - 0x19DB1DED53E8000ULL) / 10000000);
  return unix_time64;
}

function u64
os_now_microseconds(void) {
  u64           result = 0;
  LARGE_INTEGER large_int_counter;
  if (QueryPerformanceCounter(&large_int_counter)) {
    result = (large_int_counter.QuadPart * Million(1)) / os_w32_state.microsecond_resolution;
  }
  return result;
}

function void
os_sleep_milliseconds(u32 msec) {
  Sleep(msec);
}

function u64
os_now_unix(void) {
  FILETIME file_time;
  GetSystemTimeAsFileTime(&file_time);
  u64 unix_time = os_w32_unix_time_from_file_time(file_time);
  return unix_time;
}

// Entry

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow); {
  
  // attach console if launch from one
  if (AttachConsole(ATTACH_PARENT_PROCESS)) {
    FILE *res_fp = 0;
    // Redirect unbuffered STDOUT to the console
    freopen_s(&res_fp, "CON", "w", stdout);
    freopen_s(&res_fp, "CON", "w", stderr);
  }
  // get resolution for timer
  {
    os_w32_state.microsecond_resolution = 1;
    LARGE_INTEGER large_int_resolution;
    if (QueryPerformanceFrequency(&large_int_resolution)) {
      os_w32_state.microsecond_resolution = large_int_resolution.QuadPart;
    }
  }

  // get system info
  {
    SYSTEM_INFO sysinfo = { 0 };
    GetSystemInfo(&sysinfo);
    OS_SystemInfo *info = &os_w32_state.system_info;
    info->logical_processor_count = sysinfo.dwNumberOfProcessors;
    info->page_size = sysinfo.dwPageSize;
    assert(is_pow2(info->page_size));
  }

  // TODO: init thread context
  ScratchMarker m = scratch_begin(0);

  // get process info
  {
    OS_ProcessInfo *info = &os_w32_state.process_info;
    {
      ScratchMarker info_m = scratch_begin(0);
      DWORD         size = KB(32);
      u16          *buffer = scratch_push_array(info_m.scratch, u16, size);
      DWORD         length = GetModuleFileNameW(0, (WCHAR *)buffer, size);
      Str8          path = str8_from_str16(m.scratch, (Str16){ buffer, length });
      info->binary_path = str8_get_left_of_last_slash(path);
      info->binary_name = str8_get_right_of_last_slash(path);
      scratch_end(info_m);
    }
    {
      ScratchMarker info_m = scratch_begin(0);
      DWORD         length = GetCurrentDirectoryW(0, 0);
      u16          *buffer = scratch_push_array(info_m.scratch, u16, length + 1);
      length = GetCurrentDirectoryW(length + 1, (WCHAR *)buffer);
      info->initial_path = str8_from_str16(m.scratch, (Str16){ buffer, length });
      scratch_end(info_m);
    }
  }

  // args


  int     argc = __argc;
  WCHAR **wargv = __wargv;
  char  **argv = scratch_push_array(m.scratch, char *, argc);
  for (int i = 0; i < argc; i += 1) {
    Str16 arg16 = str16_from_cstring((u16 *)wargv[i]);
    Str8  arg8 = str8_from_str16(m.scratch, arg16);
    argv[i] = (char *)arg8.value;
    printf(FMT_STR8 "\n", str8_varg(arg8));
  }

  // base entry
  main_thread_entry_point(); // base

  scratch_end(m);
  return 0;
}
