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

function f64
os_now_seconds(void) {
  LARGE_INTEGER qpc;
  QueryPerformanceCounter(&qpc);
  const u64 now = muldiv_u64(qpc.QuadPart - os_w32_state.start.QuadPart, 1000000000, os_w32_state.freq.QuadPart);
  return (f64)now / 1000000000.0;
}

function void
os_sleep_seconds(double sec) {
  DWORD ms = (DWORD)(sec * 1000.0);
  Sleep(ms);
}

function u64
os_w32_unix_time_from_file_time(FILETIME file_time) {
  u64 win32_time = ((u64)file_time.dwHighDateTime << 32) | file_time.dwLowDateTime;
  u64 unix_time64 = ((win32_time - 0x19DB1DED53E8000ULL) / 10000000);
  return unix_time64;
}

function u64
os_now_unix_seconds(void) {
  FILETIME file_time;
  GetSystemTimeAsFileTime(&file_time);
  u64 unix_time = os_w32_unix_time_from_file_time(file_time);
  return unix_time;
}

// Entry and flows

function void
os_w32_init_time(void) {
  QueryPerformanceFrequency(&os_w32_state.freq);
  QueryPerformanceCounter(&os_w32_state.start);
}

function void
os_w32_init_console(void) {
  if (AttachConsole(ATTACH_PARENT_PROCESS)) {
    FILE *res_fp = 0;
    freopen_s(&res_fp, "CON", "w", stdout);
    freopen_s(&res_fp, "CON", "w", stderr);
  }
  os_w32_state.original_codepage = GetConsoleOutputCP();
  SetConsoleOutputCP(CP_UTF8);
}

function void
os_w32_restore_console(void) {
  SetConsoleOutputCP(os_w32_state.original_codepage);
}

function void
os_w32_init_system_info(void) {
  SYSTEM_INFO sysinfo = { 0 };
  GetSystemInfo(&sysinfo);
  os_app_data.system_info.logical_processor_count = sysinfo.dwNumberOfProcessors;
  os_app_data.system_info.page_size = sysinfo.dwPageSize;
  assert(is_pow2(os_app_data.system_info.page_size));
}

function void
os_w32_init_process_info(Scratch *scratch) {
  ScratchMarker m = scratch_begin(0);
  // get binary path
  {
    DWORD size = KB(32);
    u16  *buffer = scratch_push_array(m.scratch, u16, size);
    DWORD length = GetModuleFileNameW(0, (WCHAR *)buffer, size);
    Str8  path = str8_from_str16(scratch, (Str16){ buffer, length });
    os_app_data.process_info.binary_path = str8_get_left_of_last_slash(path);
    os_app_data.process_info.binary_name = str8_get_right_of_last_slash(path);
  }

  // get initial path
  {
    DWORD length = GetCurrentDirectoryW(0, 0);
    u16  *buffer = scratch_push_array(m.scratch, u16, length + 1);
    length = GetCurrentDirectoryW(length + 1, (WCHAR *)buffer);
    os_app_data.process_info.initial_path = str8_from_str16(scratch, (Str16){ buffer, length });
  }
  scratch_end(m);
}

function void
os_w32_init_args(Scratch *scratch) {
  // int     argc = __argc;
  // WCHAR **wargv = __wargv;
  // char  **argv = scratch_push_array(scratch, char *, argc);
  // for (int i = 0; i < argc; i += 1) {
  //   Str16 arg16 = str16_from_cstring((u16 *)wargv[i]);
  //   Str8  arg8 = str8_from_str16(scratch, arg16);
  //   argv[i] = (char *)arg8.value;
  // }
}

