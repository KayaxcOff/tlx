cmake_minimum_required(VERSION 4.1)

project(TLX LANGUAGES CXX)

include(CheckLanguage)
check_language(CUDA)

if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    message(STATUS "CUDA enabled")
else()
    message(STATUS "CUDA not found. Building without CUDA support.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CUDA_COMPILER)
    set(CMAKE_CUDA_STANDARD 20)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
endif()

if(NOT MSVC)
    set(TLX_CPU_FLAGS "$<$<COMPILE_LANGUAGE:CXX>:-mavx2;-mf16c>")
else()
    set(TLX_CPU_FLAGS "$<$<COMPILE_LANGUAGE:CXX>:/arch:AVX2;/Zc:preprocessor>")
endif()

if(CMAKE_CUDA_COMPILER)
    set(CMAKE_CUDA_ARCHITECTURES "all-major" CACHE STRING "CUDA architectures to target")
endif()

add_library(TLX STATIC
        source/time.cpp
        source/memory.cpp
        source/errors.cpp
        source/buffer.cpp
        source/UTF-8.cpp
        source/file_system.cpp
        source/types.cpp
        source/string.cpp
)

target_compile_options(TLX PRIVATE ${TLX_CPU_FLAGS})
target_compile_options(TLX PUBLIC $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler=/arch:AVX2,/Zc:preprocessor>)

target_include_directories(TLX
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(CMAKE_CUDA_COMPILER)
    find_package(CUDAToolkit REQUIRED)

    target_link_libraries(TLX
            PUBLIC
            CUDA::cudart
    )

    target_compile_definitions(TLX
            PUBLIC
            TLX_HAS_CUDA
    )

    if(NOT MSVC)
        target_compile_options(TLX PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler=-mavx2,-mf16c>)
    else()
        target_compile_options(TLX PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler=/arch:AVX2,/Zc:preprocessor>)
    endif()
endif()

add_executable(TLX_HOST_TEST
        tests/main.cpp
)

target_link_libraries(TLX_HOST_TEST
        PRIVATE
        TLX
)
target_compile_options(TLX_HOST_TEST PRIVATE ${TLX_CPU_FLAGS})

if(CMAKE_CUDA_COMPILER)

    add_executable(TLX_CUDA_TEST
            tests/main.cu
    )

    target_link_libraries(TLX_CUDA_TEST
            PRIVATE
            TLX
    )

    target_compile_options(TLX_CUDA_TEST PRIVATE ${TLX_CPU_FLAGS})
    target_compile_options(TLX_CUDA_TEST PRIVATE
            $<$<COMPILE_LANGUAGE:CUDA>:
            --expt-relaxed-constexpr
            --diag-suppress=20012
            >
    )
    target_compile_options(TLX_CUDA_TEST PRIVATE
            $<$<COMPILE_LANGUAGE:CXX>:/wd4556>
            $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler=/wd4556>
    )
    if(NOT MSVC)
        target_compile_options(TLX_CUDA_TEST PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler=-mavx2,-mf16c>)
    else()
        target_compile_options(TLX_CUDA_TEST PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler=/arch:AVX2,/Zc:preprocessor>)
    endif()

endif()