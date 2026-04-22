// windowing, timing, input
#include "graphic.h"
#include <pgl/app.h>
#include <pgl/defines.h>
#include <pgl/graphic.h>
#include <pgl/log.h>

//
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#if PLATFORM_EMSCRIPTEN
#  include <emscripten.h>
#endif

typedef struct SDLdata {
  SDL_Window   *window;
  SDL_GLContext glContext;
  SDL_Cursor   *cursor;
} SDLdata;

static SDLdata sdl = { 0 };

static AppData app = { 0 };

#define SDL_CHECK(x)                        \
  do {                                      \
    if (!(x)) {                             \
      log_error("SDL: %s", SDL_GetError()); \
      exit(1);                              \
    }                                       \
  } while (0)

static MouseButton s_map_mouse_button(int btn) {
  switch (btn) {
  case SDL_BUTTON_LEFT:   return MOUSE_LEFT;
  case SDL_BUTTON_RIGHT:  return MOUSE_RIGHT;
  case SDL_BUTTON_MIDDLE: return MOUSE_MIDDLE;
  case SDL_BUTTON_X1:     return MOUSE_X1;
  case SDL_BUTTON_X2:     return MOUSE_X2;
  }
  return 0;
}

static KeyboardButton s_map_key(SDL_Keycode key) {
  switch (key) {
  case SDLK_UNKNOWN:              return KEY_UNKNOWN;
  case SDLK_RETURN:               return KEY_RETURN;
  case SDLK_ESCAPE:               return KEY_ESCAPE;
  case SDLK_BACKSPACE:            return KEY_BACKSPACE;
  case SDLK_TAB:                  return KEY_TAB;
  case SDLK_SPACE:                return KEY_SPACE;
  case SDLK_EXCLAIM:              return KEY_EXCLAIM;
  case SDLK_DBLAPOSTROPHE:        return KEY_DBLAPOSTROPHE;
  case SDLK_HASH:                 return KEY_HASH;
  case SDLK_DOLLAR:               return KEY_DOLLAR;
  case SDLK_PERCENT:              return KEY_PERCENT;
  case SDLK_AMPERSAND:            return KEY_AMPERSAND;
  case SDLK_APOSTROPHE:           return KEY_APOSTROPHE;
  case SDLK_LEFTPAREN:            return KEY_LEFTPAREN;
  case SDLK_RIGHTPAREN:           return KEY_RIGHTPAREN;
  case SDLK_ASTERISK:             return KEY_ASTERISK;
  case SDLK_PLUS:                 return KEY_PLUS;
  case SDLK_COMMA:                return KEY_COMMA;
  case SDLK_MINUS:                return KEY_MINUS;
  case SDLK_PERIOD:               return KEY_PERIOD;
  case SDLK_SLASH:                return KEY_SLASH;
  case SDLK_0:                    return KEY_0;
  case SDLK_1:                    return KEY_1;
  case SDLK_2:                    return KEY_2;
  case SDLK_3:                    return KEY_3;
  case SDLK_4:                    return KEY_4;
  case SDLK_5:                    return KEY_5;
  case SDLK_6:                    return KEY_6;
  case SDLK_7:                    return KEY_7;
  case SDLK_8:                    return KEY_8;
  case SDLK_9:                    return KEY_9;
  case SDLK_COLON:                return KEY_COLON;
  case SDLK_SEMICOLON:            return KEY_SEMICOLON;
  case SDLK_LESS:                 return KEY_LESS;
  case SDLK_EQUALS:               return KEY_EQUALS;
  case SDLK_GREATER:              return KEY_GREATER;
  case SDLK_QUESTION:             return KEY_QUESTION;
  case SDLK_AT:                   return KEY_AT;
  case SDLK_LEFTBRACKET:          return KEY_LEFTBRACKET;
  case SDLK_BACKSLASH:            return KEY_BACKSLASH;
  case SDLK_RIGHTBRACKET:         return KEY_RIGHTBRACKET;
  case SDLK_CARET:                return KEY_CARET;
  case SDLK_UNDERSCORE:           return KEY_UNDERSCORE;
  case SDLK_GRAVE:                return KEY_GRAVE;
  case SDLK_A:                    return KEY_A;
  case SDLK_B:                    return KEY_B;
  case SDLK_C:                    return KEY_C;
  case SDLK_D:                    return KEY_D;
  case SDLK_E:                    return KEY_E;
  case SDLK_F:                    return KEY_F;
  case SDLK_G:                    return KEY_G;
  case SDLK_H:                    return KEY_H;
  case SDLK_I:                    return KEY_I;
  case SDLK_J:                    return KEY_J;
  case SDLK_K:                    return KEY_K;
  case SDLK_L:                    return KEY_L;
  case SDLK_M:                    return KEY_M;
  case SDLK_N:                    return KEY_N;
  case SDLK_O:                    return KEY_O;
  case SDLK_P:                    return KEY_P;
  case SDLK_Q:                    return KEY_Q;
  case SDLK_R:                    return KEY_R;
  case SDLK_S:                    return KEY_S;
  case SDLK_T:                    return KEY_T;
  case SDLK_U:                    return KEY_U;
  case SDLK_V:                    return KEY_V;
  case SDLK_W:                    return KEY_W;
  case SDLK_X:                    return KEY_X;
  case SDLK_Y:                    return KEY_Y;
  case SDLK_Z:                    return KEY_Z;
  case SDLK_LEFTBRACE:            return KEY_LEFTBRACE;
  case SDLK_PIPE:                 return KEY_PIPE;
  case SDLK_RIGHTBRACE:           return KEY_RIGHTBRACE;
  case SDLK_TILDE:                return KEY_TILDE;
  case SDLK_DELETE:               return KEY_DELETE;
  case SDLK_PLUSMINUS:            return KEY_PLUSMINUS;
  case SDLK_CAPSLOCK:             return KEY_CAPSLOCK;
  case SDLK_F1:                   return KEY_F1;
  case SDLK_F2:                   return KEY_F2;
  case SDLK_F3:                   return KEY_F3;
  case SDLK_F4:                   return KEY_F4;
  case SDLK_F5:                   return KEY_F5;
  case SDLK_F6:                   return KEY_F6;
  case SDLK_F7:                   return KEY_F7;
  case SDLK_F8:                   return KEY_F8;
  case SDLK_F9:                   return KEY_F9;
  case SDLK_F10:                  return KEY_F10;
  case SDLK_F11:                  return KEY_F11;
  case SDLK_F12:                  return KEY_F12;
  case SDLK_PRINTSCREEN:          return KEY_PRINTSCREEN;
  case SDLK_SCROLLLOCK:           return KEY_SCROLLLOCK;
  case SDLK_PAUSE:                return KEY_PAUSE;
  case SDLK_INSERT:               return KEY_INSERT;
  case SDLK_HOME:                 return KEY_HOME;
  case SDLK_PAGEUP:               return KEY_PAGEUP;
  case SDLK_END:                  return KEY_END;
  case SDLK_PAGEDOWN:             return KEY_PAGEDOWN;
  case SDLK_RIGHT:                return KEY_RIGHT;
  case SDLK_LEFT:                 return KEY_LEFT;
  case SDLK_DOWN:                 return KEY_DOWN;
  case SDLK_UP:                   return KEY_UP;
  case SDLK_NUMLOCKCLEAR:         return KEY_NUMLOCKCLEAR;
  case SDLK_KP_DIVIDE:            return KEY_KP_DIVIDE;
  case SDLK_KP_MULTIPLY:          return KEY_KP_MULTIPLY;
  case SDLK_KP_MINUS:             return KEY_KP_MINUS;
  case SDLK_KP_PLUS:              return KEY_KP_PLUS;
  case SDLK_KP_ENTER:             return KEY_KP_ENTER;
  case SDLK_KP_1:                 return KEY_KP_1;
  case SDLK_KP_2:                 return KEY_KP_2;
  case SDLK_KP_3:                 return KEY_KP_3;
  case SDLK_KP_4:                 return KEY_KP_4;
  case SDLK_KP_5:                 return KEY_KP_5;
  case SDLK_KP_6:                 return KEY_KP_6;
  case SDLK_KP_7:                 return KEY_KP_7;
  case SDLK_KP_8:                 return KEY_KP_8;
  case SDLK_KP_9:                 return KEY_KP_9;
  case SDLK_KP_0:                 return KEY_KP_0;
  case SDLK_KP_PERIOD:            return KEY_KP_PERIOD;
  case SDLK_APPLICATION:          return KEY_APPLICATION;
  case SDLK_POWER:                return KEY_POWER;
  case SDLK_KP_EQUALS:            return KEY_KP_EQUALS;
  case SDLK_F13:                  return KEY_F13;
  case SDLK_F14:                  return KEY_F14;
  case SDLK_F15:                  return KEY_F15;
  case SDLK_F16:                  return KEY_F16;
  case SDLK_F17:                  return KEY_F17;
  case SDLK_F18:                  return KEY_F18;
  case SDLK_F19:                  return KEY_F19;
  case SDLK_F20:                  return KEY_F20;
  case SDLK_F21:                  return KEY_F21;
  case SDLK_F22:                  return KEY_F22;
  case SDLK_F23:                  return KEY_F23;
  case SDLK_F24:                  return KEY_F24;
  case SDLK_EXECUTE:              return KEY_EXECUTE;
  case SDLK_HELP:                 return KEY_HELP;
  case SDLK_MENU:                 return KEY_MENU;
  case SDLK_SELECT:               return KEY_SELECT;
  case SDLK_STOP:                 return KEY_STOP;
  case SDLK_AGAIN:                return KEY_AGAIN;
  case SDLK_UNDO:                 return KEY_UNDO;
  case SDLK_CUT:                  return KEY_CUT;
  case SDLK_COPY:                 return KEY_COPY;
  case SDLK_PASTE:                return KEY_PASTE;
  case SDLK_FIND:                 return KEY_FIND;
  case SDLK_MUTE:                 return KEY_MUTE;
  case SDLK_VOLUMEUP:             return KEY_VOLUMEUP;
  case SDLK_VOLUMEDOWN:           return KEY_VOLUMEDOWN;
  case SDLK_KP_COMMA:             return KEY_KP_COMMA;
  case SDLK_KP_EQUALSAS400:       return KEY_KP_EQUALSAS400;
  case SDLK_ALTERASE:             return KEY_ALTERASE;
  case SDLK_SYSREQ:               return KEY_SYSREQ;
  case SDLK_CANCEL:               return KEY_CANCEL;
  case SDLK_CLEAR:                return KEY_CLEAR;
  case SDLK_PRIOR:                return KEY_PRIOR;
  case SDLK_RETURN2:              return KEY_RETURN2;
  case SDLK_SEPARATOR:            return KEY_SEPARATOR;
  case SDLK_OUT:                  return KEY_OUT;
  case SDLK_OPER:                 return KEY_OPER;
  case SDLK_CLEARAGAIN:           return KEY_CLEARAGAIN;
  case SDLK_CRSEL:                return KEY_CRSEL;
  case SDLK_EXSEL:                return KEY_EXSEL;
  case SDLK_KP_00:                return KEY_KP_00;
  case SDLK_KP_000:               return KEY_KP_000;
  case SDLK_THOUSANDSSEPARATOR:   return KEY_THOUSANDSSEPARATOR;
  case SDLK_DECIMALSEPARATOR:     return KEY_DECIMALSEPARATOR;
  case SDLK_CURRENCYUNIT:         return KEY_CURRENCYUNIT;
  case SDLK_CURRENCYSUBUNIT:      return KEY_CURRENCYSUBUNIT;
  case SDLK_KP_LEFTPAREN:         return KEY_KP_LEFTPAREN;
  case SDLK_KP_RIGHTPAREN:        return KEY_KP_RIGHTPAREN;
  case SDLK_KP_LEFTBRACE:         return KEY_KP_LEFTBRACE;
  case SDLK_KP_RIGHTBRACE:        return KEY_KP_RIGHTBRACE;
  case SDLK_KP_TAB:               return KEY_KP_TAB;
  case SDLK_KP_BACKSPACE:         return KEY_KP_BACKSPACE;
  case SDLK_KP_A:                 return KEY_KP_A;
  case SDLK_KP_B:                 return KEY_KP_B;
  case SDLK_KP_C:                 return KEY_KP_C;
  case SDLK_KP_D:                 return KEY_KP_D;
  case SDLK_KP_E:                 return KEY_KP_E;
  case SDLK_KP_F:                 return KEY_KP_F;
  case SDLK_KP_XOR:               return KEY_KP_XOR;
  case SDLK_KP_POWER:             return KEY_KP_POWER;
  case SDLK_KP_PERCENT:           return KEY_KP_PERCENT;
  case SDLK_KP_LESS:              return KEY_KP_LESS;
  case SDLK_KP_GREATER:           return KEY_KP_GREATER;
  case SDLK_KP_AMPERSAND:         return KEY_KP_AMPERSAND;
  case SDLK_KP_DBLAMPERSAND:      return KEY_KP_DBLAMPERSAND;
  case SDLK_KP_VERTICALBAR:       return KEY_KP_VERTICALBAR;
  case SDLK_KP_DBLVERTICALBAR:    return KEY_KP_DBLVERTICALBAR;
  case SDLK_KP_COLON:             return KEY_KP_COLON;
  case SDLK_KP_HASH:              return KEY_KP_HASH;
  case SDLK_KP_SPACE:             return KEY_KP_SPACE;
  case SDLK_KP_AT:                return KEY_KP_AT;
  case SDLK_KP_EXCLAM:            return KEY_KP_EXCLAM;
  case SDLK_KP_MEMSTORE:          return KEY_KP_MEMSTORE;
  case SDLK_KP_MEMRECALL:         return KEY_KP_MEMRECALL;
  case SDLK_KP_MEMCLEAR:          return KEY_KP_MEMCLEAR;
  case SDLK_KP_MEMADD:            return KEY_KP_MEMADD;
  case SDLK_KP_MEMSUBTRACT:       return KEY_KP_MEMSUBTRACT;
  case SDLK_KP_MEMMULTIPLY:       return KEY_KP_MEMMULTIPLY;
  case SDLK_KP_MEMDIVIDE:         return KEY_KP_MEMDIVIDE;
  case SDLK_KP_PLUSMINUS:         return KEY_KP_PLUSMINUS;
  case SDLK_KP_CLEAR:             return KEY_KP_CLEAR;
  case SDLK_KP_CLEARENTRY:        return KEY_KP_CLEARENTRY;
  case SDLK_KP_BINARY:            return KEY_KP_BINARY;
  case SDLK_KP_OCTAL:             return KEY_KP_OCTAL;
  case SDLK_KP_DECIMAL:           return KEY_KP_DECIMAL;
  case SDLK_KP_HEXADECIMAL:       return KEY_KP_HEXADECIMAL;
  case SDLK_LCTRL:                return KEY_LCTRL;
  case SDLK_LSHIFT:               return KEY_LSHIFT;
  case SDLK_LALT:                 return KEY_LALT;
  case SDLK_LGUI:                 return KEY_LGUI;
  case SDLK_RCTRL:                return KEY_RCTRL;
  case SDLK_RSHIFT:               return KEY_RSHIFT;
  case SDLK_RALT:                 return KEY_RALT;
  case SDLK_RGUI:                 return KEY_RGUI;
  case SDLK_MODE:                 return KEY_MODE;
  case SDLK_SLEEP:                return KEY_SLEEP;
  case SDLK_WAKE:                 return KEY_WAKE;
  case SDLK_CHANNEL_INCREMENT:    return KEY_CHANNEL_INCREMENT;
  case SDLK_CHANNEL_DECREMENT:    return KEY_CHANNEL_DECREMENT;
  case SDLK_MEDIA_PLAY:           return KEY_MEDIA_PLAY;
  case SDLK_MEDIA_PAUSE:          return KEY_MEDIA_PAUSE;
  case SDLK_MEDIA_RECORD:         return KEY_MEDIA_RECORD;
  case SDLK_MEDIA_FAST_FORWARD:   return KEY_MEDIA_FAST_FORWARD;
  case SDLK_MEDIA_REWIND:         return KEY_MEDIA_REWIND;
  case SDLK_MEDIA_NEXT_TRACK:     return KEY_MEDIA_NEXT_TRACK;
  case SDLK_MEDIA_PREVIOUS_TRACK: return KEY_MEDIA_PREVIOUS_TRACK;
  case SDLK_MEDIA_STOP:           return KEY_MEDIA_STOP;
  case SDLK_MEDIA_EJECT:          return KEY_MEDIA_EJECT;
  case SDLK_MEDIA_PLAY_PAUSE:     return KEY_MEDIA_PLAY_PAUSE;
  case SDLK_MEDIA_SELECT:         return KEY_MEDIA_SELECT;
  case SDLK_AC_NEW:               return KEY_AC_NEW;
  case SDLK_AC_OPEN:              return KEY_AC_OPEN;
  case SDLK_AC_CLOSE:             return KEY_AC_CLOSE;
  case SDLK_AC_EXIT:              return KEY_AC_EXIT;
  case SDLK_AC_SAVE:              return KEY_AC_SAVE;
  case SDLK_AC_PRINT:             return KEY_AC_PRINT;
  case SDLK_AC_PROPERTIES:        return KEY_AC_PROPERTIES;
  case SDLK_AC_SEARCH:            return KEY_AC_SEARCH;
  case SDLK_AC_HOME:              return KEY_AC_HOME;
  case SDLK_AC_BACK:              return KEY_AC_BACK;
  case SDLK_AC_FORWARD:           return KEY_AC_FORWARD;
  case SDLK_AC_STOP:              return KEY_AC_STOP;
  case SDLK_AC_REFRESH:           return KEY_AC_REFRESH;
  case SDLK_AC_BOOKMARKS:         return KEY_AC_BOOKMARKS;
  case SDLK_SOFTLEFT:             return KEY_SOFTLEFT;
  case SDLK_SOFTRIGHT:            return KEY_SOFTRIGHT;
  case SDLK_CALL:                 return KEY_CALL;
  case SDLK_ENDCALL:              return KEY_ENDCALL;
  case SDLK_LEFT_TAB:             return KEY_LEFT_TAB;
  case SDLK_LEVEL5_SHIFT:         return KEY_LEVEL5_SHIFT;
  case SDLK_MULTI_KEY_COMPOSE:    return KEY_MULTI_KEY_COMPOSE;
  case SDLK_LMETA:                return KEY_LMETA;
  case SDLK_RMETA:                return KEY_RMETA;
  case SDLK_LHYPER:               return KEY_LHYPER;
  case SDLK_RHYPER:               return KEY_RHYPER;
  }
  return 0;
}