function void os_w32_init_dpi(void) {
  DECLARE_HANDLE(DPI_AWARENESS_CONTEXT_T);
  typedef BOOL(WINAPI * SETPROCESSDPIAWARE_T)(void);
  typedef BOOL(WINAPI * SETPROCESSDPIAWARENESSCONTEXT_T)(DPI_AWARENESS_CONTEXT_T); // since Windows 10, version 1703
  typedef HRESULT(WINAPI * SETPROCESSDPIAWARENESS_T)(PROCESS_DPI_AWARENESS);
  typedef HRESULT(WINAPI * GETDPIFORMONITOR_T)(HMONITOR, MONITOR_DPI_TYPE, UINT *, UINT *);

  SETPROCESSDPIAWARE_T            fn_setprocessdpiaware = 0;
  SETPROCESSDPIAWARENESS_T        fn_setprocessdpiawareness = 0;
  GETDPIFORMONITOR_T              fn_getdpiformonitor = 0;
  SETPROCESSDPIAWARENESSCONTEXT_T fn_setprocessdpiawarenesscontext = 0;

  HINSTANCE user32 = LoadLibraryA("user32.dll");
  if (user32) {
    fn_setprocessdpiaware = (SETPROCESSDPIAWARE_T)(void *)GetProcAddress(user32, "SetProcessDPIAware");
    fn_setprocessdpiawarenesscontext = (SETPROCESSDPIAWARENESSCONTEXT_T)(void *)GetProcAddress(user32, "SetProcessDpiAwarenessContext");
  }
  HINSTANCE shcore = LoadLibraryA("shcore.dll");
  if (shcore) {
    fn_setprocessdpiawareness = (SETPROCESSDPIAWARENESS_T)(void *)GetProcAddress(shcore, "SetProcessDpiAwareness");
    fn_getdpiformonitor = (GETDPIFORMONITOR_T)(void *)GetProcAddress(shcore, "GetDpiForMonitor");
  }
  /*
      NOTE on SetProcessDpiAware() vs SetProcessDpiAwareness() vs SetProcessDpiAwarenessContext():

      These are different attempts to get DPI handling on Windows right, from oldest
      to newest. SetProcessDpiAwarenessContext() is required for the new
      DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 method.
  */
  if (fn_setprocessdpiawareness) {
    // first try the Win10 Creator Update per-monitor-dpi awareness, if that fails, fall back to system-dpi-awareness
    // NOTE: if DPI awareness had already been set otherwise (e.g. via manifest.xml) both calls will fail
    DPI_AWARENESS_CONTEXT_T per_monitor_aware_v2 = (DPI_AWARENESS_CONTEXT_T)-4;
    if (!(fn_setprocessdpiawarenesscontext && fn_setprocessdpiawarenesscontext(per_monitor_aware_v2))) {
      // fallback to system-dpi-aware
      fn_setprocessdpiawareness(PROCESS_SYSTEM_DPI_AWARE);
    }
  } else if (fn_setprocessdpiaware) {
    // fallback for Windows 7
    fn_setprocessdpiaware();
  }
  // get dpi scale factor for main monitor
  if (fn_getdpiformonitor) {
    POINT    pt = { os_app_init_setting.position.x, os_app_init_setting.position.y };
    HMONITOR hm = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
    UINT     dpix, dpiy;
    HRESULT  hr = fn_getdpiformonitor(hm, MDT_EFFECTIVE_DPI, &dpix, &dpiy);
    assert(SUCCEEDED(hr));
    // clamp window scale to an integer factor
    os_app_data.window.dpi_scale = (float)dpix / 96.0f;

  } else {
    assert_always(!"gay path");
  }
  if (user32) {
    FreeLibrary(user32);
  }
  if (shcore) {
    FreeLibrary(shcore);
  }
}

function void
os_w32_init_cursors(void) {
#define X(mine, win32) os_w32_state.cursors[OS_CURSOR_##mine] = LoadCursor(0, win32);
  OS_WIN32_CURSOR_MAP_TABLE
#undef X
  os_app_data.input.cursor.current = OS_CURSOR_POINTER;
}

function void
os_w32_capture_input() {
  if (0 == os_w32_state.capture_ref_counter) {
    SetCapture(os_w32_state.hwnd);
  }
  os_w32_state.capture_ref_counter += 1;
}

