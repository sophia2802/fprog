#ifndef REDBLACKTREE_HPP
#define REDBLACKTREE_HPP

#include <memory>
#include <string>
#include <functional>  // Für std::function

// Farben für den Red-Black-Tree
enum class Color { Red, Black };

// Definition des Knotens
struct Node {
    std::string key;
    Color color;
    std::shared_ptr<const Node> left;
    std::shared_ptr<const Node> right;

    Node(std::string k, Color c, std::shared_ptr<const Node> l = nullptr, std::shared_ptr<const Node> r = nullptr)
        : key(std::move(k)), color(c), left(std::move(l)), right(std::move(r)) {}
};

// Alias für den Baum
using Tree = std::shared_ptr<const Node>;

// Knoten erstellen
auto make_node = [](const std::string& key, Color color, Tree left = nullptr, Tree right = nullptr) {
    return std::make_shared<Node>(key, color, left, right);
};

// Prüfen, ob ein Knoten rot ist
auto is_red = [](const Tree& node) {
    return node && node->color == Color::Red;
};

// Helferfunktion zum Balancieren des Baumes
std::function<Tree(Tree)> balance = [](Tree node) -> Tree {
    if (is_red(node->right) && !is_red(node->left)) {
        return make_node(node->right->key, node->color,
                         make_node(node->key, Color::Red, node->left, node->right->left),
                         node->right->right);
    }
    if (is_red(node->left) && is_red(node->left->left)) {
        return make_node(node->left->key, node->color,
                         node->left->left,
                         make_node(node->key, Color::Red, node->left->right, node->right));
    }
    if (is_red(node->left) && is_red(node->right)) {
        return make_node(node->key, Color::Red,
                         make_node(node->left->key, Color::Black, node->left->left, node->left->right),
                         make_node(node->right->key, Color::Black, node->right->left, node->right));
    }
    return node;
};

// Einfügen in den Baum (unveränderlich, gibt neuen Baum zurück)
std::function<Tree(Tree, const std::string&)> insert = [](Tree node, const std::string& key) -> Tree {
    if (!node) return make_node(key, Color::Red);

    if (key < node->key) return balance(make_node(node->key, node->color, insert(node->left, key), node->right));
    if (key > node->key) return balance(make_node(node->key, node->color, node->left, insert(node->right, key)));

    return node; // Schlüssel existiert bereits, unverändert zurückgeben
};

// Wrapper für Einfügung, stellt sicher, dass die Wurzel schwarz ist
auto insert_tree = [](Tree root, const std::string& key) {
    auto node = insert(root, key);
    return make_node(node->key, Color::Black, node->left, node->right);
};

// In-Order-Traversierung zum Sammeln sortierter Wörter
std::function<void(const Tree&, const std::function<void(const std::string&)>&)> in_order = [](const Tree& node, const std::function<void(const std::string&)>& func) {
    if (node) {
        in_order(node->left, func);
        func(node->key);
        in_order(node->right, func);
    }
};

#endif // REDBLACKTREE_HPP
