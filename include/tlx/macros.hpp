//
// Created by muham on 15.07.2026.
//

#ifndef TLX_MACROS_HPP
#define TLX_MACROS_HPP

#if TLX_HAS_CUDA
    #include <cuda_runtime.h>

    #define TLX_HD          __host__ __device__
    #define TLX_HOST        __host__
    #define TLX_DEVICE      __device__
    #define TLX_GLOBAL      __global__
    #define TLX_CONSTANT    __constant__
    #define TLX_SHARED      __shared__
    #define TLX_INLINE      __forceinline__
#else //#if TLX_HAS_CUDA
    #define TLX_HD
    #define TLX_HOST
    #define TLX_DEVICE
    #define TLX_GLOBAL
    #define TLX_CONSTANT
    #define TLX_SHARED
    #define TLX_INLINE inline
#endif //#if TLX_HAS_CUDA #else

#define TLX_SAFE_EXIT 0
#define TLX_ERR_EXIT 1

#endif //TLX_MACROS_HPP