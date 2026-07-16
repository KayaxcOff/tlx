//
// Created by muham on 15.07.2026.
//

#ifndef TLX_MEMORY_HPP
#define TLX_MEMORY_HPP

#include <tlx/concepts.hpp>
#include <tlx/errors.hpp>
#include <tlx/utility.hpp>
#include <cstddef>
#include <cstdint>

namespace tlx {
    /**
     * @brief Checks if two pointers point to the same address.
     *
     * @tparam Ty Type of the pointed objects.
     * @param x1 First pointer.
     * @param x2 Second pointer.
     * @return `true` if both pointers are equal.
     */
    template <class Ty>
    [[nodiscard]]
    constexpr bool isSameAddress(const Ty* x1, const Ty* x2) noexcept {
        return x1 == x2;
    }
    /**
     * @brief Checks if two references to refer to the same object.
     *
     * Uses `addressOf` to handle overloaded `operator&`.
     *
     * @tparam Ty Type of the objects.
     * @param x1 First object.
     * @param x2 Second object.
     * @return `true` if both references point to the same object.
     */
    template <class Ty>
    [[nodiscard]]
    constexpr bool isSameAddress(const Ty& x1, const Ty& x2) noexcept {
        return ::tlx::addressOf(x1) == ::tlx::addressOf(x2);
    }

    /**
     * @brief Checks whether a pointer is null.
     *
     * @tparam T Type of the pointer.
     * @param ptr The pointer to check.
     * @return `true` if the pointer is `nullptr`.
     */
    template<typename T>
    [[nodiscard]]
    constexpr bool isNull(T* ptr) noexcept {
        return ptr == nullptr;
    }

    /**
     * @brief Checks if a pointer is aligned to the specified alignment.
     *
     * @tparam Ty Type of the pointed object.
     * @param ptr       Pointer to check.
     * @param alignment Required alignment (must be power of 2).
     * @return `true` if the pointer is properly aligned.
     */
    template<class Ty>
    [[nodiscard]]
    constexpr bool isAligned(const Ty* ptr, const std::size_t alignment) noexcept {
        return (reinterpret_cast<std::uintptr_t>(ptr) % alignment) == 0;
    }
    /**
     * @brief Checks if an address is aligned to the specified alignment.
     */
    [[nodiscard]]
    bool isAligned(std::uintptr_t address, std::size_t alignment) noexcept;
    /**
     * @brief Rounds an address up to the next multiple of the alignment.
     *
     * @param value     Address or size to align.
     * @param alignment Alignment boundary (must be power of 2).
     * @return Aligned address.
     */
    [[nodiscard]]
    std::uintptr_t alignUp(std::uintptr_t value, std::size_t alignment) noexcept;
    /**
     * @brief Rounds an address down to the previous multiple of the alignment.
     * @param value     Address or size to align.
     * @param alignment Alignment boundary (must be power of 2).
     * @return Aligned address.
     */
    [[nodiscard]]
    std::uintptr_t alignDown(std::uintptr_t value, std::size_t alignment);

    /**
     * @brief Returns the distance between two pointers (in elements).
     *
     * @tparam Ty Type of the elements.
     * @param first Start pointer.
     * @param last  End pointer.
     * @return `last - first`.
     */
    template<class Ty>
    [[nodiscard]]
    constexpr std::ptrdiff_t distance(const Ty* first, const Ty* last) noexcept {
        return last - first;
    }

    /**
     * @brief Advances a pointer by a byte offset.
     *
     * @tparam Ty Type of the pointed object.
     * @param ptr    Base pointer.
     * @param offset Byte offset to add.
     * @return Pointer advanced by the given byte offset.
     */
    template<class Ty>
    [[nodiscard]]
    Ty* offset(Ty* ptr, const std::size_t offset) {
        return reinterpret_cast<Ty*>(reinterpret_cast<std::byte*>(ptr) + offset);
    }

    /**
     * @brief Allocates aligned memory.
     *
     * @param bytes     Number of bytes to allocate.
     * @param alignment Alignment requirement.
     * @return Pointer to allocated memory, or `nullptr` on failure.
     */
    [[nodiscard]]
    void* malloc(std::size_t bytes, std::size_t alignment);
    /**
     * @brief Frees memory previously allocated with `tlx::malloc`.
     *
     * @param ptr       Pointer to free.
     * @param alignment Alignment that was used during allocation.
     */
    void free(void* ptr, std::size_t alignment) noexcept;

    /**
     * @brief Constructs an object in-place using placement new.
     *
     * @tparam Ty   Type of the object to construct.
     * @tparam Args Types of constructor arguments.
     * @param ptr   Memory location to construct the object.
     * @param args  Arguments to forward to the constructor.
     * @return Pointer to the newly constructed object.
     */
    template<class Ty, class... Args>
    constexpr Ty* construct(Ty* ptr, Args&&... args) {
        return ::new(static_cast<void*>(ptr)) Ty(static_cast<Args&&>(args)...);
    }
    /**
     * @brief Calls the destructor of an object without deallocating memory.
     *
     * @tparam Ty Type of the object.
     * @param ptr Pointer to the object to destroy.
     */
    template<class Ty>
    constexpr void destroy(Ty* ptr) noexcept {
        ptr->~Ty();
    }