static void s_poll_event() {
  app.input.any_button = 0;
  mem_move(app.input.keyboard.previous_buttons,
           app.input.keyboard.buttons,
           sizeof(app.input.keyboard.buttons));
  mem_move(app.input.mouse.previous_buttons,
           app.input.mouse.buttons,
           sizeof(app.input.mouse.buttons));
  mem_set(app.input.keyboard.buttons, 0, sizeof(app.input.keyboard.buttons));
  mem_set(app.input.mouse.buttons, 0, sizeof(app.input.mouse.buttons));
  app.input.mouse.delta_position.x = 0;
  app.input.mouse.delta_position.y = 0;
  app.input.mouse.delta_scroll.x = 0;
  app.input.mouse.delta_scroll.y = 0;
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT: app.should_close = true; break;
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
      app.window.size.x = event.window.data1;
      app.window.size.y = event.window.data2;
      glViewport(0, 0, app.window.size.x, app.window.size.y);
      app.window.pixel_density = SDL_GetWindowPixelDensity(sdl.window);
      break;
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
      app.window.display_scale = SDL_GetWindowDisplayScale(sdl.window);
      app.window.pixel_density = SDL_GetWindowPixelDensity(sdl.window);
      break;
    case SDL_EVENT_KEY_DOWN: {
      if (event.key.repeat) {
        continue;
      }
      SDL_Keycode key = SDL_GetKeyFromScancode(event.key.scancode,
                                               event.key.mod,
                                               true);
      key = s_map_key(key);
      app.input.keyboard.buttons[key] = 1;
      app.input.any_button = 1;
    } break;
    case SDL_EVENT_KEY_UP: {
      if (event.key.repeat) {
        continue;
      }
      SDL_Keycode key = SDL_GetKeyFromScancode(event.key.scancode,
                                               event.key.mod,
                                               true);
      key = s_map_key(key);
      app.input.keyboard.buttons[key] = 0;
    } break;
    case SDL_EVENT_MOUSE_MOTION:
      app.input.mouse.position.x = event.motion.x * app.window.pixel_density;
      app.input.mouse.position.y = event.motion.y * app.window.pixel_density;
      app.input.mouse.delta_position.x = event.motion.xrel * app.window.pixel_density;
      app.input.mouse.delta_position.y = event.motion.yrel * app.window.pixel_density;
      break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
      int btn = s_map_mouse_button(event.button.button);
      app.input.mouse.buttons[btn] = 1;
      app.input.any_button = 1;
    } break;
    case SDL_EVENT_MOUSE_BUTTON_UP: {
      int btn = s_map_mouse_button(event.button.button);
      app.input.mouse.buttons[btn] = 0;
    } break;
    case SDL_EVENT_MOUSE_WHEEL:
      // future pinch zoom support maybe
      // const b8 *state = SDL_GetKeyboardState(NULL);
      // if (state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RCTRL]) {
      //     LogInfo("ZOOM?");
      // }
      app.input.mouse.delta_scroll.x = event.wheel.x * app.window.pixel_density;
      app.input.mouse.delta_scroll.y = event.wheel.y * app.window.pixel_density;
      break;
    default: break;
    }
  }
}

