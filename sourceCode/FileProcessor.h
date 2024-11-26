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

// Liest den Inhalt einer Datei mit einem Input-Provider aus
// Der Input-Provider gibt einen Zeiger auf einen Eingabestream zurück
auto readFile = [](std::function<std::istream*()> inputProvider) -> std::optional<std::string> {
    std::unique_ptr<std::istream> input(inputProvider());
    if (!input || !(*input)) {
        return std::nullopt; // Gibt nichts zurück, wenn der Input-Stream ungültig ist
    }

    std::stringstream buffer;
    buffer << input->rdbuf(); // Liest den gesamten Inhalt des Streams in einen String
    return buffer.str();      // Gibt den gelesenen String zurück
};

// Erstellt einen Input-Provider für eine Datei
// Der Input-Provider öffnet die Datei und gibt einen Stream zurück
auto fileInputProvider = [](const std::string& filename) -> std::function<std::istream*()> {
    return [filename]() -> std::istream* {
        return new std::ifstream(filename); // Öffnet die Datei als Eingabestream
    };
};

// Zerlegt einen Text in einzelne Wörter
// Wandelt alle Zeichen in Kleinbuchstaben um und ignoriert nicht-alphabetische Zeichen
const auto tokenize = [](const std::string& text) -> std::vector<std::string> {
    std::vector<std::string> words;
    words.reserve(text.size() / 5); // Reserviert Speicherplatz für Effizienz

    std::string currentWord;

    // Durchläuft jedes Zeichen im Text
    std::for_each(text.begin(), text.end(), [&](char ch) {
        if (std::isalpha(ch)) { 
            currentWord += std::tolower(ch); // Fügt Kleinbuchstaben zum aktuellen Wort hinzu
        } else if (!currentWord.empty()) {
            words.push_back(currentWord);   // Fügt das aktuelle Wort zur Liste hinzu
            currentWord.clear();            // Löscht das aktuelle Wort
        }
    });

    // Fügt das letzte Wort hinzu, falls vorhanden
    if (!currentWord.empty()) {
        words.push_back(currentWord);       
    }

    return words; // Gibt die Liste der Wörter zurück
};

// Fügt eine Liste von Wörtern in einen Rot-Schwarz-Baum ein
const auto insertWordsIntoTree = [](const std::vector<std::string>& words) -> RedBlackTree {
    return std::accumulate(words.begin(), words.end(), RedBlackTree{},
        [](const RedBlackTree& tree, const std::string& word) {
            return tree.insert(word); // Fügt jedes Wort in den Baum ein
        });
};

// Führt eine Inorder-Traversierung eines Rot-Schwarz-Baums aus
// Gibt die sortierten Wörter in einer Liste zurück
const auto traverseTree = [](const auto& tree) -> std::vector<std::string> {
    return tree.inorderTraversal();
};

// Schreibt eine Liste von Wörtern in eine Datei
// Gibt eine Erfolgsmeldung oder std::nullopt zurück, falls ein Fehler auftritt
const auto writeToFile = [](const std::vector<std::string>& words, const std::string& filename) -> std::optional<std::string> {
    // Erzeugt einen String aus der Liste von Wörtern, getrennt durch Zeilenumbrüche
    std::string content = std::accumulate(words.begin(), words.end(), std::string{},
        [](const std::string& acc, const std::string& word) {
            return acc + word + "\n"; // Fügt jedes Wort mit einem Zeilenumbruch hinzu
        });

    std::ofstream file(filename); // Öffnet die Ausgabedatei
    if (!file.is_open()) {
        return std::nullopt; // Gibt std::nullopt zurück, falls die Datei nicht geöffnet werden konnte
    }

    file << content; // Schreibt die Wörter in die Datei
    return "Wörter erfolgreich in " + filename + " geschrieben";
};

// Hauptprozess: Liest eine Eingabedatei, verarbeitet die Wörter und schreibt sie in eine Ausgabedatei
const auto processFile = [](const std::string& inputFile, const std::string& outputFile) -> std::optional<std::string> {
    auto contentOpt = readFile(fileInputProvider(inputFile)); // Liest die Eingabedatei
    if (!contentOpt) {
        return std::nullopt; // Gibt std::nullopt zurück, wenn die Datei nicht gelesen werden konnte
    }

    auto words = tokenize(*contentOpt);     // Zerlegt den Inhalt in Wörter
    auto tree = insertWordsIntoTree(words); // Fügt die Wörter in einen Rot-Schwarz-Baum ein
    auto sortedWords = traverseTree(tree);  // Traversiert den Baum, um sortierte Wörter zu erhalten

    return writeToFile(sortedWords, outputFile); // Schreibt die sortierten Wörter in die Ausgabedatei
};
