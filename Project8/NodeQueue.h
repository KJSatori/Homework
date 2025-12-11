template <typename T>
class NodeQueue {
private:
    struct QNode {
        Node<T>* data;
        QNode* next;
        QNode(Node<T>* d): data(d), next(nullptr) {}
    };
    QNode* front;
    QNode* rear;
public:
    NodeQueue(): front(nullptr), rear(nullptr) {}
    ~NodeQueue() {
        while (!Empty()) Dequeue();
    }
    bool Empty() const { return front == nullptr; }
    void Enqueue(Node<T>* d) {
        QNode* newNode = new QNode(d);
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }
    Node<T>* Dequeue() {
        if (Empty()) return nullptr;
        QNode* temp = front;
        Node<T>* result = temp->data;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete temp;
        return result;
    }
};