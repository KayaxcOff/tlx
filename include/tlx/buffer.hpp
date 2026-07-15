//
// Created by muham on 15.07.2026.
//

#ifndef TLX_BUFFER_HPP
#define TLX_BUFFER_HPP

#include <cstddef>

namespace tlx {
    /**
     * @brief A non-owning view over a contiguous sequence of bytes.
     *
     * This is a lightweight wrapper similar to `std::span<std::byte>` that provides
     * type-safe access to raw memory buffers through templated `as<T>()` methods.
     */
    struct BufferView {
        /**
         * @brief Constructs an empty buffer view.
         */
        BufferView();
        /**
         * @brief Constructs a buffer view from mutable data.
         * @param data Pointer to the start of the buffer.
         * @param size Size of the buffer in bytes.
         */
        explicit BufferView(std::byte* data, std::size_t size);
        /**
         * @brief Constructs a buffer view from const data.
         * @param data Pointer to the start of the buffer.
         * @param size Size of the buffer in bytes.
         */
        BufferView(const std::byte* data, std::size_t size);
        BufferView(const BufferView& other);
        BufferView(BufferView&& other) noexcept;
        ~BufferView();

        /**
         * @brief Reinterprets the buffer as a pointer to type `T`.
         *
         * @tparam T Type to reinterpret the buffer as.
         * @return Pointer to `T` at the start of the buffer.
         */
        template<typename T>
        [[nodiscard]]
        T* as() {
            return reinterpret_cast<T*>(this->m_data);
        }
        /**
         * @brief Reinterprets the buffer as a const pointer to type `T`.
         *
         * @tparam T Type to reinterpret the buffer as.
         * @return Const pointer to `T` at the start of the buffer.
         */
        template<typename T>
        [[nodiscard]]
        const T* as() const {
            return reinterpret_cast<const T*>(this->m_data);
        }
        /**
         * @brief Returns the size of the buffer in bytes.
         * @return Size in bytes.
         */
        [[nodiscard]]
        std::size_t size() const;

        BufferView& operator=(const BufferView& other);
        BufferView& operator=(BufferView&& other) noexcept;
    private:
        std::byte* m_data;
        std::size_t m_size;
    };
} //namespace tlx

#endif //TLX_BUFFER_HPP