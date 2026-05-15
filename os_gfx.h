#ifndef OS_GFX_H
#define OS_GFX_H

typedef struct OS_GfxInfo {
  f32 default_refresh_rate;
} OS_GfxInfo;

typedef enum OS_Cursor {
  OS_CURSOR_POINTER,
  OS_CURSOR_TEXT,
  OS_CURSOR_EW,
  OS_CURSOR_NS,
  OS_CURSOR_NWSE,
  OS_CURSOR_NESW,
  OS_CURSOR_NSEW,
  OS_CURSOR_HANDPOINT,
  OS_CURSOR_CROSSHAIR,
  OS_CURSOR_DISABLED,
  OS_CURSOR_COUNT
} OS_Cursor;

/*  OS_BUTTON_         Display string         Config string      */
#define OS_BUTTON_TABLE                                           \
  X(UNKNOWN           ,"Invalid Key"         ,"unknown"          )\
  X(ESC               ,"Escape"              ,"esc"              )\
  X(F1                ,"F1"                  ,"f1"               )\
  X(F2                ,"F2"                  ,"f2"               )\
  X(F3                ,"F3"                  ,"f3"               )\
  X(F4                ,"F4"                  ,"f4"               )\
  X(F5                ,"F5"                  ,"f5"               )\
  X(F6                ,"F6"                  ,"f6"               )\
  X(F7                ,"F7"                  ,"f7"               )\
  X(F8                ,"F8"                  ,"f8"               )\
  X(F9                ,"F9"                  ,"f9"               )\
  X(F10               ,"F10"                 ,"f10"              )\
  X(F11               ,"F11"                 ,"f11"              )\
  X(F12               ,"F12"                 ,"f12"              )\
  X(F13               ,"F13"                 ,"f13"              )\
  X(F14               ,"F14"                 ,"f14"              )\
  X(F15               ,"F15"                 ,"f15"              )\
  X(F16               ,"F16"                 ,"f16"              )\
  X(F17               ,"F17"                 ,"f17"              )\
  X(F18               ,"F18"                 ,"f18"              )\
  X(F19               ,"F19"                 ,"f19"              )\
  X(F20               ,"F20"                 ,"f20"              )\
  X(F21               ,"F21"                 ,"f21"              )\
  X(F22               ,"F22"                 ,"f22"              )\
  X(F23               ,"F23"                 ,"f23"              )\
  X(F24               ,"F24"                 ,"f24"              )\
  X(TICK              ,"Tick"                ,"tick"             )\
  X(0                 ,"0"                   ,"0"                )\
  X(1                 ,"1"                   ,"1"                )\
  X(2                 ,"2"                   ,"2"                )\
  X(3                 ,"3"                   ,"3"                )\
  X(4                 ,"4"                   ,"4"                )\
  X(5                 ,"5"                   ,"5"                )\
  X(6                 ,"6"                   ,"6"                )\
  X(7                 ,"7"                   ,"7"                )\
  X(8                 ,"8"                   ,"8"                )\
  X(9                 ,"9"                   ,"9"                )\
  X(MINUS             ,"Minus"               ,"minus"            )\
  X(EQUAL             ,"Equal"               ,"equal"            )\
  X(BACKSPACE         ,"Backspace"           ,"backspace"        )\
  X(TAB               ,"Tab"                 ,"tab"              )\
  X(Q                 ,"Q"                   ,"q"                )\
  X(W                 ,"W"                   ,"w"                )\
  X(E                 ,"E"                   ,"e"                )\
  X(R                 ,"R"                   ,"r"                )\
  X(T                 ,"T"                   ,"t"                )\
  X(Y                 ,"Y"                   ,"y"                )\
  X(U                 ,"U"                   ,"u"                )\
  X(I                 ,"I"                   ,"i"                )\
  X(O                 ,"O"                   ,"o"                )\
  X(P                 ,"P"                   ,"p"                )\
  X(LEFT_BRACKET      ,"Left Bracket"        ,"left_bracket"     )\
  X(RIGHT_BRACKET     ,"Right Bracket"       ,"right_bracket"    )\
  X(BACK_SLASH        ,"Back Slash"          ,"backslash"        )\
  X(CAPS_LOCK         ,"Caps Lock"           ,"caps_lock"        )\
  X(A                 ,"A"                   ,"a"                )\
  X(S                 ,"S"                   ,"s"                )\
  X(D                 ,"D"                   ,"d"                )\
  X(F                 ,"F"                   ,"f"                )\
  X(G                 ,"G"                   ,"g"                )\
  X(H                 ,"H"                   ,"h"                )\
  X(J                 ,"J"                   ,"j"                )\
  X(K                 ,"K"                   ,"k"                )\
  X(L                 ,"L"                   ,"l"                )\
  X(SEMICOLON         ,"Semicolon"           ,"semicolon"        )\
  X(QUOTE             ,"Quote"               ,"quote"            )\
  X(ENTER             ,"Enter"               ,"enter"            )\
  X(SHIFT             ,"Shift"               ,"shift"            )\
  X(Z                 ,"Z"                   ,"z"                )\
  X(X                 ,"X"                   ,"x"                )\
  X(C                 ,"C"                   ,"c"                )\
  X(V                 ,"V"                   ,"v"                )\
  X(B                 ,"B"                   ,"b"                )\
  X(N                 ,"N"                   ,"n"                )\
  X(M                 ,"M"                   ,"m"                )\
  X(COMMA             ,"Comma"               ,"comma"            )\
  X(PERIOD            ,"Period"              ,"period"           )\
  X(SLASH             ,"Slash"               ,"slash"            )\
  X(CTRL              ,"Ctrl"                ,"ctrl"             )\
  X(ALT               ,"Alt"                 ,"alt"              )\
  X(SPACE             ,"Space"               ,"space"            )\
  X(MENU              ,"Menu"                ,"menu"             )\
  X(SCROLL_LOCK       ,"Scroll Lock"         ,"scroll_lock"      )\
  X(PAUSE             ,"Pause"               ,"pause"            )\
  X(INSERT            ,"Insert"              ,"insert"           )\
  X(HOME              ,"Home"                ,"home"             )\
  X(PAGE_UP           ,"Page Up"             ,"page_up"          )\
  X(DELETE            ,"Delete"              ,"delete"           )\
  X(END               ,"End"                 ,"end"              )\
  X(PAGE_DOWN         ,"Page Down"           ,"page_down"        )\
  X(UP                ,"Up"                  ,"up"               )\
  X(LEFT              ,"Left"                ,"left"             )\
  X(DOWN              ,"Down"                ,"down"             )\
  X(RIGHT             ,"Right"               ,"right"            )\
  X(NUMPAD_LOCK       ,"Num Lock"            ,"num_lock"         )\
  X(NUMPAD_SLASH      ,"Numpad Slash"        ,"numpad_slash"     )\
  X(NUMPAD_STAR       ,"Numpad Star"         ,"numpad_star"      )\
  X(NUMPAD_MINUS      ,"Numpad Minus"        ,"numpad_minus"     )\
  X(NUMPAD_PLUS       ,"Numpad Plus"         ,"numpad_plus"      )\
  X(NUMPAD_PERIOD     ,"Numpad Period"       ,"numpad_period"    )\
  X(NUMPAD_0          ,"Numpad 0"            ,"numpad_0"         )\
  X(NUMPAD_1          ,"Numpad 1"            ,"numpad_1"         )\
  X(NUMPAD_2          ,"Numpad 2"            ,"numpad_2"         )\
  X(NUMPAD_3          ,"Numpad 3"            ,"numpad_3"         )\
  X(NUMPAD_4          ,"Numpad 4"            ,"numpad_4"         )\
  X(NUMPAD_5          ,"Numpad 5"            ,"numpad_5"         )\
  X(NUMPAD_6          ,"Numpad 6"            ,"numpad_6"         )\
  X(NUMPAD_7          ,"Numpad 7"            ,"numpad_7"         )\
  X(NUMPAD_8          ,"Numpad 8"            ,"numpad_8"         )\
  X(NUMPAD_9          ,"Numpad 9"            ,"numpad_9"         )\
  X(LEFT_MOUSE        ,"Left Mouse"          ,"left_mouse"       )\
  X(RIGHT_MOUSE       ,"Right Mouse"         ,"right_mouse"      )\
  X(MIDDLE_MOUSE      ,"Middle Mouse"        ,"middle_mouse"     )\
  X(X1_MOUSE          ,"X1 Mouse"            ,"x1_mouse"         )\
  X(X2_MOUSE          ,"X2 Mouse"            ,"x2_mouse"         )

