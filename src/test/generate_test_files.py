from collections import Counter
import random
import string


def generate_word(length):
    return ''.join(random.choice(string.ascii_letters) for _ in range(length))


def generate_files(data_file_name, count_file_name, num_words, num_unique_words):
    word_count = Counter()
    unique_words = [generate_word(random.randint(4, 10)).lower() for _ in range(num_unique_words)]

    with open(data_file_name, 'w') as f:
        for _ in range(num_words):
            word = random.choice(unique_words)
            f.write(word)

            word_count[word] += 1

            num_random_chars = random.randint(1, 3)
            random_chars = ''.join(random.choice(' ,.;!?™ƒ1&3≠5&6') for _ in range(num_random_chars))
            f.write(random_chars)

    with open(count_file_name, 'w') as f:
        for word, count in sorted(word_count.items(), key=lambda x: (-x[1], x[0])):
            f.write(f"{word} {count}\n")


generate_files('testfiles/in.txt', 'testfiles/expected_out.txt', 10000000, 5000)
