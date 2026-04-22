#ifndef PGL_APP_H
#define PGL_APP_H

#include <pgl/defines.h>
#include <pgl/math.h>
#include <pgl/string.h>

typedef enum KeyboardButton {
  KEY_UNKNOWN,
  KEY_RETURN,
  KEY_ESCAPE,
  KEY_BACKSPACE,
  KEY_TAB,
  KEY_SPACE,
  KEY_EXCLAIM,
  KEY_DBLAPOSTROPHE,
  KEY_HASH,
  KEY_DOLLAR,
  KEY_PERCENT,
  KEY_AMPERSAND,
  KEY_APOSTROPHE,
  KEY_LEFTPAREN,
  KEY_RIGHTPAREN,
  KEY_ASTERISK,
  KEY_PLUS,
  KEY_COMMA,
  KEY_MINUS,
  KEY_PERIOD,
  KEY_SLASH,
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,
  KEY_COLON,
  KEY_SEMICOLON,
  KEY_LESS,
  KEY_EQUALS,
  KEY_GREATER,
  KEY_QUESTION,
  KEY_AT,
  KEY_LEFTBRACKET,
  KEY_BACKSLASH,
  KEY_RIGHTBRACKET,
  KEY_CARET,
  KEY_UNDERSCORE,
  KEY_GRAVE,
  KEY_A,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_G,
  KEY_H,
  KEY_I,
  KEY_J,
  KEY_K,
  KEY_L,
  KEY_M,
  KEY_N,
  KEY_O,
  KEY_P,
  KEY_Q,
  KEY_R,
  KEY_S,
  KEY_T,
  KEY_U,
  KEY_V,
  KEY_W,
  KEY_X,
  KEY_Y,
  KEY_Z,
  KEY_LEFTBRACE,
  KEY_PIPE,
  KEY_RIGHTBRACE,
  KEY_TILDE,
  KEY_DELETE,
  KEY_PLUSMINUS,
  KEY_CAPSLOCK,
  KEY_F1,
  KEY_F2,
  KEY_F3,
  KEY_F4,
  KEY_F5,
  KEY_F6,
  KEY_F7,
  KEY_F8,
  KEY_F9,
  KEY_F10,
  KEY_F11,
  KEY_F12,
  KEY_PRINTSCREEN,
  KEY_SCROLLLOCK,
  KEY_PAUSE,
  KEY_INSERT,
  KEY_HOME,
  KEY_PAGEUP,
  KEY_END,
  KEY_PAGEDOWN,
  KEY_RIGHT,
  KEY_LEFT,
  KEY_DOWN,
  KEY_UP,
  KEY_NUMLOCKCLEAR,
  KEY_KP_DIVIDE,
  KEY_KP_MULTIPLY,
  KEY_KP_MINUS,
  KEY_KP_PLUS,
  KEY_KP_ENTER,
  KEY_KP_1,
  KEY_KP_2,
  KEY_KP_3,
  KEY_KP_4,
  KEY_KP_5,
  KEY_KP_6,
  KEY_KP_7,
  KEY_KP_8,
  KEY_KP_9,
  KEY_KP_0,
  KEY_KP_PERIOD,
  KEY_APPLICATION,
  KEY_POWER,
  KEY_KP_EQUALS,
  KEY_F13,
  KEY_F14,
  KEY_F15,
  KEY_F16,
  KEY_F17,
  KEY_F18,
  KEY_F19,
  KEY_F20,
  KEY_F21,
  KEY_F22,
  KEY_F23,
  KEY_F24,
  KEY_EXECUTE,
  KEY_HELP,
  KEY_MENU,
  KEY_SELECT,
  KEY_STOP,
  KEY_AGAIN,
  KEY_UNDO,
  KEY_CUT,
  KEY_COPY,
  KEY_PASTE,
  KEY_FIND,
  KEY_MUTE,
  KEY_VOLUMEUP,
  KEY_VOLUMEDOWN,
  KEY_KP_COMMA,
  KEY_KP_EQUALSAS400,
  KEY_ALTERASE,
  KEY_SYSREQ,
  KEY_CANCEL,
  KEY_CLEAR,
  KEY_PRIOR,
  KEY_RETURN2,
  KEY_SEPARATOR,
  KEY_OUT,
  KEY_OPER,
  KEY_CLEARAGAIN,
  KEY_CRSEL,
  KEY_EXSEL,
  KEY_KP_00,
  KEY_KP_000,
  KEY_THOUSANDSSEPARATOR,
  KEY_DECIMALSEPARATOR,
  KEY_CURRENCYUNIT,
  KEY_CURRENCYSUBUNIT,
  KEY_KP_LEFTPAREN,
  KEY_KP_RIGHTPAREN,
  KEY_KP_LEFTBRACE,
  KEY_KP_RIGHTBRACE,
  KEY_KP_TAB,
  KEY_KP_BACKSPACE,
  KEY_KP_A,
  KEY_KP_B,
  KEY_KP_C,
  KEY_KP_D,
  KEY_KP_E,
  KEY_KP_F,
  KEY_KP_XOR,
  KEY_KP_POWER,
  KEY_KP_PERCENT,
  KEY_KP_LESS,
  KEY_KP_GREATER,
  KEY_KP_AMPERSAND,
  KEY_KP_DBLAMPERSAND,
  KEY_KP_VERTICALBAR,
  KEY_KP_DBLVERTICALBAR,
  KEY_KP_COLON,
  KEY_KP_HASH,
  KEY_KP_SPACE,
  KEY_KP_AT,
  KEY_KP_EXCLAM,
  KEY_KP_MEMSTORE,
  KEY_KP_MEMRECALL,
  KEY_KP_MEMCLEAR,
  KEY_KP_MEMADD,
  KEY_KP_MEMSUBTRACT,
  KEY_KP_MEMMULTIPLY,
  KEY_KP_MEMDIVIDE,
  KEY_KP_PLUSMINUS,
  KEY_KP_CLEAR,
  KEY_KP_CLEARENTRY,
  KEY_KP_BINARY,
  KEY_KP_OCTAL,
  KEY_KP_DECIMAL,
  KEY_KP_HEXADECIMAL,
  KEY_LCTRL,
  KEY_LSHIFT,
  KEY_LALT,
  KEY_LGUI,
  KEY_RCTRL,
  KEY_RSHIFT,
  KEY_RALT,
  KEY_RGUI,
  KEY_MODE,
  KEY_SLEEP,
  KEY_WAKE,
  KEY_CHANNEL_INCREMENT,
  KEY_CHANNEL_DECREMENT,
  KEY_MEDIA_PLAY,
  KEY_MEDIA_PAUSE,
  KEY_MEDIA_RECORD,
  KEY_MEDIA_FAST_FORWARD,
  KEY_MEDIA_REWIND,
  KEY_MEDIA_NEXT_TRACK,
  KEY_MEDIA_PREVIOUS_TRACK,
  KEY_MEDIA_STOP,
  KEY_MEDIA_EJECT,
  KEY_MEDIA_PLAY_PAUSE,
  KEY_MEDIA_SELECT,
  KEY_AC_NEW,
  KEY_AC_OPEN,
  KEY_AC_CLOSE,
  KEY_AC_EXIT,
  KEY_AC_SAVE,
  KEY_AC_PRINT,
  KEY_AC_PROPERTIES,
  KEY_AC_SEARCH,
  KEY_AC_HOME,
  KEY_AC_BACK,
  KEY_AC_FORWARD,
  KEY_AC_STOP,
  KEY_AC_REFRESH,
  KEY_AC_BOOKMARKS,
  KEY_SOFTLEFT,
  KEY_SOFTRIGHT,
  KEY_CALL,
  KEY_ENDCALL,
  KEY_LEFT_TAB,
  KEY_LEVEL5_SHIFT,
  KEY_MULTI_KEY_COMPOSE,
  KEY_LMETA,
  KEY_RMETA,
  KEY_LHYPER,
  KEY_RHYPER
} KeyboardButton;

