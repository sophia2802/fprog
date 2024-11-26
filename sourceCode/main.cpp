#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <optional>
#include <algorithm>
#include <numeric>
#include <cctype>
#include <functional>
#include "RedBlackTree.h"

auto readFile = [](std::function<std::istream*()> inputProvider) -> std::optional<std::string> {
    std::unique_ptr<std::istream> input(inputProvider());
    if (!input || !(*input)) {
        return std::nullopt; 
    }

    std::stringstream buffer;
    buffer << input->rdbuf();
    return buffer.str(); 
};

auto fileInputProvider = [](const std::string& filename) -> std::function<std::istream*()> {
    return [filename]() -> std::istream* {
        return new std::ifstream(filename); 
    };
};

const auto tokenize = [](const std::string& text) -> std::vector<std::string> {
    std::vector<std::string> words;
    words.reserve(text.size() / 5); 

    std::string currentWord;

    std::for_each(text.begin(), text.end(), [&](char ch) {
        if (std::isalpha(ch)) {
            currentWord += std::tolower(ch); 
        } else if (!currentWord.empty()) {
            words.push_back(currentWord);   
            currentWord.clear();            
        }
    });

    if (!currentWord.empty()) {
        words.push_back(currentWord);       
    }

    return words;
};

const auto insertWordsIntoTree = [](const std::vector<std::string>& words) -> RedBlackTree {
    return std::accumulate(words.begin(), words.end(), RedBlackTree{},
        [](const RedBlackTree& tree, const std::string& word) {
            return tree.insert(word);
        });
};


const auto traverseTree = [](const auto& tree) -> std::vector<std::string> {
    return tree.inorderTraversal(); 
};

const auto writeToFile = [](const std::vector<std::string>& words, const std::string& filename) -> std::optional<std::string> {
    std::string content = std::accumulate(words.begin(), words.end(), std::string{},
        [](const std::string& acc, const std::string& word) {
            return acc + word + "\n";
        });

    std::ofstream file(filename);
    if (!file.is_open()) {
        return std::nullopt;
    }

    file << content;
    return "Wörter erfolgreich in " + filename + " geschrieben";
};

const auto processFile = [](const std::string& inputFile, const std::string& outputFile) -> std::optional<std::string> {
    auto contentOpt = readFile(fileInputProvider(inputFile));
    if (!contentOpt) {
        return std::nullopt; 
    }

    auto words = tokenize(*contentOpt);
    auto tree = insertWordsIntoTree(words);
    auto sortedWords = traverseTree(tree);

    return writeToFile(sortedWords, outputFile);
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <inputFile> [outputFile]" << std::endl;
        return 1;
    }
    const std::string inputFile = argv[1];
    const std::string outputFile = (argc > 2) ? argv[2] : "output.txt";

    auto result = processFile(inputFile, outputFile);
    if (result) {
        std::cout << *result << std::endl;
    } else {
        std::cerr << "Datei konnte nicht verarbeitet werden." << std::endl;
        return 1;
    }

    return 0;
}



