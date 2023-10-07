#include "file_manager.h"
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

FileManager::FileManager(std::string filename)
        : m_filename(std::move(filename)) {
}

int FileReader::prepare_file(size_t &size, char **out) {
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

    size = sb.st_size;
    if (!size) {
        std::cerr << "Empty file\n";
        return -1;
    }

    *out = (char *)(mmap(nullptr, size, PROT_READ, MAP_PRIVATE, m_fd, 0));
    if (out == MAP_FAILED) {
        std::cerr << "Memory mapping failed for reading\n";
        return -1;
    }

    return 0;
}

int FileReader::close_file(char *ptr, size_t actual_size) {
    if (ptr) {
        if (msync(ptr, actual_size, MS_SYNC) == -1) {
            std::cerr << "Could not sync the file to disk\n";
            return -1;
        }

        if (munmap(ptr, actual_size) == -1) {
            std::cerr << "Could not unmap memory\n";
            return -1;
        }
        return 0;
    }
    return -1;
}

FileReader::~FileReader() {
    if (m_fd != -1) {
        close(m_fd);
    }
}

int FileWriter::prepare_file(size_t &size, char **out) {
    m_fd = open(m_filename.c_str(), O_RDWR | O_CREAT, 0666);
    if (m_fd == -1) {
        std::cerr << "Failed to open the file for writing\n";
        return -1;
    }

    if (ftruncate(m_fd, size) == -1) {
        std::cerr << "Failed to set file size\n";
        return -1;
    }

    *out = (char *)(mmap(nullptr, size, PROT_WRITE, MAP_SHARED, m_fd, 0));
    if (out == MAP_FAILED) {
        std::cerr << "Memory mapping failed for writing\n";
        return -1;
    }

    return 0;
}

int FileWriter::write_to_file(char *ptr, const std::string &data) {
    if (ptr) {
        std::memcpy(ptr, data.c_str(), data.size());
    }
    return 0;
}

int FileWriter::close_file(char *ptr, size_t actual_size) {
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

int FileWriter::resize_file(char *ptr, size_t old_size, size_t new_size) {
    if (munmap(ptr, old_size) == -1) {
        return -1;
    }

    if (ftruncate(m_fd, new_size) == -1) {
        return -1;
    }

    ptr = (char *) mmap(nullptr, new_size, PROT_WRITE, MAP_SHARED, m_fd, 0);
    if (ptr == MAP_FAILED) {
        return -1;
    }

    return 0;
}

FileWriter::~FileWriter() {
    if (m_fd != -1) {
        close(m_fd);
    }
}