static void s_main_loop() {
  s_poll_event();
  f64 newTime = app_get_time();
  app.frame.delta_time = newTime - app.frame.previous_time;
  // if we are under 4fps then slowdown update loop.
  app.frame.accumulator += (app.frame.delta_time > 0.25) ? 0.25 : app.frame.delta_time;
  app.frame.previous_time = newTime;
  while (app.frame.accumulator >= app.tick.delta_time / app.tick.time_scale) {
    app.callback.tick();
    app.tick.time += app.tick.delta_time;
    app.frame.accumulator -= app.tick.delta_time / app.tick.time_scale;
  }
  app.callback.frame();
  draw_ui_graphic();
  SDL_GL_SwapWindow(sdl.window);
}

static b8 s_event_filter(void *userdata, SDL_Event *event) {
  if (event->type == SDL_EVENT_WINDOW_EXPOSED) // linux wayland might need && event->window.data1 == 1
  {
    s_main_loop();
    return false;
  }
  return true;
}

void app_launch(CStr window_title,
                i32  width,
                i32  height,
                void (*init)(),
                void (*tick)(),
                void (*frame)()) {
  SDL_CHECK(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_CAMERA));
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#ifdef gles
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
  if (!SDL_GL_SetSwapInterval(-1)) { // try to adaptive vsync
    SDL_GL_SetSwapInterval(1);
  }
  SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE;
  sdl.window = SDL_CreateWindow(window_title.value, width, height, flags);
  SDL_CHECK(sdl.window);
  sdl.glContext = SDL_GL_CreateContext(sdl.window);
  SDL_CHECK(sdl.glContext);
  SDL_CHECK(SDL_GL_MakeCurrent(sdl.window, sdl.glContext));

