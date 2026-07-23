FUNCTION void
os_init_systems(void) {
  init();
}

FUNCTION void
os_loop_systems(void) {
  os_app_data.frame.time = os_now_seconds();
  os_app_data.frame.delta_time = os_app_data.frame.time - os_app_data.frame.previous_time;
  // if we are under 4fps then slowdown update loop.
  os_app_data.frame.accumulator += clamp_top(os_app_data.frame.delta_time, 0.25);
  os_app_data.frame.previous_time = os_app_data.frame.time;
  f64 dt = os_app_data.tick.delta_time / os_app_data.tick.time_scale;
  while (os_app_data.frame.accumulator >= dt) {
    // tick other library stuff here in the future
    tick();
    os_app_data.tick.time += os_app_data.tick.delta_time;
    os_app_data.frame.accumulator -= dt;
  }
  os_app_data.frame.interpolant = os_app_data.frame.accumulator / dt;
  // frame other library stuff here in the future
  frame();
}

FUNCTION void
os_cleanup_systems(void) {
  cleanup();
}

// Prints

FUNCTION void
os_printv(OS_PrintKind kind, char *fmt, va_list args) {
  va_list args2;
  va_copy(args2, args);
  ScratchMarker m = scratch_begin(0);

  Str8Buf buf = { 0 };
  str8buf_appendv(m.scratch, &buf, fmt, args);
  os_print(kind, buf.str);

  scratch_end(m);
  va_end(args2);
}

FUNCTION void
os_printf(OS_PrintKind kind, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  os_printv(kind, fmt, args);
  va_end(args);
}
