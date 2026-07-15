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

    class BinaryWriter {
    public:
        explicit BinaryWriter(const std::string& path);
        BinaryWriter(const BinaryWriter&) = delete;
        BinaryWriter(BinaryWriter&&) noexcept = default;
        ~BinaryWriter();

        template<typename T>
        void write(const T& value) {
            static_assert(std::is_trivially_copyable_v<T>, "It only works with trivially copyable types.");

            std::byte buffer[sizeof(T)];
            std::memcpy(buffer, &value, sizeof(T));

            this->m_file.write(reinterpret_cast<const char*>(buffer), sizeof(T));
            detail::exitIf(!this->m_file.good(), "Write has failed");
        }

        template<typename T>
        void write_array(const T* data, const std::size_t count) {
            for (std::size_t i = 0; i < count; ++i) {
                write(data[i]);
            }
        }

        void write_raw(const void* data, std::size_t size);

        [[nodiscard]]
        std::size_t tell();

        BinaryWriter& operator=(const BinaryWriter&) = delete;
        BinaryWriter& operator=(BinaryWriter&&) noexcept = default;
    private:
        std::ofstream m_file;
    };

    class BinaryReader {
    public:
        explicit BinaryReader(const std::string& path);
        BinaryReader(const BinaryReader&) = delete;
        BinaryReader(BinaryReader&&) noexcept = default;
        ~BinaryReader();

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

        template<typename T>
        void read_array(T* out, const std::size_t count) {
            for (std::size_t i = 0; i < count; ++i) {
                out[i] = read<T>();
            }
        }

        void read_raw(void* out, std::size_t size);

        [[nodiscard]]
        bool eof() const;

        [[nodiscard]]
        std::size_t tell();

        BinaryReader& operator=(const BinaryReader&) = delete;
        BinaryReader& operator=(BinaryReader&&) noexcept = default;

    private:
        std::ifstream m_file;
    };
} //namespace tlx::fs

#endif //TLX_FILE_SYSTEM_HPP