#if PLATFORM_WINDOWS || PLATFORM_LINUX || PLATFORM_MACOS
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    SDL_Log("Failed to initialize GLAD\n");
    exit(1);
  }
#endif
  app.callback.init = init;
  app.callback.tick = tick;
  app.callback.frame = frame;
  app.tick.delta_time = 1.0 / 240.0;
  app.tick.time_scale = 1;
  app.frame.previous_time = app_get_time();
  create_ui_graphic();
  init();
  SDL_SetEventFilter(s_event_filter, NULL);
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(s_main_loop, 0, 1);
#else
  while (!app.should_close) {
    s_main_loop();
  }
#endif
  SDL_DestroyCursor(sdl.cursor);
  SDL_GL_DestroyContext(sdl.glContext);
  SDL_DestroyWindow(sdl.window);
  SDL_Quit();
}

f64 app_get_time() {
  return (SDL_GetTicksNS() * 1e-9);
}

StrView app_get_base_path() {
  return app.base_path;
}

StrView window_get_title() {
  return app.window.title;
}

Vec2I32 window_get_size() {
  return app.window.size;
}

f32 window_get_pixel_density() {
  return app.window.pixel_density;
}

f32 window_get_display_scale() {
  return app.window.display_scale;
}

b8 input_is_key_down(KeyboardButton btn) {
  return app.input.keyboard.buttons[btn];
}

