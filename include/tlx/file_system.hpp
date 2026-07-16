//
// Created by muham on 15.07.2026.
//

#ifndef TLX_FILE_SYSTEM_HPP
#define TLX_FILE_SYSTEM_HPP

#include <tlx/errors.hpp>
#include <filesystem>
#include <fstream>

namespace tlx::fs {
    using std::filesystem::path;

    /**
     * @brief Checks whether a path exists on the filesystem.
     *
     * @param path The path to check.
     * @return `true` if the path exists (file, directory, or symlink), otherwise `false`.
     */
    [[nodiscard]]
    bool exists(const path& path);
    /**
     * @brief Checks whether the given path points to a directory.
     *
     * @param path The path to check.
     * @return `true` if the path exists and is a directory.
     */
    [[nodiscard]]
    bool isDirectory(const path& path);

    /**
     * @brief Removes a file or an empty directory.
     *
     * @param path The path to remove.
     * @return `true` if the file/directory was successfully removed.
     */
    [[nodiscard]]
    bool remove(const path& path);

    /**
     * @brief Creates a new directory.
     *
     * @param path The path of the directory to create.
     * @return `true` if the directory was successfully created.
     */
    [[nodiscard]]
    bool CreateDirectory(const path& path);

    /**
     * @brief Represents a file on the filesystem.
     *
     * This class provides simple read/write operations and basic file information.
     */
    class File {
    public:
        /**
         * @brief Constructs an empty (invalid) file object.
         */
        File();
        /**
         * @brief Constructs a File object from a path.
         * @param path Path to the file.
         */
        explicit File(const path& path);
        File(const File& other);
        File(File&& other) noexcept;
        ~File();

        /**
         * @brief Returns the path of this file.
         * @return Reference to the stored path.
         */
        [[nodiscard]]
        const path& path() const;
        /**
         * @brief Checks whether the file exists on the filesystem.
         * @return `true` if the file exists.
         */
        [[nodiscard]]
        bool exists() const;

        /**
         * @brief Reads the entire content of the file into a string.
         *
         * @param output String that will receive the file content.
         * @return `Status` indicating success or failure.
         */
        [[nodiscard]]
        Status read(std::string& output) const;
        /**
         * @brief Writes the given data to the file (overwrites existing content).
         *
         * @param input Data to write to the file.
         * @return `Status` indicating success or failure.
         */
        [[nodiscard]]
        Status write(std::string_view input) const;
    private:
        fs::path m_path;
    };

    /**
     * @brief Binary file writer for efficient serialization of binary data.
     *
     * This class provides a convenient interface for writing binary data to a file.
     * It supports trivially copyable types and raw byte writing.
     */
    class BinaryWriter {
    public:
        /**
         * @brief Constructs a BinaryWriter and opens the specified file.
         *
         * @param path Path to the output file.
         */
        explicit BinaryWriter(const std::string& path);
        BinaryWriter(const BinaryWriter&) = delete;
        BinaryWriter(BinaryWriter&&) noexcept = default;
        ~BinaryWriter();

        /**
         * @brief Writes a single value to the file in binary format.
         *
         * @tparam T Must be a trivially copyable type.
         * @param value The value to write.
         */
        template<typename T>
        void write(const T& value) {
            static_assert(std::is_trivially_copyable_v<T>, "It only works with trivially copyable types.");

            std::byte buffer[sizeof(T)];
            std::memcpy(buffer, &value, sizeof(T));

            this->m_file.write(reinterpret_cast<const char*>(buffer), sizeof(T));
            detail::exitIf(!this->m_file.good(), "Write has failed");
        }

        /**
         * @brief Writes an array of values to the file.
         *
         * @tparam T Element type (must be trivially copyable).
         * @param data  Pointer to the array.
         * @param count Number of elements to write.
         */
        template<typename T>
        void write_array(const T* data, const std::size_t count) {
            for (std::size_t i = 0; i < count; ++i) {
                this->write(data[i]);
            }
        }

        /**
         * @brief Writes raw bytes to the file.
         *
         * @param data Pointer to the raw data.
         * @param size Number of bytes to write.
         */
        void write_raw(const void* data, std::size_t size);

        /**
         * @brief Returns the current write position in the file.
         * @return Current position (in bytes) from the beginning of the file.
         */
        [[nodiscard]]
        std::size_t tell();

        BinaryWriter& operator=(const BinaryWriter&) = delete;
        BinaryWriter& operator=(BinaryWriter&&) noexcept = default;
    private:
        std::ofstream m_file;
    };

    /**
     * @brief Binary file reader for efficient deserialization of binary data.
     *
     * This class provides a convenient interface for reading binary data from a file.
     * It supports trivially copyable types and raw byte reading.
     */
    class BinaryReader {
    public:
        /**
         * @brief Constructs a BinaryReader and opens the specified file.
         *
         * @param path Path to the input file.
         */
        explicit BinaryReader(const std::string& path);
        BinaryReader(const BinaryReader&) = delete;
        BinaryReader(BinaryReader&&) noexcept = default;
        ~BinaryReader();

        /**
         * @brief Reads a single value of type `T` from the file.
         *
         * @tparam T Must be a trivially copyable type.
         * @return The value read from the file.
         */
        template<typename T>
        [[nodiscard]] T read() {
            static_assert(std::is_trivially_copyable_v<T>, "BinaryReader::read It only works with trivially copyable types.");
            std::byte buffer[sizeof(T)];
            this->m_file.read(reinterpret_cast<char*>(buffer), sizeof(T));
            detail::exitIf(!this->m_file.good(), "BinaryReader: read failed (end of file / corrupt data)");

            T value;
            std::memcpy(&value, buffer, sizeof(T));
            return value;
        }

        /**
         * @brief Reads an array of values from the file.
         *
         * @tparam T Element type (must be trivially copyable).
         * @param out   Pointer to the output array.
         * @param count Number of elements to read.
         */
        template<typename T>
        void read_array(T* out, const std::size_t count) {
            for (std::size_t i = 0; i < count; ++i) {
                out[i] = read<T>();
            }
        }

        /**
         * @brief Reads raw bytes from the file.
         *
         * @param out  Pointer to the buffer where data will be written.
         * @param size Number of bytes to read.
         */
        void read_raw(void* out, std::size_t size);

        /**
         * @brief Checks if the end of the file has been reached.
         * @return `true` if EOF is reached.
         */
        [[nodiscard]]
        bool eof() const;

        /**
         * @brief Returns the current read position in the file.
         * @return Current position (in bytes) from the beginning of the file.
         */
        [[nodiscard]]
        std::size_t tell();

        BinaryReader& operator=(const BinaryReader&) = delete;
        BinaryReader& operator=(BinaryReader&&) noexcept = default;

    private:
        std::ifstream m_file;
    };
} //namespace tlx::fs

#endif //TLX_FILE_SYSTEM_HPP