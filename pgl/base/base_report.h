#ifndef BASE_REPORT_H
#define BASE_REPORT_H

typedef enum ReportKind {
  REPORT_INFO,  // must not end process, no popup only to UI
  REPORT_WARN,  // must not end process, the popup will be from UI
  REPORT_ERROR, // must not end process, the popup will be from UI
  REPORT_FATAL, // must end process, the popup will be from OS
} ReportKind;

FUNCTION void _report(ReportKind state, char *file, u32 line, Str8 title, Str8 msg);
FUNCTION void _reportv(ReportKind state, char *file, u32 line, Str8 title, char *fmt, va_list args);
FUNCTION void _reportf(ReportKind state, char *file, u32 line, Str8 title, char *fmt, ...);

#define report(state, title, msg) _report(state, __FILE__, __LINE__, title, msg);
#define reportf(state, title, fmt, ...) _reportf(state, __FILE__, __LINE__, title, fmt, ##__VA_ARGS__);

#endif // BASE_REPORT_H
