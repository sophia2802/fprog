#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../RedBlackTree.h"
#include "../FileProcessor.h"

// Helpers for testing RedBlackTree
std::vector<std::string> testTreeInorder(const std::initializer_list<std::string>& words) {
    RedBlackTree tree;
    for (const auto& word : words) {
        tree = tree.insert(word);
    }
    return tree.inorderTraversal();
}

TEST_CASE("RedBlackTree") {
    SUBCASE("Empty tree") {
        RedBlackTree tree;
        CHECK(tree.inorderTraversal().empty());
    }

    SUBCASE("Single insertion") {
        RedBlackTree tree = RedBlackTree().insert("hello");
        CHECK(tree.inorderTraversal() == std::vector<std::string>{"hello"});
    }

    SUBCASE("Multiple insertions with balancing") {
        auto result = testTreeInorder({"banana", "apple", "cherry"});
        CHECK(result == std::vector<std::string>{"apple", "banana", "cherry"});
    }

    SUBCASE("Duplicate insertions") {
        auto result = testTreeInorder({"banana", "banana", "apple", "cherry", "apple"});
        CHECK(result == std::vector<std::string>{"apple", "banana", "cherry"});
    }
}

TEST_CASE("readFile") {
    SUBCASE("Valid input stream") {
        auto inputProvider = []() -> std::istream* {
            return new std::istringstream("Hello, World!");
        };
        auto content = readFile(inputProvider);
        CHECK(content.has_value());
        CHECK(*content == "Hello, World!");
    }

    SUBCASE("Empty input stream") {
        auto inputProvider = []() -> std::istream* {
            return new std::istringstream("");
        };
        auto content = readFile(inputProvider);
        CHECK(content.has_value());
        CHECK(*content == "");
    }

    SUBCASE("Invalid input stream") {
        auto inputProvider = []() -> std::istream* {
            return nullptr;
        };
        auto content = readFile(inputProvider);
        CHECK(!content.has_value());
    }
}

TEST_CASE("tokenize") {
    SUBCASE("Simple text") {
        auto words = tokenize("Hello, World! Welcome to C++ testing.");
        CHECK(words == std::vector<std::string>{"hello", "world", "welcome", "to", "c", "testing"});
    }

    SUBCASE("Mixed case and punctuation") {
        auto words = tokenize("Hi! C++ is FUN.");
        CHECK(words == std::vector<std::string>{"hi", "c", "is", "fun"});
    }

    SUBCASE("Empty string") {
        auto words = tokenize("");
        CHECK(words.empty());
    }

    SUBCASE("Only punctuation") {
        auto words = tokenize("!!!...,,,");
        CHECK(words.empty());
    }
}

TEST_CASE("insertWordsIntoTree") {
    SUBCASE("Inserting words into RedBlackTree") {
        std::vector<std::string> words = {"apple", "banana", "cherry"};
        auto tree = insertWordsIntoTree(words);
        auto sortedWords = tree.inorderTraversal();
        CHECK(sortedWords == std::vector<std::string>{"apple", "banana", "cherry"});
    }

    SUBCASE("Empty list") {
        std::vector<std::string> words = {};
        auto tree = insertWordsIntoTree(words);
        auto sortedWords = tree.inorderTraversal();
        CHECK(sortedWords.empty());
    }
}

TEST_CASE("traverseTree") {
    SUBCASE("Traverse populated tree") {
        auto result = testTreeInorder({"cherry", "banana", "apple"});
        CHECK(result == std::vector<std::string>{"apple", "banana", "cherry"});
    }

    SUBCASE("Traverse empty tree") {
        RedBlackTree tree;
        auto sortedWords = traverseTree(tree);
        CHECK(sortedWords.empty());
    }
}

TEST_CASE("writeToFile") {
    SUBCASE("Valid file output") {
        std::vector<std::string> words = {"apple", "banana", "cherry"};
        auto result = writeToFile(words, "test_output.txt");
        CHECK(result.has_value());
        CHECK(*result == "Wörter erfolgreich in test_output.txt geschrieben");

        std::ifstream file("test_output.txt");
        std::stringstream buffer;
        buffer << file.rdbuf();
        CHECK(buffer.str() == "apple\nbanana\ncherry\n");
    }

    SUBCASE("Invalid file path") {
        std::vector<std::string> words = {"apple", "banana", "cherry"};
        auto result = writeToFile(words, "/invalid_path/test_output.txt");
        CHECK(!result.has_value());
    }
}

TEST_CASE("processFile") {
    SUBCASE("Valid input and output files") {
        std::ofstream input("test_input.txt");
        input << "Hello, world! Welcome to the test.";
        input.close();

        auto result = processFile("test_input.txt", "test_output.txt");
        CHECK(result.has_value());
        CHECK(*result == "Wörter erfolgreich in test_output.txt geschrieben");

        std::ifstream output("test_output.txt");
        std::stringstream buffer;
        buffer << output.rdbuf();
        CHECK(buffer.str() == "hello\ntest\nthe\nto\nwelcome\nworld\n");
    }

    SUBCASE("Invalid input file") {
        auto result = processFile("nonexistent_input.txt", "test_output.txt");
        CHECK(!result.has_value());
    }
}

// Helper function to create nodes
std::shared_ptr<const Node> createNode(const std::string& value, Color color,
                                       std::shared_ptr<const Node> left = nullptr,
                                       std::shared_ptr<const Node> right = nullptr) {
    return std::make_shared<Node>(value, color, left, right);
}

// Test isRed
TEST_CASE("RedBlackTree: isRed") {
    RedBlackTree tree;

    auto redNode = createNode("red", Color::Red);
    auto blackNode = createNode("black", Color::Black);

    CHECK(tree.isRed(redNode) == true);
    CHECK(tree.isRed(blackNode) == false);
    CHECK(tree.isRed(nullptr) == false); // Null check
}

// Test rotateLeft
TEST_CASE("RedBlackTree: rotateLeft") {
    RedBlackTree tree;

    auto leftChild = createNode("left", Color::Black);
    auto rightChild = createNode("right", Color::Red);
    auto root = createNode("root", Color::Black, leftChild, rightChild);

    auto newRoot = tree.rotateLeft(root);

    CHECK(newRoot->value == "right");
    CHECK(newRoot->left->value == "root");
    CHECK(newRoot->left->left == leftChild);
    CHECK(newRoot->left->color == Color::Red);
    CHECK(newRoot->color == Color::Black);
}

// Test rotateRight
TEST_CASE("RedBlackTree: rotateRight") {
    RedBlackTree tree;

    auto leftChild = createNode("left", Color::Red);
    auto root = createNode("root", Color::Black, leftChild, nullptr);

    auto newRoot = tree.rotateRight(root);

    CHECK(newRoot->value == "left");
    CHECK(newRoot->right->value == "root");
    CHECK(newRoot->right->color == Color::Red);
    CHECK(newRoot->color == Color::Black);
}

// Test flipColors
TEST_CASE("RedBlackTree: flipColors") {
    RedBlackTree tree;

    auto leftChild = createNode("left", Color::Red);
    auto rightChild = createNode("right", Color::Red);
    auto root = createNode("root", Color::Black, leftChild, rightChild);

    auto newRoot = tree.flipColors(root);

    CHECK(newRoot->value == "root");
    CHECK(newRoot->color == Color::Red);
    CHECK(newRoot->left->color == Color::Black);
    CHECK(newRoot->right->color == Color::Black);
    CHECK(newRoot->left->value == "left");
    CHECK(newRoot->right->value == "right");
}