#include <iostream>
using namespace std;

template <typename T>
class Node
{
private:
    T value;
    Node<T>* leftNode = nullptr;
    Node<T>* rightNode = nullptr;
    Node<T>* parentNode = nullptr;

public:
    Node(T value, Node<T>* leftNode, Node<T>* rightNode, Node<T>* parentNode): 
        value(value), leftNode(leftNode), rightNode(rightNode), parentNode(parentNode) {}
    ~Node();

    Node<T>* GetLeftNode() const { return leftNode; }
    Node<T>* GetRightNode() const { return rightNode; }
    Node<T>* GetParentNode() const { return parentNode; }
    void SetLeftNode(Node<T>* node) 
    { 
        // if (node != nullptr) cout << value << " left -> " << node->GetValue() << endl;
        leftNode = node; 
    }
    void SetRightNode(Node<T>* node) 
    {
        // if (node != nullptr) cout << value << " right -> " << node->GetValue() << endl;
        rightNode = node; 
    }
    void SetParentNode(Node<T>* node) 
    { 
        // if (node != nullptr) cout << value << " parent -> " << node->GetValue() << endl;
        parentNode = node; 
    }
    T GetValue() const { return value; }
    void SetValue(T v) { value = v; }

};

template <typename T>
inline Node<T>::~Node()
{
    if (leftNode != nullptr)
        delete leftNode;
    if (rightNode != nullptr)
        delete rightNode;
}

