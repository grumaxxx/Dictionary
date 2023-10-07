#pragma once

#include <fstream>
#include <string>

class IWriter {
public:
    virtual int prepare_file(size_t &size, char **out) = 0;
    virtual int write_to_file(char *ptr, const std::string &data) = 0;
    virtual int resize_file(char *ptr, size_t old_size, size_t new_size) = 0;
    virtual int close_file(char *ptr, size_t actual_size) = 0;
    virtual ~IWriter() = default;
};

class MMapFileWriter : public IWriter {
public:
    explicit MMapFileWriter(std::string filename)
            : m_filename(std::move(filename)) {
    }
    int prepare_file(size_t &size, char **out) override;
    int write_to_file(char *ptr, const std::string &data) override;
    int resize_file(char *ptr, size_t old_size, size_t new_size) override;
    int close_file(char *ptr, size_t actual_size) override;
    ~MMapFileWriter() override;

private:
    std::string m_filename;
    int m_fd = -1;
};

class FStreamWriter : public IWriter {
public:
    explicit FStreamWriter(std::string filename)
            : m_filename(std::move(filename)) {
    }
    int prepare_file(size_t &size, char **out) override;
    int write_to_file(char *ptr, const std::string &data) override;
    int resize_file(char *ptr, size_t old_size, size_t new_size) override;
    int close_file(char *ptr, size_t actual_size) override;
    ~FStreamWriter() override;

private:
    std::ofstream m_ofs;
    std::string m_filename;
};
