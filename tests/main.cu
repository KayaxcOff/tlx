//
// Created by muham on 15.07.2026.
//

#include <tlx/string.hpp>
#include <cuda_runtime.h>
#include <iostream>

using tlx::vstring;

__global__ void test_vstring_kernel(const char* safe_device_data) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;

    if (tid == 0) {
        printf("=== vstring test on GPU has started. ===\n");

        const vstring v1(safe_device_data, 5);

        const vstring v2(safe_device_data, 11);
        const vstring v3 = v2.substr(6, 5);

        printf("v1 size: %llu\n", static_cast<std::uint64_t>(v1.size()));
        printf("v3 size: %llu\n", static_cast<std::uint64_t>(v3.size()));

        vstring v4("World");

        if (v3 == v4) {
            printf("Successful: v3 ('World') and v4 ('World') are equal!\n");
        } else {
            printf("Error: Equality could not be established!\n");
        }

        vstring empty_str;
        if (empty_str.empty()) {
            printf("Successful: Empty string was detected as empty.\n");
        }
    }
}

int main() {
    char* shared_data = nullptr;
    cudaMallocManaged(&shared_data, 12 * sizeof(char));

    std::strcpy(shared_data, "Hello World");

    test_vstring_kernel<<<1, 1>>>(shared_data);

    cudaError_t err = cudaDeviceSynchronize();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Error: " << cudaGetErrorString(err) << std::endl;
    } else {
        std::cout << "\nBy the host (CPU): GPU test completed successfully!" << std::endl;
    }

    cudaFree(shared_data);
    return 0;
}
