#include "dictionary.h"
#include "timer.h"

#include <iostream>

static constexpr size_t INITIAL_FILE_SIZE = 4096;
static constexpr size_t INITIAL_STRING_SIZE = 10;

Dictionary::Dictionary(std::unique_ptr<IReader> reader, std::unique_ptr<IWriter> writer)
        : m_trie_root(new TrieNode())
        , m_reader(std::move(reader))
        , m_writer(std::move(writer)) {
}

int Dictionary::write_to_file() {
    size_t curr_size = INITIAL_FILE_SIZE;
    char *ptr = nullptr;
    auto r = m_writer->prepare_file(curr_size, &ptr);
    if (r != 0) {
        return -1;
    }
    size_t offset = 0;

    while (!m_queue.empty()) {
        const auto &elem = m_queue.top();
        std::string line = elem.first + " " + std::to_string(elem.second) + "\n";

        if (offset + line.size() > curr_size) {
            curr_size *= 2;
            r = m_writer->resize_file(ptr, curr_size, curr_size * 2);
            if (r != 0) {
                return -1;
            }
        }

        r = m_writer->write_to_file(ptr + offset, line);
        if (r != 0) {
            std::cerr << "Failed to write to file\n";
            return -1;
        }
        offset += line.size();

        m_queue.pop();
    }

    r = m_writer->close_file(ptr, offset);
    if (r != 0) {
        std::cerr << "Failed to close file\n";
        return -1;
    }
    return 0;
}

void Dictionary::populate_trie(const char *file_content, size_t file_size) {
    TrieNode *root = m_trie_root.get();
    TrieNode *node = root;
    for (size_t i = 0; i <= file_size; ++i) {
        char c = file_content[i];
        if (!std::isalpha(c)) {
            if (node == root) {
                continue;
            }
            node->word_count++;
            node = root;
            continue;
        }

        c = std::tolower(c);
        size_t index = c - 'a';

        if (!node->children[index]) {
            node->children[index] = std::make_unique<TrieNode>();
        }
        node = node->children[index].get();
    }
}

template <typename T>
static void dfs(TrieNode *node, std::string &current_word, T &pq) {
    if (node == nullptr) {
        return;
    }

    if (node->word_count > 0) {
        pq.push({current_word, node->word_count});
    }

    for (int i = 0; i < ENGLISH_LETTER_COUNT; ++i) {
        if (node->children[i]) {
            current_word.push_back('a' + i);
            dfs(node->children[i].get(), current_word, pq);
            current_word.pop_back();
        }
    }
}

int Dictionary::prepare_to_output() {
    std::string initial;
    initial.reserve(INITIAL_STRING_SIZE);
    dfs(m_trie_root.get(), initial, m_queue);
    return 0;
}

int Dictionary::process(bool verbose) {
    size_t file_size = 0;
    char *file_content = nullptr;

    {
        Timer timer("Reading file", verbose);
        if (m_reader->read(&file_content, &file_size) != 0) {
            std::cerr << "Failed to read from file\n";
            return -1;
        };
    }

    {
        Timer timer("Populate trie", verbose);
        this->populate_trie(file_content, file_size);
    }

    {
        Timer timer("Prepare output", verbose);
        this->prepare_to_output();
    }

    int r;
    {
        Timer timer("Write to file", verbose);
        r = write_to_file();
    }

    return r;
}
