#ifndef OS_CORE_H
#define OS_CORE_H

// System Info

typedef struct OS_SystemInfo {
  u32 logical_processor_count;
  u32 page_size;
} OS_SystemInfo;

// Process Info

typedef struct OS_ProcessInfo {
  Str8 binary_path;
  Str8 binary_name;
  Str8 initial_path;
} OS_ProcessInfo;

// Info

function OS_SystemInfo  *os_get_system_info(void); // (Implemented Per-OS)
function OS_ProcessInfo *os_get_process_info(void); // (Implemented Per-OS)

// Memory

function void *os_memory_reserve(u64 size); // (Implemented Per-OS)
function void  os_memory_release(void *ptr, u64 size); // (Implemented Per-OS)

// File System

// Abort

function void os_abort(i32 exit_code); // (Implemented Per-OS)

// Time

function u64  os_now_microseconds(void); // (Implemented Per-OS)
function void os_sleep_milliseconds(u32 msec); // (Implemented Per-OS)
function u64  os_now_unix(void); // (Implemented Per-OS)

// Thread & Sync Primitive

#endif
