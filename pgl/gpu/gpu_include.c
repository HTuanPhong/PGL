#if GPU_VULKAN
#  include "vulkan/gpu_vulkan.c"
#elif GPU_WEBGPU
#  include "webgpu/gpu_webgpu.c"
#else
#  error GPU layer not implemented for this platform
#endif
