FUNCTION void
_report(ReportKind state, char *file, u32 line, Str8 title, Str8 msg) {
  switch (state) {
  case REPORT_INFO: {
    os_printf(OS_PRINT_INFO, "[INFO] %s:%d\n\t" FMT_STR8 ": " FMT_STR8 "\n",
              file, line, str8_varg(title), str8_varg(msg));
  } break;
  case REPORT_WARN: {
    os_printf(OS_PRINT_WARN, "[WARN] %s:%d\n\t" FMT_STR8 ": " FMT_STR8 "\n",
              file, line, str8_varg(title), str8_varg(msg));
    os_graphical_message(false, title, msg); // TODO this should be ui
  } break;
  case REPORT_ERROR: {
    os_printf(OS_PRINT_ERROR, "[ERROR] %s:%d\n\t" FMT_STR8 ": " FMT_STR8 "\n",
              file, line, str8_varg(title), str8_varg(msg));
    os_graphical_message(true, title, msg); // TODO this should be ui
  } break;
  case REPORT_FATAL: {
    os_printf(OS_PRINT_ERROR, "[FATAL] %s:%d\n\t" FMT_STR8 ": " FMT_STR8 "\n",
              file, line, str8_varg(title), str8_varg(msg));
    os_graphical_message(true, title, msg);
    os_abort(-1);
  } break;
  default:
    break;
  }
}

FUNCTION void
_reportv(ReportKind state, char *file, u32 line, Str8 title, char *fmt, va_list args) {
  va_list args2;
  va_copy(args2, args);
  ScratchMarker m = scratch_begin(0);

  Str8Buf buf = { 0 };
  str8buf_appendv(m.scratch, &buf, fmt, args);
  _report(state, file, line, title, buf.str);

  scratch_end(m);
  va_end(args2);
}

FUNCTION void
_reportf(ReportKind state, char *file, u32 line, Str8 title, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  _reportv(state, file, line, title, fmt, args);
  va_end(args);
}
