#pragma once

#include <string>
#include <array>

namespace eurora::utils {

enum class ErrorCode : int {
    kSuccess = 0,

    kIO_FileNotFound     = 1000,
    kIO_FileOpenFailed   = 1001,
    kIO_ReadError        = 1002,
    kIO_WriteError       = 1003,
    kIO_PermissionDenied = 1004,

    kData_DimensionMismatch = 2000,
    kData_OutOfRangeIndex   = 2001,
    kData_NullPointer       = 2002,
    kData_EmptyArray        = 2003,
    kData_UnsupportedFormat = 2004,

    kAlgo_NotConverged     = 3000,
    kAlgo_InvalidParameter = 3001,
    kAlgo_SingularMatrix   = 3002,
    kAlgo_NoSolution       = 3003,
    kAlgo_Overflow         = 3004,

    kCompute_GpuInitFailed    = 4000,
    kCompute_CudaError        = 4001,
    kCompute_OpenCLError      = 4002,
    kCompute_KernelLaunchFail = 4003,
    kCompute_DeviceMismatch   = 4004,

    kMemory_AllocationFailed  = 5000,
    kMemory_DeallocationError = 5001,
    kMemory_OutOfMemory       = 5002,
    kMemory_LeakDetected      = 5003,

    kDevice_NotFound     = 6000,
    kDevice_Busy         = 6001,
    kDevice_Timeout      = 6002,
    kDevice_Incompatible = 6003,

    kNet_Disconnected   = 7000,
    kNet_Timeout        = 7001,
    kNet_InvalidAddress = 7002,
    kNet_ProtocolError  = 7003,

    kInvalidArgument = 8000,

    kUnknownError = 9999,
};

struct ErrorMapping {
    ErrorCode code_;
    const char* message_;
};

inline constexpr std::array<ErrorMapping, 35> ErrorMessages = {{{ErrorCode::kSuccess, "Success"},

                                                                {ErrorCode::kIO_FileNotFound, "File not found"},
                                                                {ErrorCode::kIO_FileOpenFailed, "File open failed"},
                                                                {ErrorCode::kIO_ReadError, "File read error"},
                                                                {ErrorCode::kIO_WriteError, "File write error"},
                                                                {ErrorCode::kIO_PermissionDenied, "Permission denied"},

                                                                {ErrorCode::kData_DimensionMismatch, "Data dimension mismatch"},
                                                                {ErrorCode::kData_OutOfRangeIndex, "Data index out of range"},
                                                                {ErrorCode::kData_NullPointer, "Null pointer accessed"},
                                                                {ErrorCode::kData_EmptyArray, "Empty array"},
                                                                {ErrorCode::kData_UnsupportedFormat, "Unsupported data format"},

                                                                {ErrorCode::kAlgo_NotConverged, "Algorithm did not converge"},
                                                                {ErrorCode::kAlgo_InvalidParameter, "Invalid algorithm parameter"},
                                                                {ErrorCode::kAlgo_SingularMatrix, "Singular matrix encountered"},
                                                                {ErrorCode::kAlgo_NoSolution, "No solution found"},
                                                                {ErrorCode::kAlgo_Overflow, "Algorithm overflow"},

                                                                {ErrorCode::kCompute_GpuInitFailed, "GPU initialization failed"},
                                                                {ErrorCode::kCompute_CudaError, "CUDA error"},
                                                                {ErrorCode::kCompute_OpenCLError, "OpenCL error"},
                                                                {ErrorCode::kCompute_KernelLaunchFail, "Kernel launch failed"},
                                                                {ErrorCode::kCompute_DeviceMismatch, "Compute device mismatch"},

                                                                {ErrorCode::kMemory_AllocationFailed, "Memory allocation failed"},
                                                                {ErrorCode::kMemory_DeallocationError, "Memory deallocation error"},
                                                                {ErrorCode::kMemory_OutOfMemory, "Out of memory"},
                                                                {ErrorCode::kMemory_LeakDetected, "Memory leak detected"},

                                                                {ErrorCode::kDevice_NotFound, "Device not found"},
                                                                {ErrorCode::kDevice_Busy, "Device busy"},
                                                                {ErrorCode::kDevice_Timeout, "Device timeout"},
                                                                {ErrorCode::kDevice_Incompatible, "Device incompatible"},

                                                                {ErrorCode::kNet_Disconnected, "Network disconnected"},
                                                                {ErrorCode::kNet_Timeout, "Network timeout"},
                                                                {ErrorCode::kNet_InvalidAddress, "Invalid network address"},
                                                                {ErrorCode::kNet_ProtocolError, "Network protocol error"},

                                                                {ErrorCode::kInvalidArgument, "Invalid argument provided"},

                                                                {ErrorCode::kUnknownError, "An unknown error occurred"}}};

inline std::string GetErrorMessage(ErrorCode code) {
    for (const auto& mapping : ErrorMessages) {
        if (mapping.code_ == code) {
            return mapping.message_;
        }
    }
    return "Unknown error code";
}

}  // namespace eurora::utils
