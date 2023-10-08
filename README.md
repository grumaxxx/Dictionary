### Clone the repository
```bash
git clone --recurse-submodules https://github.com/grumaxxx/Dictionary.git
```

### Navigate to the project directory
```bash
cd Dictionary
```

### Create a build directory
```bash
mkdir build
```

### Change to the build directory
```bash
cd build
```

### Run CMake
```bash
cmake ..
```

### Build the project
```bash
make
```

## How to Run the Program

To run the program, you'll need to pass in two file names as command-line arguments: the input file and the output file.

### Command Syntax

The syntax to run the program is as follows:

```bash
./freq input_file output_file [reader_writer_type] [--verbose]
```

- `input_file`: Path to the file that you want to read from.
- `output_file`: Path to the file where you want to write the output.
- `reader_writer_type` (Optional): The type of reader and writer methods to use. Supported values are mmap or default. Defaults to default.
- `--verbose` (Optional): If this flag is present, the program will output additional debug information and statistics.

### Using Test Files

You can use test files located in the `src/test/testfiles` directory for quick testing. Simply copy them to the directory from which you are running your program, or specify the relative path to these test files when running the program.

For example:

```bash
./freq ../src/test/testfiles/in.txt out.txt --verbose
```

### Using Your Own Files

Alternatively, you can specify the path to your own custom input and output files:

```bash
./freq path/to/your/input.txt path/to/your/output.txt
```
