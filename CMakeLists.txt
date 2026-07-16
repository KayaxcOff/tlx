cmake_minimum_required(VERSION 3.20)

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

target_include_directories(TLX
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)

    find_package(CUDAToolkit REQUIRED)

    target_link_libraries(TLX
            PUBLIC
            CUDA::cudart
    )

    target_compile_definitions(TLX
            PUBLIC
            TLX_HAS_CUDA
    )
endif()

add_executable(TLX_HOST_TEST
        tests/main.cpp
)

target_link_libraries(TLX_HOST_TEST
        PRIVATE
        TLX
)

if(CMAKE_CUDA_COMPILER)

    add_executable(TLX_CUDA_TEST
            tests/main.cu
    )

    target_link_libraries(TLX_CUDA_TEST
            PRIVATE
            TLX
    )

endif()