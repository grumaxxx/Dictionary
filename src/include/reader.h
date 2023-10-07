#pragma once

#include <fstream>
#include <string>

class IReader {
public:
    virtual int read(char **content, size_t *size) = 0;
    virtual ~IReader() = default;
};

class MMapFileReader : public IReader {
public:
    explicit MMapFileReader(std::string filename)
            : m_filename(std::move(filename)) {
    }
    int read(char **content, size_t *size) override;
    ~MMapFileReader() override;

private:
    char *m_data = nullptr;
    unsigned long m_size = 0;
    std::string m_filename;
    int m_fd = -1;
};

class FStreamReader : public IReader {
public:
    FStreamReader(std::string file_name)
            : m_filename(std::move(file_name)) {
    }
    int read(char **out, size_t *size) override;

private:
    std::string m_data;
    std::string m_filename;
};
