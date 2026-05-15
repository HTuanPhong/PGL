#ifndef OS_WIN32_H
#define OS_WIN32_H

#define NOMINMAX
#undef function // window have a function word somewhere in there >: (
#include <dwmapi.h>
#include <shellscalingapi.h>
#include <windows.h>
#include <windowsx.h>
#pragma comment(linker, "/subsystem:windows")
#pragma comment(lib, "dwmapi")
#define function static

typedef struct OS_W32_State {
  UINT          original_codepage;
  LARGE_INTEGER freq;
  LARGE_INTEGER start;
  HCURSOR       cursors[OS_CURSOR_COUNT];
  HWND          hwnd;
  HINSTANCE     hinstance;
  RECT          stored_window_rect; // used to restore window pos/size when toggling fullscreen => windowed
  i32           capture_ref_counter;
  // mouse_scale = 1
  // content_scale = window_scale
} OS_W32_State;

global OS_W32_State os_w32_state = { 0 };

/*  OS_CURSOR_        Win32         */
#define OS_WIN32_CURSOR_MAP_TABLE    \
  X(POINTER          ,IDC_ARROW     )\
  X(TEXT             ,IDC_IBEAM     )\
  X(EW               ,IDC_SIZEWE    )\
  X(NS               ,IDC_SIZENS    )\
  X(NWSE             ,IDC_SIZENWSE  )\
  X(NESW             ,IDC_SIZENESW  )\
  X(NSEW             ,IDC_SIZEALL   )\
  X(HANDPOINT        ,IDC_HAND      )\
  X(CROSSHAIR        ,IDC_CROSS     )\
  X(DISABLED         ,IDC_NO        )

