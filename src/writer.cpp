#include "writer.h"

#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int MMapFileWriter::prepare_file(size_t &size, char **out) {
    m_fd = open(m_filename.c_str(), O_RDWR | O_CREAT, 0666);
    if (m_fd == -1) {
        std::cerr << "Failed to open the file for writing\n";
        return -1;
    }

    if (ftruncate(m_fd, size) == -1) {
        std::cerr << "Failed to set file size\n";
        return -1;
    }

    *out = (char *) (mmap(nullptr, size, PROT_WRITE, MAP_SHARED, m_fd, 0));
    if (out == MAP_FAILED) {
        std::cerr << "Memory mapping failed for writing\n";
        return -1;
    }

    return 0;
}

int MMapFileWriter::write_to_file(char *ptr, const std::string &data) {
    if (ptr) {
        std::memcpy(ptr, data.c_str(), data.size());
    }
    return 0;
}

int MMapFileWriter::close_file(char *ptr, size_t actual_size) {
    if (ptr) {
        if (msync(ptr, actual_size, MS_SYNC) == -1) {
            std::cerr << "Could not sync the file to disk\n";
            return -1;
        }

        if (munmap(ptr, actual_size) == -1) {
            std::cerr << "Could not unmap memory\n";
            return -1;
        }

        if (ftruncate(m_fd, actual_size) == -1) {
            std::cerr << "Failed to truncate file\n";
            return -1;
        }
    }
    return 0;
}

int MMapFileWriter::resize_file(char *ptr, size_t old_size, size_t new_size) {
    if (munmap(ptr, old_size) == -1) {
        std::cerr << "Could not unmap memory\n";
        return -1;
    }

    if (ftruncate(m_fd, new_size) == -1) {
        std::cerr << "Failed to truncate file\n";
        return -1;
    }

    ptr = (char *) mmap(nullptr, new_size, PROT_WRITE, MAP_SHARED, m_fd, 0);
    if (ptr == MAP_FAILED) {
        return -1;
    }

    return 0;
}

MMapFileWriter::~MMapFileWriter() {
    if (m_fd != -1) {
        close(m_fd);
    }
}

int FStreamWriter::prepare_file(size_t &, char **) {
    m_ofs = std::ofstream(m_filename, std::ios::out | std::ios::binary);
    if (!m_ofs.is_open()) {
        return -1;
    }
    return 0;
}

int FStreamWriter::write_to_file(char *, const std::string &data) {
    if (!m_ofs) {
        return -1;
    }
    m_ofs.write(data.c_str(), data.size());
    if (m_ofs.fail()) {
        return -1;
    }
    return 0;
}

int FStreamWriter::resize_file(char *, size_t, size_t) {
    return 0;
}

int FStreamWriter::close_file(char *, size_t) {
    if (m_ofs.is_open()) {
        m_ofs.close();
    }
    return 0;
}

FStreamWriter::~FStreamWriter() {
    if (m_ofs.is_open()) {
        m_ofs.close();
    }
}
