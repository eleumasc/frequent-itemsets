#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Transactions.h"

class FPTree
{
public:
    static FPTree make(const Transactions &ts);

    int computeSupport(int item) const;

    FPTree makeConditional(int item, int minsup) const;

    const std::vector<int> &getItemset() const;

    const std::vector<std::string> &getItems() const;

private:
    struct Node
    {
        int item;

        int count = 0;

        std::unique_ptr<Node> leftChild;

        std::unique_ptr<Node> rightSibling;

        Node *parent;

        int index;

        Node(int item, Node *parent);
    };

    struct HeaderRecord
    {
        FPTree::Node *node;

        int count;
    };

    using Header = std::vector<std::vector<HeaderRecord>>;

    std::shared_ptr<Node> m_tree;

    Header m_header;

    std::vector<int> m_itemset;

    std::shared_ptr<std::vector<std::string>> m_items;

    void enumerate();

    int enumerate(Node *node, int nextIndex);

    Header climb(int item) const;
};
