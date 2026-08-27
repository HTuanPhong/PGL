#include <volk/volk.c>

FUNCTION const char *
vulkan_result_to_string(VkResult input_value) {
  switch (input_value) {
  case VK_SUCCESS:
    return "VK_SUCCESS";
  case VK_NOT_READY:
    return "VK_NOT_READY";
  case VK_TIMEOUT:
    return "VK_TIMEOUT";
  case VK_EVENT_SET:
    return "VK_EVENT_SET";
  case VK_EVENT_RESET:
    return "VK_EVENT_RESET";
  case VK_INCOMPLETE:
    return "VK_INCOMPLETE";
  case VK_ERROR_OUT_OF_HOST_MEMORY:
    return "VK_ERROR_OUT_OF_HOST_MEMORY";
  case VK_ERROR_OUT_OF_DEVICE_MEMORY:
    return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
  case VK_ERROR_INITIALIZATION_FAILED:
    return "VK_ERROR_INITIALIZATION_FAILED";
  case VK_ERROR_DEVICE_LOST:
    return "VK_ERROR_DEVICE_LOST";
  case VK_ERROR_MEMORY_MAP_FAILED:
    return "VK_ERROR_MEMORY_MAP_FAILED";
  case VK_ERROR_LAYER_NOT_PRESENT:
    return "VK_ERROR_LAYER_NOT_PRESENT";
  case VK_ERROR_EXTENSION_NOT_PRESENT:
    return "VK_ERROR_EXTENSION_NOT_PRESENT";
  case VK_ERROR_FEATURE_NOT_PRESENT:
    return "VK_ERROR_FEATURE_NOT_PRESENT";
  case VK_ERROR_INCOMPATIBLE_DRIVER:
    return "VK_ERROR_INCOMPATIBLE_DRIVER";
  case VK_ERROR_TOO_MANY_OBJECTS:
    return "VK_ERROR_TOO_MANY_OBJECTS";
  case VK_ERROR_FORMAT_NOT_SUPPORTED:
    return "VK_ERROR_FORMAT_NOT_SUPPORTED";
  case VK_ERROR_FRAGMENTED_POOL:
    return "VK_ERROR_FRAGMENTED_POOL";
  case VK_ERROR_UNKNOWN:
    return "VK_ERROR_UNKNOWN";
  case VK_ERROR_VALIDATION_FAILED:
    return "VK_ERROR_VALIDATION_FAILED";
  case VK_ERROR_OUT_OF_POOL_MEMORY:
    return "VK_ERROR_OUT_OF_POOL_MEMORY";
  case VK_ERROR_INVALID_EXTERNAL_HANDLE:
    return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
  case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
    return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
  case VK_ERROR_FRAGMENTATION:
    return "VK_ERROR_FRAGMENTATION";
  case VK_PIPELINE_COMPILE_REQUIRED:
    return "VK_PIPELINE_COMPILE_REQUIRED";
  case VK_ERROR_NOT_PERMITTED:
    return "VK_ERROR_NOT_PERMITTED";
  case VK_ERROR_SURFACE_LOST_KHR:
    return "VK_ERROR_SURFACE_LOST_KHR";
  case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
    return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
  case VK_SUBOPTIMAL_KHR:
    return "VK_SUBOPTIMAL_KHR";
  case VK_ERROR_OUT_OF_DATE_KHR:
    return "VK_ERROR_OUT_OF_DATE_KHR";
  case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
    return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
  case VK_ERROR_INVALID_SHADER_NV:
    return "VK_ERROR_INVALID_SHADER_NV";
  case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
    return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
  case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
    return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
  case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
    return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
  case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
    return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
  case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
    return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
  case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
    return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
  case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
    return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
  case VK_ERROR_PRESENT_TIMING_QUEUE_FULL_EXT:
    return "VK_ERROR_PRESENT_TIMING_QUEUE_FULL_EXT";
  case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
    return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
  case VK_THREAD_IDLE_KHR:
    return "VK_THREAD_IDLE_KHR";
  case VK_THREAD_DONE_KHR:
    return "VK_THREAD_DONE_KHR";
  case VK_OPERATION_DEFERRED_KHR:
    return "VK_OPERATION_DEFERRED_KHR";
  case VK_OPERATION_NOT_DEFERRED_KHR:
    return "VK_OPERATION_NOT_DEFERRED_KHR";
  case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
    return "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR";
  case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
    return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
  case VK_INCOMPATIBLE_SHADER_BINARY_EXT:
    return "VK_INCOMPATIBLE_SHADER_BINARY_EXT";
  case VK_PIPELINE_BINARY_MISSING_KHR:
    return "VK_PIPELINE_BINARY_MISSING_KHR";
  case VK_ERROR_NOT_ENOUGH_SPACE_KHR:
    return "VK_ERROR_NOT_ENOUGH_SPACE_KHR";
  default:
    return "Unhandled VkResult";
  }
}

