#ifndef GPU_VULKAN_INCLUDE_H
#define GPU_VULKAN_INCLUDE_H

#if PLATFORM_WINDOWS
#  define VK_USE_PLATFORM_WIN32_KHR
#elif PLATFORM_LINUX
#  define VK_USE_PLATFORM_WAYLAND_KHR
#else
#  error Vulkan layer not implemented for this platform.
#endif

#include <volk/volk.h>

typedef struct Vulkan_State {
  VkInstance       instance;
  VkSurfaceKHR     surface;
  VkPhysicalDevice physical_device;
  u32              queue_family_index;
  VkDevice         logical_device;
  VkQueue          queue;
  VkSwapchainKHR   swapchain;
} Vulkan_State;

GLOBAL Vulkan_State vulkan_state = { 0 };

// check

#if PLATFORM_WINDOWS
#  define VK_CHECK(call)                                          \
    do {                                                          \
        VkResult result = (call);                                 \
        if (result != VK_SUCCESS) {                               \
          reportf(REPORT_FATAL,                                   \
               str8_from_lit("Vulkan Error"),                     \
               "Vulkan Error in %s Result: %s (%d)",              \
               #call, vulkan_result_to_string(result), result);   \
        }                                                         \
    } while (0)
#elif PLATFORM_LINUX
// TODO
#endif

// Flows

FUNCTION void vulkan_init();
FUNCTION void vulkan_cleanup();

// Utils

FUNCTION const char *vulkan_result_to_string(VkResult input_value);

#endif // GPU_VULKAN_INCLUDE_H
