// Modern Windows SDK Functions
// (We must dynamically link to them, since they can be missing in older SDKs)

#define w32_DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((void*)-4)

typedef BOOL    w32_SetProcessDpiAwarenessContext_Type(void *value);
typedef UINT    w32_GetDpiForWindow_Type(HWND hwnd);
typedef HRESULT w32_GetDpiForMonitor_Type(HMONITOR hmonitor, MONITOR_DPI_TYPE dpiType, UINT *dpiX, UINT *dpiY);
typedef int     w32_GetSystemMetricsForDpi_Type(int nIndex, UINT dpi);

global w32_GetDpiForWindow_Type               *w32_GetDpiForWindow_func = 0;
global w32_GetDpiForMonitor_Type              *w32_GetDpiForMonitor_func = 0;
global w32_GetSystemMetricsForDpi_Type        *w32_GetSystemMetricsForDpi_func = 0;
global w32_SetProcessDpiAwarenessContext_Type *SetProcessDpiAwarenessContext_func = 0;

function void
os_set_cursor(OS_Cursor cursor) {
  b8      valid_cursor = 1;
  HCURSOR hcursor = 0;
  switch (cursor) {
  default: {
    valid_cursor = 0;
  } break;
#define X(E, R)                                  \
  case OS_CURSOR_##E:{                           \
  local_persist HCURSOR curs = 0;                \
  if (curs == 0){ curs = LoadCursor(NULL, R); }  \
  hcursor = curs;                                \
  }break;
    OS_WIN32_CURSOR_MAP_TABLE
#undef X
  }
  if (valid_cursor && !os_w32_gfx_state.resizing) {
    if (hcursor != os_w32_gfx_state.hCursor) {
      PostMessage(0, WM_SETCURSOR, 0, 0);
      POINT p = { 0 };
      GetCursorPos(&p);
      SetCursorPos(p.x, p.y);
    }
    os_w32_gfx_state.hCursor = hcursor;
  }
}

function void os_gfx_init() {
  os_w32_gfx_state.thread_tid = (u32)GetCurrentThreadId();
  os_w32_gfx_state.hInstance = GetModuleHandle(0);

  HMODULE module = LoadLibraryA("user32.dll");
  if (module != 0) {
    SetProcessDpiAwarenessContext_func = (w32_SetProcessDpiAwarenessContext_Type *)GetProcAddress(module, "SetProcessDpiAwarenessContext");
    w32_GetDpiForWindow_func = (w32_GetDpiForWindow_Type *)GetProcAddress(module, "GetDpiForWindow");
    w32_GetDpiForMonitor_func = (w32_GetDpiForMonitor_Type *)GetProcAddress(module, "GetDpiForMonitor");
    w32_GetSystemMetricsForDpi_func = (w32_GetSystemMetricsForDpi_Type *)GetProcAddress(module, "GetSystemMetricsForDpi");
    FreeLibrary(module);
  }
  if (SetProcessDpiAwarenessContext_func != 0) {
    // Windows 10, version 1703
    SetProcessDpiAwarenessContext_func(w32_DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
  } else {
    HMODULE shcore = LoadLibraryA("shcore.dll");
    if (shcore) {
      typedef HRESULT(WINAPI * SetProcessDpiAwareness_t)(int);
      SetProcessDpiAwareness_t SetProcessDpiAwareness = (void *)GetProcAddress(shcore, "SetProcessDpiAwareness");
      if (SetProcessDpiAwareness) {
        // Windows 8.1
        SetProcessDpiAwareness(2);
      }
      FreeLibrary(shcore);
    }
    // Windows Vista
    SetProcessDPIAware();
  }

  // register graphical-window class
  {
    WNDCLASSEXW wndclass = { size_of(wndclass) };
    // wndclass.lpfnWndProc = os_w32_wnd_proc;
    wndclass.hInstance = os_w32_gfx_state.hInstance;
    wndclass.lpszClassName = L"graphical-window";
    wndclass.hCursor = LoadCursorA(0, IDC_ARROW);
    wndclass.hIcon = LoadIcon(os_w32_gfx_state.hInstance, MAKEINTRESOURCE(1));
    wndclass.style = CS_VREDRAW | CS_HREDRAW;
    ATOM wndatom = RegisterClassExW(&wndclass);
    (void)wndatom;
  }

  // grab graphics system info
  {
    os_w32_gfx_state.gfx_info.default_refresh_rate = 60.0f;
    DEVMODEW devmodew = { 0 };
    if (EnumDisplaySettingsW(0, ENUM_CURRENT_SETTINGS, &devmodew)) {
      os_w32_gfx_state.gfx_info.default_refresh_rate = (f32)devmodew.dmDisplayFrequency;
    }
  }

  os_set_cursor(OS_CURSOR_POINTER);
}
