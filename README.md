# 🚀 TLX

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-20-blue.svg?style=flat-for-the-badge&logo=c%2B%2B" alt="C++20" />
  <img src="https://img.shields.io/badge/CUDA-Enabled-green.svg?style=flat-for-the-badge&logo=nvidia" alt="CUDA Support" />
  <img src="https://img.shields.io/badge/Build-CMake-orange.svg?style=flat-for-the-badge&logo=cmake" alt="CMake" />
  <img src="https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-for-the-badge" alt="MIT License" />
</p>

TLX, modern **C++20** standardı ile sıfırdan inşa edilmiş, yüksek performanslı, modüler ve hafif bir yardımcı kütüphanedir (utility library). Büyük ölçekli ve performans kritik uygulamalar (özellikle makine öğrenimi ve HPC) için sıfır maliyetli soyutlamalar (zero-overhead abstractions) sunar.

Aslen **CortexMind** makine öğrenimi çerçevesinin (framework) çekirdek kütüphanesi olarak geliştirilen TLX, harici hiçbir bağımlılığa ihtiyaç duymadan mevcut C++ projelerinize kolayca entegre edilebilir.

---

## ✨ Öne Çıkan Özellikler

* **⚡ Modern C++20 Standardı:** Konseptler (Concepts), `std::format` benzeri güvenli biçimlendirme ve modern bellek yönetimi.
* **🧠 Yapay Zeka Odaklı Sayısal Tipler:** CPU ve GPU (CUDA) üzerinde uyumlu çalışabilen `bfloat16`, `half` (fp16) ve kuantize tamsayı (`qint8`, `quint8`, `qint16`, `quint16`) destekleri.
* **📦 Hafif & Bağımsız:** Herhangi bir dış bağımlılık barındırmayan, header-first yaklaşımlı modüler yapı.
* **🛸 CUDA Desteği:** `TLX_HAS_CUDA` makrosu yardımıyla host (CPU) ve device (GPU) üzerinde kesintisiz çalışabilen fonksiyonlar (`TLX_HD`).
* **📁 Gelişmiş Girdi/Çıktı (I/O) ve Dosya Sistemi:** Kolaylaştırılmış `vstring`, `File` işlemleri ve hızlı serileştirme için ikili okuyucu/yazıcı (`BinaryReader` / `BinaryWriter`).
* **⏱️ Zamanlama & Profil Çıkarma:** Mikrosaniye ve nanosaniye hassasiyetinde çalışan `StopWatch` aracı.

---

## 🛠️ Temel Bileşenler ve Örnek Kodlar

### 1. Bellek Görünümleri ve `Span` (Bellek Kopyalamasız Erişim)
`tlx::Span`, `std::span` benzeri, sürekli bir bellek bloğu üzerinde kopyalama maliyeti olmaksızın hafif bir görünüm (view) sunar.

```cpp
#include <tlx/span.hpp>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> veriler = {10, 20, 30, 40};

    // Vektör verileri üzerinde sıfır maliyetli bir görünüm oluşturuyoruz
    tlx::Span<int> gorunum(veriler.data(), veriler.size());

    // Elemanları ikiye katla
    for (auto& deger : gorunum) {
        deger *= 2;
    }

    std::cout << "Guncellenmis ilk eleman: " << gorunum[0] << std::endl; // Çıktı: 20
}
```

### 2. Yapay Zeka ve Numerik Tipler (`bfloat16` & `half`)
Yapay zeka modellerinde bellek kullanımını azaltmak ve işlem hızını artırmak amacıyla kullanılan 16-bit kayan nokta tipleri.

```cpp
#include <tlx/types.hpp>
#include <iostream>

int main() {
    // 32-bit float değerden bfloat16 oluşturma
    tlx::bfloat16 fp_bfloat = 3.14159f;
    tlx::half fp_half = 2.71828f;

    // Temel aritmetik işlemler
    tlx::bfloat16 sonuc = fp_bfloat + tlx::bfloat16(1.0f);

    std::cout << "bfloat16 Degeri: " << float(sonuc) << std::endl;
    std::cout << "half Degeri: " << float(fp_half) << std::endl;
}
```

