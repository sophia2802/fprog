#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <memory>
#include <string>
#include <vector>

// Enum für Knotentyp
enum class Color { Red, Black };

// Struktur eines Knotens
//Sobald ein Knoten erstellt wurde, können weder sein Wert noch seine Verweise verändert werden
struct Node {
    std::string value;
    Color color;
    std::shared_ptr<const Node> left;       //zeigt auf linken Teilbaum
    std::shared_ptr<const Node> right;      //zeigt auf rechten Teilbaum

    Node(std::string value, Color color, std::shared_ptr<const Node> left = nullptr, std::shared_ptr<const Node> right = nullptr)
        : value(std::move(value)), color(color), left(left), right(right) {}
};

// Änderungen erzeugen neue Bäume, ohne die bestehenden zu modifizieren
class RedBlackTree {
private:
    std::shared_ptr<const Node> root;

public:
    // Konstruktor für einen leeren Baum
    RedBlackTree() : root(nullptr) {}

    // Einfügen eines Wertes (liefert neuen Baum zurück)
    RedBlackTree insert(const std::string& value) const {
        auto isRed = [](const std::shared_ptr<const Node>& node) -> bool {
            return node && node->color == Color::Red;
        };

        auto rotateLeft = [](const std::shared_ptr<const Node>& node) -> std::shared_ptr<const Node> {
            return std::make_shared<Node>(
                node->right->value, node->color,
                std::make_shared<Node>(node->value, Color::Red, node->left, node->right->left),
                node->right->right
            );
        };

        auto rotateRight = [](const std::shared_ptr<const Node>& node) -> std::shared_ptr<const Node> {
            return std::make_shared<Node>(
                node->left->value, node->color,
                node->left->left,
                std::make_shared<Node>(node->value, Color::Red, node->left->right, node->right)
            );
        };

        auto flipColors = [](const std::shared_ptr<const Node>& node) -> std::shared_ptr<const Node> {
            return std::make_shared<Node>(
                node->value, Color::Red,
                std::make_shared<Node>(node->left->value, Color::Black, node->left->left, node->left->right),
                std::make_shared<Node>(node->right->value, Color::Black, node->right->left, node->right->right)
            );
        };

        auto insertNode = [&](auto self, const std::shared_ptr<const Node>& node, const std::string& value) -> std::shared_ptr<const Node> {
            if (!node) return std::make_shared<Node>(value, Color::Red);

            auto newNode = [&]() -> std::shared_ptr<const Node> {
                if (value < node->value)
                    return std::make_shared<Node>(node->value, node->color, self(self, node->left, value), node->right);
                if (value > node->value)
                    return std::make_shared<Node>(node->value, node->color, node->left, self(self, node->right, value));
                return node;
            }();

            auto balanced = newNode;
            if (isRed(balanced->right) && !isRed(balanced->left)) balanced = rotateLeft(balanced);
            if (isRed(balanced->left) && balanced->left && isRed(balanced->left->left)) balanced = rotateRight(balanced);
            if (isRed(balanced->left) && isRed(balanced->right)) balanced = flipColors(balanced);

            return balanced;
        };

        auto newRoot = insertNode(insertNode, root, value);
        return RedBlackTree(
            std::make_shared<Node>(newRoot->value, Color::Black, newRoot->left, newRoot->right)
        );
    }

    // Inorder Traversal 
    std::vector<std::string> inorderTraversal() const {
        std::vector<std::string> result;

        auto traverse = [&](auto self, const std::shared_ptr<const Node>& node) -> void {
            if (!node) return;
            self(self, node->left);
            result.push_back(node->value);
            self(self, node->right);
        };

        traverse(traverse, root);
        return result;
    }

private:
    // Privater Konstruktor für Bauminstanz
    explicit RedBlackTree(std::shared_ptr<const Node> root) : root(std::move(root)) {}
};

#endif // REDBLACKTREE_H