/*  OS_BUTTON_            Win32        */
#define OS_WIN32_BUTTON_MAP_TABLE       \
  X(ESC              ,VK_ESCAPE     )\
  X(F1               ,VK_F1         )\
  X(F2               ,VK_F2         )\
  X(F3               ,VK_F3         )\
  X(F4               ,VK_F4         )\
  X(F5               ,VK_F5         )\
  X(F6               ,VK_F6         )\
  X(F7               ,VK_F7         )\
  X(F8               ,VK_F8         )\
  X(F9               ,VK_F9         )\
  X(F10              ,VK_F10        )\
  X(F11              ,VK_F11        )\
  X(F12              ,VK_F12        )\
  X(F13              ,VK_F13        )\
  X(F14              ,VK_F14        )\
  X(F15              ,VK_F15        )\
  X(F16              ,VK_F16        )\
  X(F17              ,VK_F17        )\
  X(F18              ,VK_F18        )\
  X(F19              ,VK_F19        )\
  X(F20              ,VK_F20        )\
  X(F21              ,VK_F21        )\
  X(F22              ,VK_F22        )\
  X(F23              ,VK_F23        )\
  X(F24              ,VK_F24        )\
  X(TICK             ,VK_OEM_3      )\
  X(0                ,'0'           )\
  X(1                ,'1'           )\
  X(2                ,'2'           )\
  X(3                ,'3'           )\
  X(4                ,'4'           )\
  X(5                ,'5'           )\
  X(6                ,'6'           )\
  X(7                ,'7'           )\
  X(8                ,'8'           )\
  X(9                ,'9'           )\
  X(MINUS            ,VK_OEM_MINUS  )\
  X(EQUAL            ,VK_OEM_PLUS   )\
  X(BACKSPACE        ,VK_BACK       )\
  X(TAB              ,VK_TAB        )\
  X(Q                ,'Q'           )\
  X(W                ,'W'           )\
  X(E                ,'E'           )\
  X(R                ,'R'           )\
  X(T                ,'T'           )\
  X(Y                ,'Y'           )\
  X(U                ,'U'           )\
  X(I                ,'I'           )\
  X(O                ,'O'           )\
  X(P                ,'P'           )\
  X(LEFT_BRACKET     ,VK_OEM_4      )\
  X(RIGHT_BRACKET    ,VK_OEM_6      )\
  X(BACK_SLASH       ,VK_OEM_5      )\
  X(CAPS_LOCK        ,VK_CAPITAL    )\
  X(A                ,'A'           )\
  X(S                ,'S'           )\
  X(D                ,'D'           )\
  X(F                ,'F'           )\
  X(G                ,'G'           )\
  X(H                ,'H'           )\
  X(J                ,'J'           )\
  X(K                ,'K'           )\
  X(L                ,'L'           )\
  X(SEMICOLON        ,VK_OEM_1      )\
  X(QUOTE            ,VK_OEM_7      )\
  X(ENTER            ,VK_RETURN     )\
  X(SHIFT            ,VK_SHIFT      )\
  X(Z                ,'Z'           )\
  X(X                ,'X'           )\
  X(C                ,'C'           )\
  X(V                ,'V'           )\
  X(B                ,'B'           )\
  X(N                ,'N'           )\
  X(M                ,'M'           )\
  X(COMMA            ,VK_OEM_COMMA  )\
  X(PERIOD           ,VK_OEM_PERIOD )\
  X(SLASH            ,VK_OEM_2      )\
  X(CTRL             ,VK_CONTROL    )\
  X(CTRL             ,VK_LCONTROL   )\
  X(CTRL             ,VK_RCONTROL   )\
  X(ALT              ,VK_MENU       )\
  X(ALT              ,VK_LMENU      )\
  X(ALT              ,VK_RMENU      )\
  X(SPACE            ,VK_SPACE      )\
  X(MENU             ,VK_APPS       )\
  X(SCROLL_LOCK      ,VK_SCROLL     )\
  X(PAUSE            ,VK_PAUSE      )\
  X(INSERT           ,VK_INSERT     )\
  X(HOME             ,VK_HOME       )\
  X(PAGE_UP          ,VK_PRIOR      )\
  X(DELETE           ,VK_DELETE     )\
  X(END              ,VK_END        )\
  X(PAGE_DOWN        ,VK_NEXT       )\
  X(UP               ,VK_UP         )\
  X(LEFT             ,VK_LEFT       )\
  X(DOWN             ,VK_DOWN       )\
  X(RIGHT            ,VK_RIGHT      )\
  X(NUMPAD_LOCK      ,VK_NUMLOCK    )\
  X(NUMPAD_SLASH     ,VK_DIVIDE     )\
  X(NUMPAD_STAR      ,VK_MULTIPLY   )\
  X(NUMPAD_MINUS     ,VK_SUBTRACT   )\
  X(NUMPAD_PLUS      ,VK_ADD        )\
  X(NUMPAD_PERIOD    ,VK_DECIMAL    )\
  X(NUMPAD_0         ,VK_NUMPAD0    )\
  X(NUMPAD_1         ,VK_NUMPAD1    )\
  X(NUMPAD_2         ,VK_NUMPAD2    )\
  X(NUMPAD_3         ,VK_NUMPAD3    )\
  X(NUMPAD_4         ,VK_NUMPAD4    )\
  X(NUMPAD_5         ,VK_NUMPAD5    )\
  X(NUMPAD_6         ,VK_NUMPAD6    )\
  X(NUMPAD_7         ,VK_NUMPAD7    )\
  X(NUMPAD_8         ,VK_NUMPAD8    )\
  X(NUMPAD_9         ,VK_NUMPAD9    )\
  X(LEFT_MOUSE       ,VK_LBUTTON    )\
  X(RIGHT_MOUSE      ,VK_RBUTTON    )\
  X(MIDDLE_MOUSE     ,VK_MBUTTON    )\
  X(X1_MOUSE         ,VK_XBUTTON1   )\
  X(X2_MOUSE         ,VK_XBUTTON2   )

global OS_Button os_w32_button_table[256] = {
#define X(os_button, vk) [vk] = OS_BUTTON_##os_button,
  OS_WIN32_BUTTON_MAP_TABLE
#undef X
};

#endif
