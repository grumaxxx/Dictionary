#include <chrono>
#include <fstream>
#include <iostream>

#include "dictionary.h"
#include "reader.h"
#include "timer.h"
#include "writer.h"
#include "gtest/gtest.h"

void create_test_file(const std::string &filename, const std::string &content) {
    std::ofstream outfile(filename);
    outfile << content;
    outfile.close();
}

std::string read_file(const std::string &filename) {
    std::ifstream infile(filename);
    std::stringstream buffer;
    buffer << infile.rdbuf();
    return buffer.str();
}

TEST(DictionaryTest, EmptyFile) {
    create_test_file("test.txt", "");

    Dictionary dict(std::make_unique<MMapFileReader>("test.txt"), std::make_unique<MMapFileWriter>("output.txt"));
    auto res = dict.process(true);
    ASSERT_EQ(res, -1);
}

TEST(DictionaryTest, BasicTest) {
    create_test_file("test.txt", "hello world world");

    Dictionary dict(std::make_unique<MMapFileReader>("test.txt"), std::make_unique<MMapFileWriter>("output.txt"));
    auto res = dict.process(true);
    ASSERT_EQ(res, 0);

    std::string output = read_file("output.txt");
    ASSERT_EQ(output, "world 2\nhello 1\n");
}

TEST(DictionaryTest, SomeUnusualCharacters) {
    create_test_file("test.txt", "hello12˙world world  123");

    Dictionary dict(std::make_unique<MMapFileReader>("test.txt"), std::make_unique<MMapFileWriter>("output.txt"));
    auto res = dict.process(true);
    ASSERT_EQ(res, 0);

    std::string output = read_file("output.txt");
    ASSERT_EQ(output, "world 2\nhello 1\n");
}

TEST(DictionaryTest, TestPreparedFilesMMap) {

    {
        Timer timer("Total time", true);
        Dictionary dict(std::make_unique<MMapFileReader>("testfiles/in.txt"),
                std::make_unique<MMapFileWriter>("testfiles/out.txt"));
        auto res = dict.process(true);
        ASSERT_EQ(res, 0);
    }

    std::string output = read_file("testfiles/out.txt");
    std::string expected = read_file("testfiles/expected_out.txt");
    ASSERT_EQ(output, expected);
}

TEST(DictionaryTest, TestPreparedFilesFStream) {
    {
        Timer timer("Total time", true);
        Dictionary dict(std::make_unique<FStreamReader>("testfiles/in.txt"),
                std::make_unique<FStreamWriter>("testfiles/out.txt"));
        auto res = dict.process(true);
        ASSERT_EQ(res, 0);
    }
    std::string output = read_file("testfiles/out.txt");
    std::string expected = read_file("testfiles/expected_out.txt");
    ASSERT_EQ(output, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