b8 input_is_key_up(KeyboardButton btn) {
  return !app.input.keyboard.buttons[btn];
}

b8 input_is_key_pressed(KeyboardButton btn) {
  return !app.input.keyboard.previous_buttons[btn] && app.input.keyboard.buttons[btn];
}

b8 input_is_key_released(KeyboardButton btn) {
  return app.input.keyboard.previous_buttons[btn] && !app.input.keyboard.buttons[btn];
}

b8 input_is_mouse_down(MouseButton btn) {
  return app.input.mouse.buttons[btn];
}

b8 input_is_mouse_up(MouseButton btn) {
  return !app.input.mouse.buttons[btn];
}

b8 input_is_mouse_pressed(MouseButton btn) {
  return !app.input.mouse.previous_buttons[btn] && app.input.mouse.buttons[btn];
}

b8 input_is_mouse_released(MouseButton btn) {
  return app.input.mouse.previous_buttons[btn] && !app.input.mouse.buttons[btn];
}

Vec2I32 input_get_mouse_position() {
  return app.input.mouse.position;
}

Vec2I32 input_get_mouse_delta_position() {
  return app.input.mouse.delta_position;
}

Vec2I32 input_get_mouse_delta_scroll() {
  return app.input.mouse.delta_scroll;
}

f64 tick_get_delta_time() {
  return app.tick.delta_time;
}

f64 tick_get_time() {
  return app.tick.time;
}

f64 tick_get_time_scale() {
  return app.tick.time_scale;
}

f64 frame_get_delta_time() {
  return app.frame.delta_time;
}

f64 frame_get_time() {
  return app.frame.time;
}

f64 frame_get_interpolant() {
  return app.frame.interpolant;
}
