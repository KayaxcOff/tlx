# TLX

> A modern C++20 utility library designed for high-performance applications.

TLX is a lightweight and modular C++20 library that provides a collection of utilities, containers, numeric types, memory tools, and type traits. It is designed as a modern foundation for larger projects while remaining independent and easy to integrate into existing codebases.

Originally developed as the core utility library for **CortexMind**, TLX focuses on clean APIs, zero-overhead abstractions, and modern C++ design principles.

---

## Features

* Modern **C++20** implementation
* Header-first modular design
* Lightweight with minimal dependencies
* STL-inspired interfaces
* High-performance memory utilities
* Generic programming using concepts
* Numeric helper functions
* Custom containers
* Reference wrapper utilities
* Platform-independent design

### Numeric Types

TLX includes support for several specialized numeric formats commonly used in machine learning and high-performance computing:

* `half` (16-bit floating point)
* `bfloat16`
* Quantized integer types
* Standard integer and floating-point aliases

These types are designed to simplify AI and numerical applications while maintaining a clean C++ interface.

---

## Components

The library currently includes modules such as:

* Memory utilities
* Type traits
* Concepts
* Containers
* References
* Numeric utilities
* File utilities
* Allocators
* Timing utilities
* Mathematical helpers

Example components include:

* `tlx::span`
* `tlx::vec`
* `tlx::reference`
* `tlx::ref`
* `tlx::cref`
* Memory alignment helpers
* `addressOf()`
* `move()`
* `swap()`
* Range and sign utilities

---

## Design Goals

TLX follows several core principles:

* **Zero-overhead abstractions**
* **Readable APIs**
* **Performance-first implementation**
* **Minimal dynamic allocation**
* **Modern C++20 features**
* **Easy integration**
* **Cross-platform compatibility**

---

## Example

```cpp
#include <tlx/span.hpp>
#include <vector>

int main() {
    std::vector<int> values = {1, 2, 3, 4};

    tlx::span<int> view(values.data(), values.size());

    for (auto& value : view) {
        value *= 2;
    }
}
```

---

## Building

TLX uses **CMake**.

```bash
git clone https://github.com/KayaxcOff/TLX.git

cd TLX

cmake -B build

cmake --build build
```

---

## Integration

Using CMake:

```cmake
FetchContent_Declare(
    TLX
    GIT_REPOSITORY https://github.com/KayaxcOff/TLX.git
    GIT_TAG main
)

FetchContent_MakeAvailable(TLX)

target_link_libraries(MyProject PRIVATE TLX)
```

---

## Why TLX?

While the C++ Standard Library already provides excellent building blocks, TLX aims to offer:

* More consistent APIs
* Additional utility components
* Specialized numeric types
* Machine-learning friendly data types
* Convenience utilities frequently needed in modern C++ projects

Rather than replacing the STL, TLX is designed to complement it.

---

## Roadmap

Planned and ongoing development includes:

* Additional containers
* SIMD utilities
* Advanced memory management
* Extended filesystem utilities
* More numerical algorithms
* Better benchmarking tools
* Serialization utilities
* Expanded AI-oriented numeric types

---

## Used In

TLX serves as the foundation utility library for the **CortexMind** machine learning framework.

---

## Contributing

Contributions, bug reports, feature requests, and discussions are always welcome.

Feel free to open an issue or submit a pull request.

---

## License

This project is licensed under the MIT License.