FUNCTION void
vulkan_create_instance() {
  VK_CHECK(volkInitialize());

  // LAYERS:
  const char *layer_names[32] = { 0 };
  u32         layer_count = 0;
#if BUILD_DEBUG
  layer_names[layer_count++] = "VK_LAYER_KHRONOS_validation";
#endif
  assert_always(layer_count <= 32);

  // EXTENSIONS:
  const char *extension_names[32] = { 0 };
  u32         extension_count = 0;
  extension_names[extension_count++] = VK_KHR_SURFACE_EXTENSION_NAME;
#if PLATFORM_WINDOWS
  extension_names[extension_count++] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
#elif PLATFORM_LINUX
  extension_names[extension_count++] = VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
#endif
#if BUILD_DEBUG
  extension_names[extension_count++] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
#endif
  assert_always(extension_count <= 32);

  // make instance
  VkApplicationInfo app_info = { 0 };
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "PGL_VULKAN";               // TODO let people name this ?
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // TODO
  app_info.pEngineName = "PGL";                           // TODO
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_4;

  VkInstanceCreateInfo create_info = { 0 };
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;
  create_info.enabledLayerCount = layer_count;
  create_info.ppEnabledLayerNames = layer_names;
  create_info.enabledExtensionCount = extension_count;
  create_info.ppEnabledExtensionNames = extension_names;

  VK_CHECK(vkCreateInstance(&create_info, NULL, &vulkan_state.instance));

  volkLoadInstance(vulkan_state.instance);
}

FUNCTION void
vulkan_create_surface() {
#if PLATFORM_WINDOWS
  VkWin32SurfaceCreateInfoKHR win32_info = { 0 };
  win32_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  win32_info.hinstance = os_w32_state.hinstance;
  win32_info.hwnd = os_w32_state.hwnd;
  VK_CHECK(vkCreateWin32SurfaceKHR(vulkan_state.instance, &win32_info, 0, &vulkan_state.surface));
#elif PLATFORM_LINUX
#  error TODO
#endif
}

FUNCTION b8
vulkan_device_check_extensions(VkPhysicalDevice pdev, const char **required_exts, u32 num_required_exts) {
  ArenaMarker m = arena_begin(0);

  u32 ext_count = 0;
  VK_CHECK(vkEnumerateDeviceExtensionProperties(pdev, 0, &ext_count, 0));
  VkExtensionProperties *ext_props = arena_push_array(m.arena, VkExtensionProperties, ext_count);
  VK_CHECK(vkEnumerateDeviceExtensionProperties(pdev, 0, &ext_count, ext_props));

  b8 result = true;

  for (u32 i = 0; i < num_required_exts; i++) {
    b8 found = false;
    for (u32 j = 0; j < ext_count; j++) {
      if (strcmp(required_exts[i], ext_props[j].extensionName) == 0) {
        found = true;
      }
    }
    if (!found) {
      result = false;
      goto clean_up;
    }
  }

clean_up:
  arena_end(m);
  return result;
}

FUNCTION b8
vulkan_device_check_queue_families(VkPhysicalDevice pdev) {
  ArenaMarker m = arena_begin(0);

  u32 queue_family_properties_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(pdev, &queue_family_properties_count, NULL);
  VkQueueFamilyProperties *queue_family_properties = arena_push_array(m.arena, VkQueueFamilyProperties, queue_family_properties_count);
  vkGetPhysicalDeviceQueueFamilyProperties(pdev, &queue_family_properties_count, queue_family_properties);

  b8 result = false;

  const VkQueueFlags required_flags = VK_QUEUE_GRAPHICS_BIT
                                    | VK_QUEUE_COMPUTE_BIT
                                    | VK_QUEUE_TRANSFER_BIT;
  for (u32 i = 0; i < queue_family_properties_count; i++) {
    if ((queue_family_properties[i].queueFlags & required_flags) == required_flags) {
      vulkan_state.queue_family_index = i;
      result = true;
    }
  }

  arena_end(m);
  return result;
}

FUNCTION b8
vulkan_device_check_presentation_supported(VkPhysicalDevice pdev) {
  VkBool32 presentation_supported = VK_FALSE;
  VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(pdev, vulkan_state.queue_family_index, vulkan_state.surface, &presentation_supported));
  return presentation_supported == VK_TRUE;
}

