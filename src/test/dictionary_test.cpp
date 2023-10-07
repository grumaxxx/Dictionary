#include <iostream>
#include <fstream>
#include <chrono>

#include "gtest/gtest.h"
#include "dictionary.h"

void create_test_file(const std::string& filename, const std::string& content) {
    std::ofstream outfile(filename);
    outfile << content;
    outfile.close();
}

std::string read_file(const std::string& filename) {
    std::ifstream infile(filename);
    std::stringstream buffer;
    buffer << infile.rdbuf();
    return buffer.str();
}

TEST(DictionaryTest, EmptyFile) {
    create_test_file("test.txt", "");

    Dictionary dict;
    auto res = dict.process("test.txt", "output.txt");
    ASSERT_EQ(res, -1);
}

TEST(DictionaryTest, BasicTest) {
    create_test_file("test.txt", "hello world world");

    Dictionary dict;
    auto res = dict.process("test.txt", "output.txt");
    ASSERT_EQ(res, 0);

    std::string output = read_file("output.txt");
    ASSERT_EQ(output, "world 2\nhello 1\n");
}

TEST(DictionaryTest, SomeUnusualCharacters) {
    create_test_file("test.txt", "hello12˙world world  123");

    Dictionary dict;
    auto res = dict.process("test.txt", "output.txt");
    ASSERT_EQ(res, 0);

    std::string output = read_file("output.txt");
    ASSERT_EQ(output, "world 2\nhello 1\n");
}

TEST(DictionaryTest, TestPreparedFiles) {
    auto start_time = std::chrono::high_resolution_clock::now();

    Dictionary dict;
    auto res = dict.process("testfiles/in.txt", "testfiles/out.txt");
    ASSERT_EQ(res, 0);
    auto end_time = std::chrono::high_resolution_clock::now();

    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "Elapsed time: " << elapsed_time << " milliseconds" << std::endl;

    std::string output = read_file("testfiles/out.txt");
    std::string expected = read_file("testfiles/expected_out.txt");
    ASSERT_EQ(output, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
