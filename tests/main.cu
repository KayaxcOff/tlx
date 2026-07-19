//
// Created by muham on 15.07.2026.
//

#include "tlx/random.hpp"
#include "tlx/types.hpp"
#include <cuda_runtime.h>
#include <cstdio>
#include <cassert>
#include <cmath>

using tlx::philox4x32;
using tlx::bfloat16;

constexpr int N = 8;

__global__ void kernel(std::uint64_t seed,
                        float* uf, float* ubf, float* uh,
                        float* nf, double* nd, float* nbf, float* nh) {
    printf("[KERNEL] basladi\n");

    philox4x32 r1(seed);
    for (int i = 0; i < N; ++i) uf[i] = r1.uniform<float>();
    printf("[KERNEL] uf tamam\n");

    philox4x32 r2(seed);
    for (int i = 0; i < N; ++i) {
        printf("[KERNEL] index\n");
        ubf[i] = static_cast<float>(r2.uniform<float>());
    }
    printf("[KERNEL] ubf tamam\n");

    philox4x32 r3(seed);
    for (int i = 0; i < N; ++i) uh[i] = static_cast<float>(r3.uniform<tlx::half>());
    printf("[KERNEL] uh tamam\n");

    philox4x32 r4(seed);
    for (int i = 0; i < N; ++i) nf[i] = r4.normal<float>();
    printf("[KERNEL] nf tamam\n");

    philox4x32 r5(seed);
    for (int i = 0; i < N; ++i) nd[i] = r5.normal<double>();
    printf("[KERNEL] nd tamam\n");

    philox4x32 r6(seed);
    for (int i = 0; i < N; ++i) nbf[i] = static_cast<float>(r6.normal<bfloat16>());
    printf("[KERNEL] nbf tamam\n");

    philox4x32 r7(seed);
    for (int i = 0; i < N; ++i) nh[i] = static_cast<float>(r7.normal<tlx::half>());
    printf("[KERNEL] nh tamam\n");

    printf("[KERNEL] bitti\n");
}

void run_host(std::uint64_t seed,
              float* uf, float* ubf, float* uh,
              float* nf, double* nd, float* nbf, float* nh) {
    philox4x32 r1(seed);
    for (int i = 0; i < N; ++i) uf[i] = r1.uniform<float>();

    philox4x32 r2(seed);
    for (int i = 0; i < N; ++i) ubf[i] = static_cast<float>(r2.uniform<bfloat16>());

    philox4x32 r3(seed);
    for (int i = 0; i < N; ++i) uh[i] = static_cast<float>(r3.uniform<tlx::half>());

    philox4x32 r4(seed);
    for (int i = 0; i < N; ++i) nf[i] = r4.normal<float>();

    philox4x32 r5(seed);
    for (int i = 0; i < N; ++i) nd[i] = r5.normal<double>();

    philox4x32 r6(seed);
    for (int i = 0; i < N; ++i) nbf[i] = static_cast<float>(r6.normal<bfloat16>());

    philox4x32 r7(seed);
    for (int i = 0; i < N; ++i) nh[i] = static_cast<float>(r7.normal<tlx::half>());
}

