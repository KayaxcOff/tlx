//
// Created by muham on 15.07.2026.
//

#include <tlx/string.hpp>
#include <cuda_runtime.h>
#include <cstdio>
#include <cassert>

using tlx::vstring;

// ---------------------------------------------------------------------
// Device tarafi: kernel icinde vstring olusturma, data/size/begin/end,
// operator[], karsilastirma, substr, move semantigi testi.
// ---------------------------------------------------------------------
__global__ void vstring_kernel(char* shared_buffer, int* results) {
    // shared_buffer icinde "Hello, CUDA!" var (host'tan kopyalandi)
    vstring vs(shared_buffer, 12); // "Hello, CUDA!" -> 12 karakter

    results[0] = (vs.size() == 12) ? 1 : 0;
    results[1] = (!vs.empty()) ? 1 : 0;
    results[2] = (vs[0] == 'H') ? 1 : 0;
    results[3] = (vs[11] == '!') ? 1 : 0;

    // begin/end ile manuel dolasma (range-based for yerine, TLX_HD dogrulugu icin)
    int char_count = 0;
    for (char* it = vs.begin(); it != vs.end(); ++it) {
        char_count++;
    }
    results[4] = (char_count == 12) ? 1 : 0;

    // Karsilastirma
    vstring vs2(shared_buffer, 12);
    results[5] = (vs == vs2) ? 1 : 0;

    vstring vs3(shared_buffer, 5); // "Hello"
    results[6] = (vs != vs3) ? 1 : 0;

    // substr
    vstring hello = vs.substr(0, 5);
    results[7] = (hello.size() == 5 && hello[0] == 'H' && hello[4] == 'o') ? 1 : 0;

    // Move semantigi: tasima sonrasi kaynagin bosaltilmis olmasi
    vstring moved_from(shared_buffer, 12);
    vstring moved_to(static_cast<vstring&&>(moved_from));
    results[8] = (moved_to.size() == 12 && moved_to.data() == shared_buffer) ? 1 : 0;
    results[9] = (moved_from.size() == 0 && moved_from.data() == nullptr) ? 1 : 0;

    // operator[] ile yazma (non-const)
    char local_buf[] = "abc";
    vstring writable(local_buf, 3);
    writable[0] = 'X';
    results[10] = (local_buf[0] == 'X') ? 1 : 0;
}

void run_device_tests() {
    std::printf("== DEVICE (CUDA KERNEL) TESTLERI (vstring) ==\n");

    const char host_text[] = "Hello, CUDA!"; // 12 karakter (null terminator haric)
    char* d_buffer = nullptr;
    cudaError_t err = cudaMalloc(&d_buffer, sizeof(host_text));
    assert(err == cudaSuccess);
    cudaMemcpy(d_buffer, host_text, sizeof(host_text), cudaMemcpyHostToDevice);

    constexpr int N = 11;
    int* d_results = nullptr;
    err = cudaMalloc(&d_results, N * sizeof(int));
    assert(err == cudaSuccess);

    vstring_kernel<<<1, 1>>>(d_buffer, d_results);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::printf("Kernel launch basarisiz: %s\n", cudaGetErrorString(err));
        std::exit(1);
    }
    cudaDeviceSynchronize();

    int h_results[N];
    cudaMemcpy(h_results, d_results, N * sizeof(int), cudaMemcpyDeviceToHost);
    cudaFree(d_buffer);
    cudaFree(d_results);

    const char* labels[N] = {
        "size() == 12", "!empty()", "vs[0] == 'H'", "vs[11] == '!'",
        "begin/end dolasma (12 karakter)", "vs == vs2 (ayni veri)",
        "vs != vs3 (farkli boyut)", "substr(0,5) == \"Hello\"",
        "move sonrasi hedef gecerli", "move sonrasi kaynak bosaltildi",
        "operator[] ile yazma calisti"
    };

    bool all_ok = true;
    for (int i = 0; i < N; ++i) {
        std::printf("[%2d] %-38s = %s\n", i, labels[i], h_results[i] ? "OK" : "BASARISIZ");
        if (!h_results[i]) all_ok = false;
    }

    assert(all_ok);
    std::printf("Device testleri BASARILI\n\n");
}

int main() {
    run_device_tests();
    std::printf("TUM TESTLER (vstring, CUDA) BASARILI\n");
    return 0;
}