### 3. Yüksek Performanslı İkili Dosya İşlemleri (`BinaryWriter` & `BinaryReader`)
Trivially copyable (doğrudan kopyalanabilir) verilerin hızlıca diskten okunup yazılmasını sağlar.

```cpp
#include <tlx/file_system.hpp>
#include <iostream>

struct ModelAgirliklari {
    float bias;
    int katman_id;
};

int main() {
    const std::string dosya_yolu = "model.bin";

    // 1. Dosyaya ikili (binary) olarak veri yazma
    {
        tlx::fs::BinaryWriter writer(dosya_yolu);
        ModelAgirliklari agirlik = {0.85f, 42};
        writer.write(agirlik);
    }

    // 2. Dosyadan ikili (binary) olarak veri okuma
    {
        tlx::fs::BinaryReader reader(dosya_yolu);
        auto okunan_agirlik = reader.read<ModelAgirliklari>();

        std::cout << "Bias: " << okunan_agirlik.bias
                  << ", Katman ID: " << okunan_agirlik.katman_id << std::endl;
    }
}
```

### 4. Hassas Zaman Ölçümü (`StopWatch`)
Kod bloklarınızın ne kadar sürede çalıştığını en yüksek hassasiyetle ölçün.

```cpp
#include <tlx/time.hpp>
#include <thread>
#include <iostream>

int main() {
    tlx::StopWatch kronometre;

    // Ölçülmek istenen işlem (örneğin 50 milisaniye uyuma)
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    auto gecen_sure = kronometre.elapsed<tlx::microseconds>();
    std::cout << "Gecen sure: " << gecen_sure << " mikrosaniye." << std::endl;
}
```

---

## 🏗️ Derleme ve Test Etme

TLX, modern **CMake** yapılandırma sistemini kullanmaktadır.

### Gereksinimler
* C++20 destekleyen modern bir derleyici (GCC 11+, Clang 13+, MSVC 2022+)
* CMake (Sürüm 3.20 ve üzeri önerilir)
* NVIDIA CUDA Toolkit (İsteğe bağlı, GPU desteği için)

### Local Olarak Derleme ve Testleri Çalıştırma

```bash
# Projeyi klonlayın
git clone https://github.com/KayaxcOff/TLX.git
cd TLX

# CMake yapılandırmasını oluşturun
cmake -B build

# Kütüphaneyi ve testleri derleyin
cmake --build build

# Testleri çalıştırın
./build/TLX_HOST_TEST
```

---

## 📦 Projenize Dahil Etme (CMake Integration)

TLX kütüphanesini `FetchContent` kullanarak kendi CMake projenize tek bir adımda ekleyebilirsiniz:

```cmake
include(FetchContent)

FetchContent_Declare(
    TLX
    GIT_REPOSITORY https://github.com/KayaxcOff/TLX.git
    GIT_TAG        main # Belirli bir commit hash veya tag de kullanabilirsiniz
)

FetchContent_MakeAvailable(TLX)

# Kendi hedefinize (target) TLX kütüphanesini bağlayın
target_link_libraries(KendiUygulamaniz PRIVATE TLX)
```

---

## 🗺️ Gelecek Yol Haritası (Roadmap)

- [ ] Gelişmiş SIMD (AVX2 / AVX-512) entegrasyonu
- [ ] Daha fazla yapay zeka odaklı veri yapıları ve tensör yardımcıları
- [ ] Genişletilmiş dosya sistemi ve JSON/YAML serileştirme desteği
- [ ] Detaylı Benchmark (başarım ölçüm) testleri

---

## 🛡️ Lisans

Bu proje **MIT Lisansı** altında lisanslanmıştır. Daha fazla bilgi için `LICENSE` dosyasına göz atabilirsiniz.
