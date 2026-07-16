# TLX

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-20-blue.svg?style=flat-for-the-badge&logo=c%2B%2B" alt="C++20" />
  <img src="https://img.shields.io/badge/CUDA-Enabled-green.svg?style=flat-for-the-badge&logo=nvidia" alt="CUDA Support" />
  <img src="https://img.shields.io/badge/Build-CMake-orange.svg?style=flat-for-the-badge&logo=cmake" alt="CMake" />
  <img src="https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-for-the-badge" alt="MIT License" />
</p>

TLX is a high-performance, modular, and lightweight utility library built from scratch using the modern **C++20** standard. It offers zero-overhead abstractions for large-scale and performance-critical applications (particularly in machine learning and HPC).

Originally developed as the core library of the **CortexMind** machine learning framework, TLX can be easily integrated into your existing C++ projects without requiring any external dependencies.

---

## Key Features

* ** Modern C++20 Standard:** Concepts, safe formatting similar to `std::format`, and modern memory management.
* ** AI-Focused Numeric Types:** Support for `bfloat16`, `half` (fp16), and quantized integers (`qint8`, `quint8`, `qint16`, `quint16`) that work compatibly on both CPU and GPU (CUDA).
* ** Lightweight & Independent:** A modular, header-only structure with no external dependencies.
* ** CUDA Support: Functions (`TLX_HD`) capable of running seamlessly on both the host (CPU) and the device (GPU), utilizing the `TLX_HAS_CUDA` macro.
* ** Advanced Input/Output (I/O) and File System:** Streamlined `vstring`, `File` operations, and binary reader/writer (`BinaryReader` / `BinaryWriter`) for fast serialization.
* ** Timing & Profiling:** A `StopWatch` tool operating with microsecond and nanosecond precision.

---

##  Core Components and Sample Code

### 1. Memory Views and `Span` (Zero-Copy Memory Access)
`tlx::Span` provides a lightweight view over a contiguous memory block—similar to `std::span`—without the cost of copying.

```cpp
#include <tlx/span.hpp>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> x1 = {10, 20, 30, 40};

    We create a zero-cost view on vector data.
    tlx::Span<int> x2(x1.data(), x1.size());

    // Double the number of elements
    for (auto& item : x2) {
        item *= 2;
    }

    std::cout << "The first updated element: " << x2[0] << std::endl; // Output: 20
}
```

### 2. Artificial Intelligence and Numeric Types (`bfloat16` & `half`)
16-bit floating-point types used to reduce memory usage and increase processing speed in artificial intelligence models.

```cpp
#include <tlx/types.hpp>
#include <iostream>

int main() {
    // Creating a bfloat16 from a 32-bit float value
    tlx::bfloat16 fp_bfloat = 3.14159f;
    tlx::half fp_half = 2.71828f;

    // Basic arithmetic operations
    tlx::bfloat16 sonuc = fp_bfloat + tlx::bfloat16(1.0f);

    std::cout << "bfloat16 value: " << float(sonuc) << std::endl;
    std::cout << "half value: " << float(fp_half) << std::endl;
}
```

### 3. High-Performance Binary File Operations (`BinaryWriter` & `BinaryReader`)
It enables the rapid reading and writing of trivially copyable data from and to the disk.

```cpp
#include <tlx/file_system.hpp>
#include <iostream>

struct ModelWeights {
    float bias;
    int layer_id;
};

int main() {
    const std::string file_path = "model.bin";

    // 1. Writing data to a file in binary format
    {
        tlx::fs::BinaryWriter writer(file_path);
        ModelWeights weight = {0.85f, 42};
        writer.write(weight);
    }

    // 2. 2. Reading binary data from a file
    {
        tlx::fs::BinaryReader reader(file_path);
        auto reading_weight = reader.read<ModelWeights>();
        
        std::cout << "Bias: " << reading_weight.bias 
                  << ", Layer ID: " << reading_weight.katman_id << std::endl;
    }
}
```

### 4. High-Precision Timing (`StopWatch`)
Measure the execution time of your code blocks with the highest precision.

```cpp
#include <tlx/time.hpp>
#include <thread>
#include <iostream>

int main() {
    tlx::StopWatch sw;

    // The operation to be measured (e.g., sleeping for 50 milliseconds)
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    auto value = sw.elapsed<tlx::microseconds>();
    std::cout << "Value: " << value << " micro seconds" << std::endl;
}
```

---

## Compiling and Testing

TLX uses the modern **CMake** build system.

### Requirements
* A modern compiler supporting C++20 (GCC 11+, Clang 13+, MSVC 2022+)
* CMake (Version 3.20 or later recommended)
* NVIDIA CUDA Toolkit (Optional, for GPU support)

### Compiling and Running Tests Locally

```bash
# Clone the project
git clone https://github.com/KayaxcOff/TLX.git
cd TLX

# Generate the CMake configuration.
cmake -B build

# Compile the library and the tests.
cmake --build build

# Run the tests
./build/TLX_HOST_TEST
```

---

## Integrating into Your Project (CMake Integration)

You can add the TLX library to your own CMake project in a single step using `FetchContent`:

```cmake
include(FetchContent)

FetchContent_Declare(
    TLX
    GIT_REPOSITORY https://github.com/KayaxcOff/TLX.git
    GIT_TAG        main # You can also use a specific commit hash or tag
)

FetchContent_MakeAvailable(TLX)

# Link the TLX library to your own target
target_link_libraries(YourApp PRIVATE TLX)
```

---

## Future Roadmap

- [ ] More AI-focused data structures and tensor utilities
- [ ] Expanded file system and JSON/YAML serialization support
- [ ] Detailed benchmark tests

---

## License

This project is licensed under the **MIT License**. For more information, you can check the `LICENSE` file.