FUNCTION void
vulkan_create_device() {
  ArenaMarker m = arena_begin(0);

  const char *extension_names[32] = { 0 };
  u32         extension_count = 0;
  extension_names[extension_count++] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
  assert_always(extension_count <= 32);

  u32 physical_device_count = 0;
  VK_CHECK(vkEnumeratePhysicalDevices(vulkan_state.instance, &physical_device_count, NULL));
  VkPhysicalDevice *physical_devices = arena_push_array(m.arena, VkPhysicalDevice, physical_device_count);
  VK_CHECK(vkEnumeratePhysicalDevices(vulkan_state.instance, &physical_device_count, physical_devices));

  for (u32 i = 0; i < physical_device_count; i++) {
    VkPhysicalDeviceProperties physical_device_properties = { 0 };
    vkGetPhysicalDeviceProperties(physical_devices[i], &physical_device_properties);
    if (physical_device_properties.apiVersion < VK_API_VERSION_1_4) { // if people fail this they need to update their driver
      continue;
    }
    if (!vulkan_device_check_extensions(physical_devices[i], extension_names, extension_count)) {
      continue;
    }
    if (!vulkan_device_check_queue_families(physical_devices[i])) {
      continue;
    }
    if (!vulkan_device_check_presentation_supported(physical_devices[i])) {
      continue;
    }
    // TODO check if feature extension if used maybe
    vulkan_state.physical_device = physical_devices[i];
    break;
  }

  if (!vulkan_state.physical_device) {
    report(REPORT_FATAL, str8_from_lit("Vulkan Error"), str8_from_lit("Try update your driver"));
  }

  const f32               queue_priority = 1.0f;
  VkDeviceQueueCreateInfo queue_create_info = { 0 };
  queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queue_create_info.queueFamilyIndex = vulkan_state.queue_family_index;
  queue_create_info.queueCount = 1;
  queue_create_info.pQueuePriorities = &queue_priority;

  VkPhysicalDeviceVulkan12Features vk12_features = { 0 };
  vk12_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
  vk12_features.descriptorIndexing = true;
  vk12_features.shaderSampledImageArrayNonUniformIndexing = true;
  vk12_features.descriptorBindingVariableDescriptorCount = true;
  vk12_features.runtimeDescriptorArray = true;
  vk12_features.bufferDeviceAddress = true;

  VkPhysicalDeviceVulkan13Features vk13_features = { 0 };
  vk13_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
  vk13_features.pNext = &vk12_features;
  vk13_features.synchronization2 = true;
  vk13_features.dynamicRendering = true;

  VkPhysicalDeviceFeatures vk10_features = { 0 };
  vk10_features.samplerAnisotropy = true;

  VkDeviceCreateInfo dev_create_info = { 0 };
  dev_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  dev_create_info.pNext = &vk13_features;
  dev_create_info.queueCreateInfoCount = 1;
  dev_create_info.pQueueCreateInfos = &queue_create_info;
  dev_create_info.enabledExtensionCount = extension_count;
  dev_create_info.ppEnabledExtensionNames = extension_names;
  dev_create_info.pEnabledFeatures = &vk10_features;

  VK_CHECK(vkCreateDevice(vulkan_state.physical_device, &dev_create_info, 0, &vulkan_state.logical_device));
  vkGetDeviceQueue(vulkan_state.logical_device, vulkan_state.queue_family_index, 0, &vulkan_state.queue);

  arena_end(m);
}

FUNCTION void
vulkan_create_swap_chain() {
  // VkSurfaceCapabilitiesKHR surf_caps = {0};
  // VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan_state.physical_device, vulkan_state.surface, &surf_caps));
  // // todo minized window has zero width/height on some platforms
  // VkSwapchainKHR old_swapchain = vulkan_state.swapchain;
  // _sapp.surface_format =
  // VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
}

FUNCTION void
vulkan_create_image_views() {
}

FUNCTION void
vulkan_create_graphic_pipeline() {
}

FUNCTION void
vulkan_create_command_pool() {
}

FUNCTION void
vulkan_create_vertex_buffer() {
}

FUNCTION void
vulkan_create_command_buffers() {
}

FUNCTION void
vulkan_create_sync_objects() {
}

FUNCTION void
vulkan_init() {
  vulkan_create_instance();
  vulkan_create_surface();
  vulkan_create_device();
  vulkan_create_swap_chain();
  vulkan_create_image_views();
  vulkan_create_graphic_pipeline();
  vulkan_create_command_pool();
  vulkan_create_vertex_buffer();
  vulkan_create_command_buffers();
  vulkan_create_sync_objects();
}
