#define BUILD_DEBUG 0
#include "pgl_include.h"
#include "pgl_include.c"

#include <stdio.h>
#define eval_print(x) fprintf(stdout, "%s = %.2f\n", #x, (float)(x))
#define eval_print_u64(x) fprintf(stdout, "%s = %llu\n", #x, (u64)(x))
#define eval_print_str8(x) fprintf(stdout, "%s = "FMT_STR8"\n", #x,  str8_varg(x))

i32 state = 1;

GLOBAL OS_AppInitSetting os_app_init_setting = {
  .size = { 300, 300 },
  .position = { 500, 100 },
  .msaa = 0,
  .fullscreen = 0,
  .window_title = str8_from_lit("hello window việt")
};

void init(void) {
  // defines
  {
    eval_print(COMPILER_MSVC);
    eval_print(COMPILER_MSVC);
    eval_print(COMPILER_CLANG);
    eval_print(COMPILER_GCC);
    eval_print(PLATFORM_WINDOWS);
    eval_print(PLATFORM_WEB);
    eval_print(PLATFORM_LINUX);
    eval_print(PLATFORM_MACOS);
    eval_print(PLATFORM_IOS);
    eval_print(PLATFORM_ANDROID);

    u64 version = version(1, 7, 10);
    eval_print(major_from_version(version));
    eval_print(minor_from_version(version));
    eval_print(patch_from_version(version));
  }

  printf("a ă â b c d đ e ê g h i k l m n o ô ơ p q r s t u ư v x y\n");
  printf("\033[H\033[2J\033[s\n");
}

void tick(void) {
}

void frame(void) {
  printf(
    "\033[u\033[J"
    "--- DEBUG LOGS ---\n"
    "SYSTEM INFO:\n"
    "\tLogical Processor Count: " FMT_U32 "\n"
    "\tPage Size: " FMT_U32 "\n"
    "PROCESS INFO:\n"
    "\tBinary Path: " FMT_STR8 "\n"
    "\tBinary Name: " FMT_STR8 "\n"
    "\tInitial Path: " FMT_STR8 "\n"
    "ARGS: (TODO)\n"
    "GFX INFO: (TODO)\n"
    "WINDOW:\n"
    "\tLogical position: x:%d y:%d\n"
    "\tLogical size: x:%d y:%d\n"
    "\tPhysical size: x:%d y:%d\n"
    "\tDPI scale: " FMT_F32 "\n"
    "INPUT:\n"
    "\tCursor Position: x:%d y:%d\n"
    "TICK:\n"
    "\tDelta Time: " FMT_F64 "\n"
    "\tTime: " FMT_F64 "\n"
    "\tTime Scale: " FMT_F64 "\n"
    "FRAME:\n"
    "\tDelta Time: " FMT_F64 "\n"
    "\tTime: " FMT_F64 "\n"
    "\tInterpolant: " FMT_F64 "\n"
    "",
    os_app_data.system_info.logical_processor_count,
    os_app_data.system_info.page_size,
    str8_varg(os_app_data.process_info.binary_path),
    str8_varg(os_app_data.process_info.binary_name),
    str8_varg(os_app_data.process_info.initial_path),
    // TODO
    // TODO
    os_app_data.window.logical_position.x, os_app_data.window.logical_position.y,
    os_app_data.window.logical_size.x, os_app_data.window.logical_size.y,
    os_app_data.window.physical_size.x, os_app_data.window.physical_size.y,
    os_app_data.window.dpi_scale,
    os_app_data.input.cursor.position.x, os_app_data.input.cursor.position.y,
    os_app_data.tick.delta_time,
    os_app_data.tick.time,
    os_app_data.tick.time_scale,
    os_app_data.frame.delta_time,
    os_app_data.frame.time,
    os_app_data.frame.interpolant);
  // if (os_app_data.input.cursor.delta_position.x != 0 || os_app_data.input.cursor.delta_position.y != 0) {
  //   printf("A: %d %d\n", os_app_data.input.cursor.position.x, os_app_data.input.cursor.position.y);
  // }
  // if (os_app_data.input.cursor.delta_scroll.x != 0 || os_app_data.input.cursor.delta_scroll.y != 0) {
  //   printf("B: %d %d\n", os_app_data.input.cursor.delta_scroll.x, os_app_data.input.cursor.delta_scroll.y);
  // }
}

void cleanup(void) {
}
