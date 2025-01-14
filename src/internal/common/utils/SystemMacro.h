#ifndef EURORA_COMMON_UTILS_SYSTEM_MACRO_
#define EURORA_COMMON_UTILS_SYSTEM_MACRO_

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)
#if defined(EURORA_BUILD_EXPORTS)
#define EURORA_API __declspec(dllexport)
#elif defined(EURORA_USE_SHARED)
#define EURORA_API __declspec(dllimport)
#else
#define EURORA_API
#endif
#else
#if __GNUC__ >= 4
#define EURORA_API __attribute__((visibility("default")))
#else
#define EURORA_API
#endif
#endif

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // EURORA_COMMON_UTILS_SYSTEM_MACRO_
