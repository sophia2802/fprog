#include <fstream>
#include <string>
#include <vector>

// Write sorted words to output file
auto write_file = [](const std::string& path, const std::vector<std::string>& words) {
    std::ofstream file(path);
    for (const auto& word : words) {
        file << word << "\n";
    }
};