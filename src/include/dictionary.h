#pragma once

#include <array>
#include <memory>
#include <queue>
#include <string>

#include "file_manager.h"

static constexpr size_t ENGLISH_LETTER_COUNT = 26;

struct TrieNode {
    int word_count = 0;
    std::array<std::unique_ptr<TrieNode>, ENGLISH_LETTER_COUNT> children;

    TrieNode()
            : children{} {
    }
};

using WordFreq = std::pair<std::string, int>;
class Comp {
public:
    bool operator()(const WordFreq &a, const WordFreq &b) const {
        if (a.second != b.second) {
            return a.second < b.second;
        }
        return a.first > b.first;
    }
};

class Dictionary {
public:
    Dictionary();
    int process(const std::string &input_file, const std::string &output_file);

private:
    void populate_trie(const char *file_content, size_t file_size);
    int prepare_to_output();
    int write_to_file(const std::string &output_file);
    std::priority_queue<WordFreq, std::vector<WordFreq>, Comp> m_queue;
    std::unique_ptr<TrieNode> m_trie_root;
};