int main() {
    constexpr std::uint64_t seed = 123;

    float h_uf[N], h_ubf[N], h_uh[N], h_nf[N], h_nbf[N], h_nh[N];
    double h_nd[N];
    run_host(seed, h_uf, h_ubf, h_uh, h_nf, h_nd, h_nbf, h_nh);

    float *d_uf, *d_ubf, *d_uh, *d_nf, *d_nbf, *d_nh;
    double* d_nd;
    cudaMalloc(&d_uf, N * sizeof(float));
    cudaMalloc(&d_ubf, N * sizeof(float));
    cudaMalloc(&d_uh, N * sizeof(float));
    cudaMalloc(&d_nf, N * sizeof(float));
    cudaMalloc(&d_nd, N * sizeof(double));
    cudaMalloc(&d_nbf, N * sizeof(float));
    cudaMalloc(&d_nh, N * sizeof(float));

    kernel<<<1, 1>>>(seed, d_uf, d_ubf, d_uh, d_nf, d_nd, d_nbf, d_nh);
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("Kernel launch basarisiz: %s\n", cudaGetErrorString(err));
        return 1;
    }
    err = cudaDeviceSynchronize();   // <-- BUNU KONTROL ETMEMİŞTİM
    if (err != cudaSuccess) {
        printf("Kernel CALISMA ZAMANI hatasi: %s\n", cudaGetErrorString(err));
        return 1;
    }

    float g_uf[N], g_ubf[N], g_uh[N], g_nf[N], g_nbf[N], g_nh[N];
    double g_nd[N];
    err = cudaMemcpy(g_uf, d_uf, N * sizeof(float), cudaMemcpyDeviceToHost);
    printf("%s\n", cudaGetErrorString(err));
    err = cudaMemcpy(g_ubf, d_ubf, N * sizeof(float), cudaMemcpyDeviceToHost);
    printf("%s\n", cudaGetErrorString(err));
    err = cudaMemcpy(g_uh, d_uh, N * sizeof(float), cudaMemcpyDeviceToHost);
    printf("%s\n", cudaGetErrorString(err));
    err = cudaMemcpy(g_nf, d_nf, N * sizeof(float), cudaMemcpyDeviceToHost);
    printf("%s\n", cudaGetErrorString(err));
    err = cudaMemcpy(g_nd, d_nd, N * sizeof(double), cudaMemcpyDeviceToHost);
    printf("%s\n", cudaGetErrorString(err));
    err = cudaMemcpy(g_nbf, d_nbf, N * sizeof(float), cudaMemcpyDeviceToHost);
    printf("%s\n", cudaGetErrorString(err));
    err = cudaMemcpy(g_nh, d_nh, N * sizeof(float), cudaMemcpyDeviceToHost);
    printf("%s\n", cudaGetErrorString(err));
    cudaFree(d_uf); cudaFree(d_ubf); cudaFree(d_uh);
    cudaFree(d_nf); cudaFree(d_nd); cudaFree(d_nbf); cudaFree(d_nh);

    bool ok = true;
    auto check = [&](const char* name, float h, float d, float tol) {
        bool match = std::fabs(h - d) <= tol;
        printf("[%-18s] host=%12.6f device=%12.6f %s\n", name, h, d, match ? "OK" : "UYUSMUYOR");
        if (!match) ok = false;
    };

    printf("== uniform<float/bfloat16/half> (host vs device) ==\n");
    for (int i = 0; i < N; ++i) {
        assert(h_uf[i] >= 0.0f && h_uf[i] < 1.0f);
        assert(h_ubf[i] >= 0.0f && h_ubf[i] < 1.0f);
        assert(h_uh[i] >= 0.0f && h_uh[i] < 1.0f);
        check("uniform<float>", h_uf[i], g_uf[i], 1e-9f);
        check("uniform<bfloat16>", h_ubf[i], g_ubf[i], 1e-9f);
        check("uniform<half>", h_uh[i], g_uh[i], 1e-9f);
    }

    printf("\n== normal<float/double/bfloat16/half> (host vs device) ==\n");
    for (int i = 0; i < N; ++i) {
        check("normal<float>", h_nf[i], g_nf[i], 1e-6f);
        check("normal<double>", static_cast<float>(h_nd[i]), static_cast<float>(g_nd[i]), 1e-9f);
        check("normal<bfloat16>", h_nbf[i], g_nbf[i], 1e-9f);
        check("normal<half>", h_nh[i], g_nh[i], 1e-9f);
    }

    printf("\n%s\n", ok ? "TUM TESTLER BASARILI" : "BAZI TESTLER BASARISIZ");
    fflush(stdout);
    assert(ok);
    return 0;
}
