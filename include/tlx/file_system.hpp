//
// Created by muham on 15.07.2026.
//

#ifndef TLX_FILE_SYSTEM_HPP
#define TLX_FILE_SYSTEM_HPP

#include <tlx/errors.hpp>
#include <filesystem>

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
} //namespace tlx::fs

#endif //TLX_FILE_SYSTEM_HPP