typedef enum OS_Button {
#define X(enum, display, config) OS_BUTTON_##enum,
  OS_BUTTON_TABLE
#undef X
    OS_BUTTON_COUNT
} OS_Button;

global Str8 os_button_display_string_table[] = {
#define X(enum, display, config) str8_from_lit(display),
  OS_BUTTON_TABLE
#undef X
};

global Str8 os_button_config_string_table[] = {
#define X(enum, display, config) str8_from_lit(config),
  OS_BUTTON_TABLE
#undef X
};

// Main init

function void os_gfx_init(); // (Implemented Per-OS)

// info

function OS_GfxInfo *os_get_gfx_info(); // (Implemented Per-OS)

// clipboard

function void os_set_clipboard_text(Str8 string); // (Implemented Per-OS)
function Str8 os_get_clipboard_text(Scratch *scratch); // (Implemented Per-OS)

// cursor

function void os_set_cursor(OS_Cursor cursor); // (Implemented Per-OS)

// native graphical ui

function void os_graphical_message(b8 error, Str8 title, Str8 message); // (Implemented Per-OS)
function Str8 os_graphical_pick_file(Scratch *scratch, Str8 initial_path); // (Implemented Per-OS)

// web

function void os_open_in_browser(Str8 url); // (Implemented Per-OS)

#endif
