#include "base/base_include.h"

#include "base/base_include.c"

#include <stdio.h>
int main() {
  printf("MSVC       = %d\n", COMPILER_MSVC);
  printf("Clang      = %d\n", COMPILER_CLANG);
  printf("GCC        = %d\n", COMPILER_GCC);
  printf("Windows    = %d\n", PLATFORM_WINDOWS);
  printf("Emscripten = %d\n", PLATFORM_EMSCRIPTEN);
  printf("Linux      = %d\n", PLATFORM_LINUX);
  printf("MacOS      = %d\n", PLATFORM_MACOS);
  printf("IOS        = %d\n", PLATFORM_IOS);
  printf("Android    = %d\n", PLATFORM_ANDROID);
  

  
  assert(1+1==3);
  #define BUILD_DEBUG 1
  assert(1+1==2);
  assert(1+1==3);

  return 0;
}
