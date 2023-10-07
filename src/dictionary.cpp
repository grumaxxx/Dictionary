#include "dictionary.h"

static constexpr size_t INITIAL_FILE_SIZE = 4096;

Dictionary::Dictionary()
        : m_trie_root(new TrieNode()) {
}

int Dictionary::write_to_file(const std::string &output_file) {
    FileWriter manager(output_file);
    size_t curr_size = INITIAL_FILE_SIZE;
    char *ptr;
    auto r = manager.prepare_file(curr_size, &ptr);
    if (r != 0) {
        return -1;
    }
    size_t offset = 0;

    while (!m_queue.empty()) {
        const auto &elem = m_queue.top();
        std::string line = elem.first + " " + std::to_string(elem.second) + "\n";

        if (offset + line.size() > curr_size) {
            curr_size *= 2;
            r = manager.resize_file(ptr, curr_size, curr_size * 2);
            if (r != 0) {
                return -1;
            }
        }

        r = manager.write_to_file(ptr + offset, line);
        if (r != 0) {
            return -1;
        }
        offset += line.size();

        m_queue.pop();
    }

    r = manager.close_file(ptr, offset);
    if (r != 0) {
        return -1;
    }
    return 0;
}

void Dictionary::populate_trie(const char *file_content, size_t file_size) {
    TrieNode *node = m_trie_root.get();
    for (size_t i = 0; i <= file_size; ++i) {
        char c = file_content[i];
        if (!std::isalpha(c)) {
            if (node == m_trie_root.get()) {
                continue;
            }
            node->word_count++;
            node = m_trie_root.get();
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
static void dfs(TrieNode *node, std::string current_word, T &pq) {
    if (node == nullptr) {
        return;
    }

    if (node->word_count > 0) {
        pq.push({current_word, node->word_count});
    }

    for (int i = 0; i < 26; ++i) {
        if (node->children[i].get() != nullptr) {
            char nextChar = 'a' + i;
            dfs(node->children[i].get(), current_word + nextChar, pq);
        }
    }
}

int Dictionary::prepare_to_output() {
    dfs(m_trie_root.get(), "", m_queue);
    return 0;
}

int Dictionary::process(const std::string &input_file, const std::string &output_file) {
    FileReader manager(input_file);
    size_t file_size = 0;
    char *file_content = nullptr;
    auto r = manager.prepare_file(file_size, &file_content);
    if (r != 0) {
        return -1;
    }

    this->populate_trie(file_content, file_size);
    r = manager.close_file(file_content, file_size);
    if (r != 0) {
        return -1;
    }

    this->prepare_to_output();

    return write_to_file(output_file);
}
