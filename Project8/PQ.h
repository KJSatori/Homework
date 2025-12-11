#include "Node.h"
#include "NodeQueue.h"

template <typename T>
class PQ
{
private:
    Node<T>* headNode = nullptr;
    Node<T>* tailNode = nullptr;

    Node<T>* FindTail(Node<T>* root)
    {
        if (!root) return nullptr;
        NodeQueue<T> queue;
        queue.Enqueue(root);
        Node<T>* last = nullptr;
        while (!queue.Empty())
        {
            last = queue.Dequeue();
            if (last->GetRightNode()) queue.Enqueue(last->GetRightNode());
            if (last->GetLeftNode()) queue.Enqueue(last->GetLeftNode());
            
        }
        return last;
    }
public:
    PQ(T value): headNode(new Node<T>(value, nullptr, nullptr, nullptr)),
        tailNode(headNode) {}
    PQ() {}
    ~PQ() { if(headNode != nullptr) delete headNode; }

    bool Empty() const;
    void Push(T value);
    T Pop();
};

template <typename T>
bool PQ<T>::Empty() const
{
    return headNode == nullptr;
}

template <typename T>
void PQ<T>::Push(T value)
{
    if (tailNode == nullptr || headNode == nullptr)
    {
        headNode = new Node<T>(value, nullptr, nullptr, nullptr);
        tailNode = headNode;
        return;
    }
    Node<T>* newNode = new Node<T>(value, nullptr, nullptr, nullptr);

    // 获得最后一行的最右侧节点
    Node<T>* currNode = headNode;
    Node<T>* firstNodeAtLastRow;
    while (currNode != nullptr)
    {
        if (currNode->GetRightNode() != nullptr)
        {
            currNode = currNode->GetRightNode();
        }
        else
        {
            firstNodeAtLastRow = currNode;
            break;
        }
    }

    // 获得Tail左侧的预备节点的父节点
    Node<T>* formerNode = tailNode;
    currNode = tailNode->GetParentNode();
    if (currNode == nullptr)
    {
        currNode = firstNodeAtLastRow;
    }
    else
    {
        while (currNode != nullptr)
        {
            if (currNode->GetLeftNode() == formerNode)
            {
                formerNode = currNode;
                currNode = currNode->GetParentNode();
                if (currNode == nullptr && formerNode == headNode)
                {
                    currNode = firstNodeAtLastRow;
                    break;
                }
            }
            if (currNode->GetRightNode() == formerNode)
            {
                formerNode = currNode;
                currNode = currNode->GetLeftNode();
                while (currNode != nullptr)
                {
                    formerNode = currNode;
                    currNode = currNode->GetRightNode();
                }
                currNode = formerNode;
                break;
            }
        }
    }
    if (currNode != nullptr) 
    {
        if (currNode->GetRightNode() == nullptr)
        {
            currNode->SetRightNode(newNode);
        }
        else
        {
            currNode->SetLeftNode(newNode);
        }
        newNode->SetParentNode(currNode);
        tailNode = newNode;
    }

    currNode = tailNode;
    while (currNode->GetParentNode() != nullptr && (currNode->GetParentNode()->GetValue() > currNode->GetValue()))
    {
        T temp = currNode->GetValue();
        currNode->SetValue(currNode->GetParentNode()->GetValue());
        currNode->GetParentNode()->SetValue(temp);
        currNode = currNode->GetParentNode();
    }
    
}

template <typename T>
T PQ<T>::Pop()
{
    if (headNode == nullptr)
    {
        cerr << "错误: PQ为空! " << endl;
        return T();
    }

    if (headNode == tailNode)
    {
        T output = headNode->GetValue();
        delete headNode;
        headNode = nullptr;
        tailNode = nullptr;
        return output;
    }

    T output = headNode->GetValue();
    // 将尾节点与头节点交换
    headNode->SetValue(tailNode->GetValue());

    Node<T>* tailParentNode = tailNode->GetParentNode();
    if (tailParentNode->GetLeftNode() == tailNode) tailParentNode->SetLeftNode(nullptr);
    else tailParentNode->SetRightNode(nullptr);
    delete tailNode;

    // 重新寻找尾节点
    tailNode = FindTail(headNode);


    // 处理互换
    Node<T>* currNode = headNode;
    Node<T>* smallerNode;
    if(currNode->GetLeftNode() == nullptr)
    {
        smallerNode = currNode->GetRightNode();
    }
    else
    {
        Node<T>* currLeftNode = currNode->GetLeftNode();
        Node<T>* currRightNode = currNode->GetRightNode();
        smallerNode = ((currLeftNode->GetValue() < currRightNode->GetValue()))?
            currLeftNode : currRightNode;
    }
    while (smallerNode != nullptr)
    {
        if (smallerNode->GetValue() < currNode->GetValue())
        {
            T temp = currNode->GetValue();
            currNode->SetValue(smallerNode->GetValue());
            smallerNode->SetValue(temp);

            currNode = smallerNode;
            if(currNode->GetLeftNode() == nullptr)
            {
                smallerNode = currNode->GetRightNode();
            }
            else
            {
                Node<T>* currLeftNode = currNode->GetLeftNode();
                Node<T>* currRightNode = currNode->GetRightNode();
                smallerNode = ((currLeftNode->GetValue() < currRightNode->GetValue()))?
                    currLeftNode : currRightNode;
            }
        }
        else
        {
            break;
        }
    }
    
    return output;
}