function void
os_w32_release_input() {
  if (0 != os_w32_state.capture_ref_counter) {
    os_w32_state.capture_ref_counter -= 1;
    if (0 == os_w32_state.capture_ref_counter) {
      ReleaseCapture();
    }
  }
}

function void
os_w32_cursor_pos_update(LPARAM lparam) {
  os_app_data.input.cursor.position.x = GET_X_LPARAM(lparam);
  os_app_data.input.cursor.position.y = GET_Y_LPARAM(lparam);
}

function LRESULT CALLBACK
os_w32_wndproc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
  switch (umsg) {
  case WM_CLOSE: {
    // to cancel quit dont post the message maybe add a bool that say unsaved and stuff
    PostQuitMessage(0);
    return 0;
  } break;
  case WM_SETCURSOR: {
    if (LOWORD(lparam) == HTCLIENT) {
      SetCursor(os_w32_state.cursors[os_app_data.input.cursor.current]);
      return TRUE;
    }
  } break;
  case WM_LBUTTONUP: {
    os_w32_cursor_pos_update(lparam);
    os_app_data.input.buttons.current[OS_BUTTON_LEFT_MOUSE] = 0;
    os_w32_release_input();
  } break;
  case WM_MBUTTONUP: {
    os_w32_cursor_pos_update(lparam);
    os_app_data.input.buttons.current[OS_BUTTON_MIDDLE_MOUSE] = 0;
    os_w32_release_input();
  } break;
  case WM_RBUTTONUP: {
    os_w32_cursor_pos_update(lparam);
    os_app_data.input.buttons.current[OS_BUTTON_RIGHT_MOUSE] = 0;
    os_w32_release_input();
  } break;
  case WM_XBUTTONUP: {
    os_w32_cursor_pos_update(lparam);
    switch (GET_XBUTTON_WPARAM(wparam)) {
    case XBUTTON1: {
      os_app_data.input.buttons.current[OS_BUTTON_X1_MOUSE] = 0;
    } break;
    case XBUTTON2: {
      os_app_data.input.buttons.current[OS_BUTTON_X2_MOUSE] = 0;
    } break;
    }
    os_w32_release_input();
  } break;
  case WM_LBUTTONDOWN: {
    os_w32_cursor_pos_update(lparam);
    os_app_data.input.buttons.current[OS_BUTTON_LEFT_MOUSE] = 1;
    os_w32_capture_input();
  } break;
  case WM_MBUTTONDOWN: {
    os_w32_cursor_pos_update(lparam);
    os_app_data.input.buttons.current[OS_BUTTON_MIDDLE_MOUSE] = 1;
    os_w32_capture_input();
  } break;
  case WM_RBUTTONDOWN: {
    os_w32_cursor_pos_update(lparam);
    os_app_data.input.buttons.current[OS_BUTTON_RIGHT_MOUSE] = 1;
    os_w32_capture_input();
  } break;
  case WM_XBUTTONDOWN: {
    os_w32_cursor_pos_update(lparam);
    switch (GET_XBUTTON_WPARAM(wparam)) {
    case XBUTTON1: {
      os_app_data.input.buttons.current[OS_BUTTON_X1_MOUSE] = 1;
    } break;
    case XBUTTON2: {
      os_app_data.input.buttons.current[OS_BUTTON_X2_MOUSE] = 1;
    } break;
    }
    os_w32_capture_input();
  } break;
  case WM_MOUSEMOVE: {
    os_w32_cursor_pos_update(lparam);
  } break;
  case WM_MOUSEWHEEL: {
    os_w32_cursor_pos_update(lparam);
    os_app_data.input.cursor.delta_scroll.y = GET_WHEEL_DELTA_WPARAM(wparam);
  } break;
  case WM_MOUSEHWHEEL: {
    os_w32_cursor_pos_update(lparam);
    os_app_data.input.cursor.delta_scroll.x = GET_WHEEL_DELTA_WPARAM(wparam);
  } break;
  case WM_SYSKEYDOWN:
  case WM_KEYDOWN:    {
    os_app_data.input.buttons.current[os_w32_button_table[wparam]] = 1;
  } break;
  case WM_SYSKEYUP:
  case WM_KEYUP:    {
    os_app_data.input.buttons.current[os_w32_button_table[wparam]] = 0;
  } break;
  case WM_SYSCHAR: {
    return 0;
  } break;
  case WM_CHAR: {
    // u32 c = wparam;
    // if (c >= 32) {
    //   if (c >= 0xD800 && c <= 0xDBFF) {
    //     _sapp.win32.surrogate = (WCHAR)c - 0xD800;
    //   } else {
    //     if (c > 0xDC00 && c <= 0xDFFF) {
    //       c = (uint32_t)(_sapp.win32.surrogate) << 10 | (c - 0xDC00);
    //       c += 0x10000;
    //       _sapp.win32.surrogate = 0;
    //     }
    //     _sapp_init_event(SAPP_EVENTTYPE_CHAR);
    //     _sapp.event.modifiers = _sapp_win32_mods();
    //     _sapp.event.char_code = c;
    //     _sapp.event.key_repeat = repeat;
    //     _sapp_call_event(&_sapp.event);
    //   }
    // }
  } break;
  case WM_KILLFOCUS: {
    os_w32_state.capture_ref_counter = 0;
    ReleaseCapture();
    mem_set(os_app_data.input.buttons.current, 0, sizeof(os_app_data.input.buttons.current));
  } break;
  case WM_DPICHANGED: {
    os_app_data.window.dpi_scale = (f32)(wparam & 0xffff) / 96.0f;
    RECT *suggested_new_rect = (RECT *)lparam;
    SetWindowPos(hwnd, 0,
                 suggested_new_rect->left,
                 suggested_new_rect->top,
                 suggested_new_rect->right - suggested_new_rect->left,
                 suggested_new_rect->bottom - suggested_new_rect->top,
                 0);
  } break;
  // case WM_DROPFILES:
  case WM_SYSCOMMAND: {
    switch (wparam & 0xFFF0) {
    case SC_KEYMENU: {
      return 0;
    } break;
    }
  } break;
  case WM_ENTERSIZEMOVE: {
    SetTimer(hwnd, 1, USER_TIMER_MINIMUM, 0);
  } break;
  case WM_EXITSIZEMOVE: {
    KillTimer(hwnd, 1);
  } break;

  case WM_TIMER: {
    if (wparam == 1) {
      os_loop_systems();
    }
  } break;
  case WM_SIZE: {
    os_app_data.window.logical_size.x = LOWORD(lparam);
    os_app_data.window.logical_size.y = HIWORD(lparam);
  } break;
  case WM_MOVE: {
    os_app_data.window.logical_position.x = LOWORD(lparam);
    os_app_data.window.logical_position.y = HIWORD(lparam);
  } break;
  case WM_ERASEBKGND: {
    return 1;
  } break;
  case WM_NCLBUTTONDOWN:
    /* workaround for half-second pause when starting to move window
        see: https://gamedev.net/forums/topic/672094-keeping-things-moving-during-win32-moveresize-events/5254386/
    */
    if (SendMessage(hwnd, WM_NCHITTEST, wparam, lparam) == HTCAPTION) {
      POINT point = { 0, 0 };
      if (GetCursorPos(&point)) {
        ScreenToClient(hwnd, &point);
        PostMessage(hwnd, WM_MOUSEMOVE, 0, ((u32)point.x) | (((u32)point.y) << 16));
      }
    }
    break;
  //  // case WM_NCPAINT:
  // //   case WM_NCUAHDRAWCAPTION:
  // //   case WM_NCUAHDRAWFRAME:
  // // case WM_SETICON:
  // // case WM_SETTEXT:
  // // case WM_NCACTIVATE:
  // // case WM_NCCALCSIZE:
  // // case WM_NCHITTEST:
  default:
    break;
  }
  return DefWindowProcW(hwnd, umsg, wparam, lparam);
}

