#ifndef GPU_INCLUDE_H
#define GPU_INCLUDE_H

#if PLATFORM_WINDOWS
#  define GPU_VULKAN 1
#elif PLATFORM_LINUX
#  define GPU_VULKAN 1
#elif PLATFORM_WEB
#  define GPU_WEBGPU 1
#endif

#if !defined(GPU_VULKAN)
#  define GPU_VULKAN 0
#endif
#if !defined(GPU_WEBGPU)
#  define GPU_WEBGPU 0
#endif

#if GPU_VULKAN
#  include "vulkan/gpu_vulkan.h"
#elif GPU_WEBGPU
#  include "webgpu/gpu_webgpu.h"
#else
#  error GPU layer not implemented for this platform
#endif

#endif // GPU_INCLUDE_H
