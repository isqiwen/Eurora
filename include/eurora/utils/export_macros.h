#pragma once

#if defined(_WIN32) || defined(_WIN64)

#if defined(EURORA_BUILD_SHARED_LIBS)
#define EURORA_API __declspec(dllexport)
#else
#define EURORA_API __declspec(dllimport)
#endif

#else  // For non-Windows platforms

#if defined(EURORA_BUILD_SHARED_LIBS)
#define EURORA_API __attribute__((visibility("default")))
#else
#define EURORA_API
#endif

#endif  // End of header guard