function void
os_w32_init_window(Scratch *scratch) {
  ScratchMarker m = scratch_begin(0);
  os_w32_state.hinstance = GetModuleHandleW(0);

  WNDCLASSW wndclassw = { 0 };
  wndclassw.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wndclassw.lpfnWndProc = (WNDPROC)os_w32_wndproc;
  wndclassw.hInstance = os_w32_state.hinstance;
  wndclassw.hCursor = os_w32_state.cursors[OS_CURSOR_POINTER];
  wndclassw.hIcon = LoadIcon(0, IDI_WINLOGO);
  wndclassw.lpszClassName = L"window-class";
  RegisterClassW(&wndclassw);

  // TODO: research about custom border

  /* NOTE: regardless whether fullscreen is requested or not, a regular
   windowed-mode window will always be created first (however in hidden
   mode, so that no windowed-mode window pops up before the fullscreen window)
  */

  /*
   NOTE: The size pass into CreateWindowExW is not only content area but title bar and border
   aswell so we need AdjustWindowRectEx to get the desired size.
  */

  DWORD win_ex_style = WS_EX_APPWINDOW
                     | WS_EX_ACCEPTFILES;
  DWORD win_style = WS_OVERLAPPEDWINDOW
                  | WS_SIZEBOX;

  Str16 title = str16_from_str8(m.scratch, os_app_init_setting.window_title);

  os_w32_state.hwnd = CreateWindowExW(
    win_ex_style,                                                                    // dwExStyle
    L"window-class",                                                                 // lpClassName
    title.value,                                                                     // lpWindowName
    win_style,                                                                       // dwStyle
    os_app_init_setting.position.x ? os_app_init_setting.position.x : CW_USEDEFAULT, // X
    os_app_init_setting.position.y ? os_app_init_setting.position.y : CW_USEDEFAULT, // Y
    os_app_init_setting.size.x ? os_app_init_setting.size.x : CW_USEDEFAULT,         // nWidth
    os_app_init_setting.size.y ? os_app_init_setting.size.y : CW_USEDEFAULT,         // nHeight (NOTE: if width is CW_USEDEFAULT, height is actually ignored)
    NULL,                                                                            // hWndParent
    NULL,                                                                            // hMenu
    os_w32_state.hinstance,                                                          // hInstance
    NULL);                                                                           // lParam

  assert(os_w32_state.hwnd);

  ShowWindow(os_w32_state.hwnd, SW_SHOW);
  scratch_end(m);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
  ScratchMarker m = scratch_begin(0);
  // init
  os_w32_init_time();
  os_w32_init_console();
  os_w32_init_system_info();
  os_w32_init_process_info(m.scratch);
  os_w32_init_args(m.scratch);
  // init gfx stuff
  os_w32_init_dpi();
  os_w32_init_cursors();
  // os_w32_init_icons(m.scratch);
  os_w32_init_window(m.scratch);
  os_w32_init_vulkan();

  // base entry
  os_init_systems();
  while (!os_app_data.should_close) {
    // reset before handling events
    mem_move(os_app_data.input.buttons.previous, os_app_data.input.buttons.current, sizeof(os_app_data.input.buttons.current));
    os_app_data.input.cursor.previous_position.x = os_app_data.input.cursor.position.x;
    os_app_data.input.cursor.previous_position.y = os_app_data.input.cursor.position.y;
    os_app_data.input.cursor.delta_scroll.x = 0;
    os_app_data.input.cursor.delta_scroll.y = 0;

    // handling events
    MSG msg;
    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (WM_QUIT == msg.message) {
        os_app_data.should_close = true;
      } else {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
      }
    }

    // calculations after events
    os_app_data.input.cursor.delta_position.x = os_app_data.input.cursor.position.x - os_app_data.input.cursor.previous_position.x;
    os_app_data.input.cursor.delta_position.y = os_app_data.input.cursor.position.y - os_app_data.input.cursor.previous_position.y;

    // actual stuff
    os_loop_systems();
  }
  os_cleanup_systems();

  os_w32_restore_console();
  scratch_end(m);
  return 0;
}