    /**
     * @brief A simple owning smart pointer
     *
     * This class manages a single dynamically allocated object with automatic
     * destruction and deallocation when it goes out of scope.
     *
     * @tparam T Type of the managed object.
     */
    template<typename T>
    class ptr {
    public:
        /**
         * @brief Constructs a null pointer.
         */
        ptr() noexcept {
            this->value = nullptr;
        }
        /**
         * @brief Constructs from a raw pointer (takes ownership).
         * @param ptr Raw pointer to take ownership of.
         */
        explicit ptr(T* ptr) noexcept {
            this->value = ptr;
        }
        /**
         * @brief Constructs and allocates + constructs the object in one step.
         *
         * @tparam Args Types of constructor arguments.
         * @param args Arguments forwarded to the constructor of `T`.
         */
        template<typename... Args> requires ::tlx::constructible_from<T, Args...>
        explicit ptr(Args&&... args) : value(nullptr) {
            this->value = static_cast<T*>(::operator new(sizeof(T)));
            ::tlx::construct(this->value, ::tlx::forward<Args>(args)...);
        }
        ptr(const ptr& other) = default;
        ptr(ptr&& other) noexcept = default;
        ~ptr() {
            if (this->value) {
                ::tlx::destroy(this->value);
                ::operator delete(this->value);
            }
        }

        /**
         * @brief Creates a `ptr` by casting from another pointer type.
         * @param x Value to reinterpret cast to `T*`.
         * @return New `ptr<T>` instance.
         */
        [[nodiscard]]
        static ptr cast(T x) {
            return ptr(reinterpret_cast<T*>(x));
        }

        /**
         * @brief Returns the underlying raw pointer.
         */
        [[nodiscard]]
        T* get() {
            return this->value;
        }
        /**
         * @brief Returns the underlying raw pointer (const version).
         */
        [[nodiscard]]
        const T* get() const {
            return this->value;
        }
        /**
         * @brief Destroys the managed object and releases ownership.
         */
        void reset() {
            ::tlx::destroy(this->value);
            ::operator delete(this->value);
            this->value = nullptr;
        }

        T* operator->() noexcept {
            return this->value;
        }
        T& operator*() noexcept {
            return *this->value;
        }
        explicit operator bool() const {
            return this->value != nullptr;
        }

        ptr& operator=(const ptr& other) = default;
        ptr& operator=(ptr&& other) noexcept = default;

        T& operator[](const std::size_t index) noexcept {
            return this->value[index];
        }
        const T& operator[](const std::size_t index) const noexcept {
            return this->value[index];
        }
    private:
        T* value;
    };

    /**
     * @brief A lightweight non-owning reference wrapper.
     *
     * This class acts like `std::reference_wrapper` but is lighter and provides
     * pointer-like access (`operator->` and `operator*`).
     *
     * @tparam T Type of the referenced object.
     */
    template <typename T>
    class reference {
    public:
        /**
         * @brief Constructs a reference wrapper from a lvalue.
         * @param t The object to reference.
         */
        explicit reference(T& t) : value(::tlx::addressOf(t)) {}
        constexpr reference(const reference&) noexcept = default;
        constexpr reference(reference&&) = default;
        ~reference() noexcept = default;

        /**
         * @brief Returns a reference to the wrapped object.
         * @return Reference to the underlying object.
         */
        [[nodiscard]]
        constexpr T& get() const noexcept {
            return *this->value;
        }
        /**
         * @brief Implicit conversion to `T&`.
         */
        [[nodiscard]]
        explicit constexpr operator T&() const noexcept {
            return *this->value;
        }
        /**
         * @brief Member access operator.
         * @return Pointer to the underlying object.
         */
        [[nodiscard]]
        constexpr T* operator->() const noexcept {
            return this->value;
        }
        /**
         * @brief Dereference operator.
         * @return Reference to the underlying object.
         */
        constexpr T& operator*() const noexcept {
            return *this->value;
        }

        constexpr reference& operator=(const reference& other) {
            if (this != &other) {
                this->value = other.value;
            }
            return *this;
        }
        constexpr reference& operator=(reference&& other) noexcept {
            if (this != &other) {
                this->value = ::tlx::move(other.value);
            }
            return *this;
        }
    private:
        T* value;
    };

    /**
     * @brief Creates a `reference<T>` wrapper for a lvalue.
     *
     * @tparam T Type of the object.
     * @param value The object to wrap.
     * @return A `reference<T>` object.
     */
    template<class T>
    [[nodiscard]]
    constexpr reference<T> ref(T& value) noexcept {
        return reference<T>(value);
    }
    /**
     * @brief Creates a `reference<const T>` wrapper for a const lvalue.
     *
     * @tparam T Type of the object.
     * @param value The const object to wrap.
     * @return A `reference<const T>` object.
     */
    template<class T>
    [[nodiscard]]
    constexpr reference<const T> cref(const T& value) noexcept {
        return reference<const T>(value);
    }

    /**
     * @brief Dereferences a pointer with null check.
     *
     * @tparam T Type of the pointed object.
     * @param ptr Pointer to dereference.
     * @return Reference to the object.
     * @throws tlx::Exception if `ptr` is `nullptr`.
     */
    template<typename T>
    [[nodiscard]]
    T& dereference(T* ptr) {
        if (ptr == nullptr) {
            throw Exception("dereference() called on nullptr");
        }
        return *ptr;
    }

    template <class T>
    void ref(const T&&) = delete;
    template <class T>
    void cref(const T&&) = delete;
} //namespace tlx

#endif //TLX_MEMORY_HPP