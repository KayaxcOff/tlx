//
// Created by muham on 15.07.2026.
//

#include "tlx/file_system.hpp"
#include <tlx/utility.hpp>
#include <fstream>

using namespace tlx::fs;
using namespace tlx;

bool fs::exists(const path &path) {
    return std::filesystem::exists(path);
}

bool fs::isDirectory(const path &path) {
    return std::filesystem::is_directory(path);
}

bool fs::remove(const path &path) {
    return std::filesystem::remove(path);
}

bool fs::CreateDirectory(const path &path) {
    return std::filesystem::create_directory(path);
}

File::File() {
    this->m_path = "";
}

File::File(const ::path &path) {
    this->m_path = path;
}

File::File(const File &other) {
    this->m_path = other.m_path;
}

File::File(File &&other) noexcept {
    this->m_path = ::tlx::move(other.m_path);
}

File::~File() = default;

const path &File::path() const {
    return this->m_path;
}

bool File::exists() const {
    return fs::exists(this->m_path);
}

Status File::read(std::string &output) const {
    const std::ifstream stream(m_path, std::ios::binary);
    if (!stream) {
        return FileNotFound("Cannot open file");
    }

    std::ostringstream buffer;
    buffer << stream.rdbuf();

    if (!stream.good() && !stream.eof()) {
        return IO("Failed to read file");
    }

    output = std::move(buffer).str();
    return Success();
}

Status File::write(const std::string_view input) const {
    std::ofstream stream(m_path, std::ios::binary);
    if (!stream) {
        return FileNotFound("Cannot open file");
    }

    stream.write(input.data(), static_cast<std::streamsize>(input.size()));

    if (!stream) {
        return IO("Failed to write file");
    }

    return Success();
}

BinaryWriter::BinaryWriter(const std::string &path) {
    this->m_file = std::ofstream(path, std::ios::binary | std::ios::out | std::ios::trunc);
    detail::exitIf(!this->m_file.is_open(), "Cannot open file");
}

BinaryWriter::~BinaryWriter() {
    if (this->m_file.is_open()) {
        this->m_file.close();
    }
}

void BinaryWriter::write_raw(const void *data, const std::size_t size) {
    this->m_file.write(static_cast<const char*>(data), static_cast<std::streamsize>(size));
    detail::exitIf(!this->m_file.good(), "Write raw failed");
}

std::size_t BinaryWriter::tell() {
    return this->m_file.tellp();
}

BinaryReader::BinaryReader(const std::string &path) {
    this->m_file = std::ifstream(path, std::ios::binary);
    detail::exitIf(!this->m_file.is_open(), "Cannot open file");
}

BinaryReader::~BinaryReader() {
    if (this->m_file.is_open()) {
        this->m_file.close();
    }
}

void BinaryReader::read_raw(void *out, const std::size_t size) {
    this->m_file.read(static_cast<char*>(out), static_cast<std::streamsize>(size));
    detail::exitIf(!this->m_file.good(), "BinaryReader: RAW read failed");
}

bool BinaryReader::eof() const {
    return this->m_file.eof();
}

std::size_t BinaryReader::tell() {
    return this->m_file.tellg();
}