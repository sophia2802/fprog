#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

// Read file content as a string
auto read_file = [](const std::string& path) -> std::optional<std::string> {
    std::ifstream file(path);
    if (!file.is_open()) return std::nullopt;

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
};

// Tokenize function: lowercase and split by non-alphabetic characters
auto tokenize = [](const std::string& text) -> std::vector<std::string> {
    std::vector<std::string> tokens;
    std::string token;
    for (char c : text) {
        if (std::isalpha(c)) token += std::tolower(c);
        else if (!token.empty()) {
            tokens.push_back(token);
            token.clear();
        }
    }
    if (!token.empty()) tokens.push_back(token);
    return tokens;
};