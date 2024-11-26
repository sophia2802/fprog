#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <memory>
#include <string>
#include <vector>

// Enum für die Farbe eines Knotens (rot oder schwarz)
// Rot-Schwarz-Bäume verwenden diese Farben, um Balance zu gewährleisten.
enum class Color { Red, Black };

// Struktur eines Knotens
// Jeder Knoten hat einen Wert, eine Farbe sowie Zeiger auf den linken und rechten Teilbaum.
struct Node {
    std::string value;                      // Der Wert des Knotens
    Color color;                            // Farbe des Knotens (rot oder schwarz)
    std::shared_ptr<const Node> left;       // Zeiger auf den linken Teilbaum
    std::shared_ptr<const Node> right;      // Zeiger auf den rechten Teilbaum

    // Konstruktor: Erstellt einen neuen Knoten mit angegebenem Wert, Farbe und optionalen Teilbäumen
    Node(std::string value, Color color, std::shared_ptr<const Node> left = nullptr, std::shared_ptr<const Node> right = nullptr)
        : value(std::move(value)), color(color), left(left), right(right) {}
};

// Klasse für einen Rot-Schwarz-Baum
// Änderungen am Baum erzeugen neue Instanzen, ohne den bestehenden Baum zu verändern.
class RedBlackTree {
private:
    std::shared_ptr<const Node> root; // Zeiger auf die Wurzel des Baums

    // Privater Konstruktor: Erstellt einen Baum mit einer gegebenen Wurzel
    explicit RedBlackTree(std::shared_ptr<const Node> root) : root(std::move(root)) {}

public:
    // Konstruktor für einen leeren Baum
    RedBlackTree() : root(nullptr) {}

    // Lambda: Prüft, ob ein Knoten rot ist
    std::function<bool(const std::shared_ptr<const Node>&)> isRed = [](const std::shared_ptr<const Node>& node) -> bool {
        // Ein Knoten ist rot, wenn er existiert und seine Farbe Rot ist
        return node && node->color == Color::Red;
    };

    // Lambda: Führt eine Linksrotation durch
    std::function<std::shared_ptr<const Node>(const std::shared_ptr<const Node>&)> rotateLeft = [](const std::shared_ptr<const Node>& node) -> std::shared_ptr<const Node> {
        // Die Rotation verschiebt den rechten Teilbaum zur Wurzel
        return std::make_shared<Node>(
            node->right->value, node->color,
            std::make_shared<Node>(node->value, Color::Red, node->left, node->right->left),
            node->right->right
        );
    };

    // Lambda: Führt eine Rechtsrotation durch
    std::function<std::shared_ptr<const Node>(const std::shared_ptr<const Node>&)> rotateRight = [](const std::shared_ptr<const Node>& node) -> std::shared_ptr<const Node> {
        // Die Rotation verschiebt den linken Teilbaum zur Wurzel
        return std::make_shared<Node>(
            node->left->value, node->color,
            node->left->left,
            std::make_shared<Node>(node->value, Color::Red, node->left->right, node->right)
        );
    };

    // Lambda: Ändert die Farben der Knoten
    std::function<std::shared_ptr<const Node>(const std::shared_ptr<const Node>&)> flipColors = [](const std::shared_ptr<const Node>& node) -> std::shared_ptr<const Node> {
        // Die Wurzel wird rot, die Kinder schwarz
        return std::make_shared<Node>(
            node->value, Color::Red,
            std::make_shared<Node>(node->left->value, Color::Black, node->left->left, node->left->right),
            std::make_shared<Node>(node->right->value, Color::Black, node->right->left, node->right->right)
        );
    };

    // Einfügen eines Wertes in den Baum
    // Gibt einen neuen Baum zurück, da der Rot-Schwarz-Baum unveränderlich ist
    RedBlackTree insert(const std::string& value) const {
        // Rekursive Funktion für das Einfügen eines Knotens
        auto insertNode = [&](auto self, const std::shared_ptr<const Node>& node, const std::string& value) -> std::shared_ptr<const Node> {
            if (!node) return std::make_shared<Node>(value, Color::Red); // Neuer Knoten wird immer rot eingefügt

            // Neuen Teilbaum basierend auf der Vergleichsoperation erstellen
            auto newNode = [&]() -> std::shared_ptr<const Node> {
                if (value < node->value)
                    return std::make_shared<Node>(node->value, node->color, self(self, node->left, value), node->right);
                if (value > node->value)
                    return std::make_shared<Node>(node->value, node->color, node->left, self(self, node->right, value));
                return node; // Keine Änderung bei doppeltem Wert
            }();

            // Baum balancieren
            auto balanced = newNode;
            if (isRed(balanced->right) && !isRed(balanced->left)) balanced = rotateLeft(balanced);
            if (isRed(balanced->left) && balanced->left && isRed(balanced->left->left)) balanced = rotateRight(balanced);
            if (isRed(balanced->left) && isRed(balanced->right)) balanced = flipColors(balanced);

            return balanced;
        };

        // Neue Wurzel mit Schwarzer Farbe erstellen
        auto newRoot = insertNode(insertNode, root, value);
        return RedBlackTree(
            std::make_shared<Node>(newRoot->value, Color::Black, newRoot->left, newRoot->right)
        );
    }

    // Inorder-Traversierung des Baums
    // Gibt eine sortierte Liste der Knotenwerte zurück
    std::vector<std::string> inorderTraversal() const {
        std::vector<std::string> result;

        // Rekursive Funktion für die Traversierung
        auto traverse = [&](auto self, const std::shared_ptr<const Node>& node) -> void {
            if (!node) return;
            self(self, node->left); // Linken Teilbaum besuchen
            result.push_back(node->value); // Wurzel hinzufügen
            self(self, node->right); // Rechten Teilbaum besuchen
        };

        traverse(traverse, root);
        return result; // Sortierte Liste zurückgeben
    }
};

#endif // REDBLACKTREE_H
