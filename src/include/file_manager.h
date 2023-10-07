#pragma once

#include <string>

class FileManager {
public:
    FileManager(const FileManager &) = delete;
    FileManager &operator=(const FileManager &) = delete;
    FileManager(FileManager &&) = delete;
    FileManager &operator=(FileManager &&) = delete;

    virtual int prepare_file(size_t &size, char **out) = 0;
    virtual int close_file(char *ptr, size_t actual_size) = 0;
    virtual ~FileManager() = default;

protected:
    explicit FileManager(std::string filename);
    std::string m_filename;
};

class FileReader : public FileManager {
public:
    explicit FileReader(std::string filename)
            : FileManager(filename) {
    }
    FileReader(const FileReader &) = delete;
    FileReader &operator=(const FileReader &) = delete;
    FileReader(FileReader &&) = delete;
    FileReader &operator=(FileReader &&) = delete;

    int prepare_file(size_t &size, char **out) override;
    int close_file(char *ptr, size_t actual_size) override;
    ~FileReader() override;

private:
    int m_fd = -1;
};

class FileWriter : public FileManager {
public:
    explicit FileWriter(const std::string &filename)
            : FileManager(filename) {
    }
    FileWriter(const FileWriter &) = delete;
    FileWriter &operator=(const FileWriter &) = delete;
    FileWriter(FileWriter &&) = delete;
    FileWriter &operator=(FileWriter &&) = delete;

    int prepare_file(size_t &size, char **out) override;
    int write_to_file(char *ptr, const std::string &data);
    int resize_file(char *ptr, size_t old_size, size_t new_size);
    int close_file(char *ptr, size_t actual_size) override;
    ~FileWriter() override;

private:
    int m_fd = -1;
};