typedef enum MouseButton {
  MOUSE_LEFT,
  MOUSE_RIGHT,
  MOUSE_MIDDLE,
  MOUSE_X1,
  MOUSE_X2
} MouseButton;

typedef struct AppData {
  struct Callback {
    void (*init)();
    void (*tick)();
    void (*frame)();
  } callback;
  b8      should_close;
  StrView base_path;

  struct Window {
    StrView title;
    // flag ?
    Vec2I32 size;          // pixel    // for gpu fbo
    f32     pixel_density; // (sizePixel / sizeLogical) used to convert input space and if you want same ui size on many screen do size * this
    f32     display_scale; // for ui scale
  } window;
  struct Input {
    b8 any_button;
    struct Keyboard {
      b8 buttons[256];
      b8 previous_buttons[256];
    } keyboard;
    struct Mouse {
      b8      buttons[5];
      b8      previous_buttons[5];
      Vec2I32 position;       // pixel
      Vec2I32 delta_position; // pixel
      Vec2I32 delta_scroll;   // pixel
    } mouse;
  } input;
  struct Tick {
    f64 delta_time; // for physic
    f64 time;       // for physic
    f64 time_scale; // [0,inf] 0 stop physic, 1 is normal
  } tick;
  struct Frame {
    f64 delta_time; // for non deterministic stuff like camera...
    f64 time;
    f64 previous_time; // internal
    f64 accumulator;
    f64 interpolant; // [0,1]
  } frame;
} AppData;

void    app_launch(CStr window_title,
                   i32  width,
                   i32  height,
                   void (*init)(),
                   void (*tick)(),
                   void (*frame)());
f64     app_get_time();
StrView app_get_base_path();

StrView window_get_title();
Vec2I32 window_get_size();
f32     window_get_pixel_density();
f32     window_get_display_scale();

b8      input_is_key_down(KeyboardButton btn);
b8      input_is_key_up(KeyboardButton btn);
b8      input_is_key_pressed(KeyboardButton btn);
b8      input_is_key_released(KeyboardButton btn);
b8      input_is_mouse_down(MouseButton btn);
b8      input_is_mouse_up(MouseButton btn);
b8      input_is_mouse_pressed(MouseButton btn);
b8      input_is_mouse_released(MouseButton btn);
Vec2I32 input_get_mouse_position();
Vec2I32 input_get_mouse_delta_position();
Vec2I32 input_get_mouse_delta_scroll();

f64 tick_get_delta_time();
f64 tick_get_time();
f64 tick_get_time_scale();

f64 frame_get_delta_time();
f64 frame_get_time();
f64 frame_get_interpolant();

#endif // PGL_APP_H
