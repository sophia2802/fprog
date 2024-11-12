#include "redBlackTree.hpp"
#include "fileProcessor.hpp"
#include "fileWriter.hpp"
#include <iostream>
#include <set>  // Verwenden eines Sets für einzigartige Wörter

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_text_file>\n";
        return 1;
    }

    auto path = std::string(argv[1]);
    auto content = read_file(path);
    if (!content) {
        std::cerr << "Error reading file.\n";
        return 1;
    }

    auto words = tokenize(*content);
    Tree root = nullptr;

    for (const auto& word : words) {
        root = insert_tree(root, word);
    }

    // Verwenden eines Sets, um einzigartige Wörter zu sammeln
    std::set<std::string> unique_words;
    in_order(root, [&](const std::string& word) { unique_words.insert(word); });

    // Ausgabedatei schreiben
    try {
        write_file("output.txt", {unique_words.begin(), unique_words.end()});
        std::cout << "Unique sorted words written to output.txt\n";
    } catch (const std::ios_base::failure& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
