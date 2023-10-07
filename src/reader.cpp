#include "reader.h"

#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

MMapFileReader::~MMapFileReader() {
    if (m_data) {
        if (msync(m_data, m_size, MS_SYNC) == -1) {
            std::cerr << "Could not sync the file to disk\n";
        }

        if (munmap(m_data, m_size) == -1) {
            std::cerr << "Could not unmap memory\n";
        }
    }
    if (m_fd != -1) {
        close(m_fd);
    }
}

int MMapFileReader::read(char **out, size_t *size) {
    m_fd = open(m_filename.c_str(), O_RDONLY);
    if (m_fd == -1) {
        std::cerr << "Failed to open the file for reading\n";
        return -1;
    }

    struct stat sb {};
    if (fstat(m_fd, &sb) == -1) {
        std::cerr << "Failed to get file size\n";
        return -1;
    }

    *size = sb.st_size;
    if (!*size) {
        std::cerr << "Empty file\n";
        return -1;
    }

    *out = (char *) (mmap(nullptr, *size, PROT_READ, MAP_PRIVATE, m_fd, 0));
    if (out == MAP_FAILED) {
        std::cerr << "Memory mapping failed for reading\n";
        return -1;
    }
    m_data = *out;
    m_size = *size;
    return 0;
}

int FStreamReader::read(char **out, size_t *size) {
    std::ifstream ifs(m_filename, std::ios::binary);
    if (!ifs.is_open()) {
        return -1;
    }

    ifs.seekg(0, std::ios::end);
    *size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    m_data.resize(*size);

    if (!ifs.read((char *) m_data.data(), *size)) {
        return -1;
    }
    *out = (char *) m_data.data();
    return 0;
}
