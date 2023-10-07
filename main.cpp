#include <chrono>
#include <iostream>
#include <string>

#include "dictionary.h"
#include "reader.h"
#include "timer.h"
#include "writer.h"

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 5) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> [reader_writer_type] [--verbose]" << std::endl;
        return 1;
    }

    const std::string input_file = argv[1];
    const std::string output_file = argv[2];
    std::string rw_type = (argc >= 4 && std::string(argv[3]) != "--verbose") ? argv[3] : "default";

    bool verbose = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--verbose") {
            verbose = true;
            break;
        }
    }

    std::unique_ptr<IReader> reader;
    std::unique_ptr<IWriter> writer;

    if (rw_type == "mmap") {
        std::cout << "Using MMap" << std::endl;
        reader = std::make_unique<MMapFileReader>(input_file);
        writer = std::make_unique<MMapFileWriter>(output_file);
    } else {
        std::cout << "Using FStream" << std::endl;
        reader = std::make_unique<FStreamReader>(input_file);
        writer = std::make_unique<FStreamWriter>(output_file);
    }

    {
        Timer timer("Total time", verbose);
        Dictionary dict(std::move(reader), std::move(writer));
        if (dict.process(verbose) != 0) {
            std::cerr << "Process failed!" << std::endl;
            return 1;
        }
    }

    return 0;
}
