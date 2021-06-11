#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <unordered_set>

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

        std::map<int, std::unique_ptr<Node>> children;

        Node *next = nullptr;

        std::unordered_set<int> prefixOf;

        static std::unique_ptr<Node> makeRoot();

        Node(int item);

        void initPrefixOf();

        bool isPrefixOf(int item) const;
    };

    struct HeaderRecord
    {
        FPTree::Node *head = nullptr;

        FPTree::Node *last = nullptr;
    };

    std::shared_ptr<Node> m_root;

    std::vector<HeaderRecord> m_header;

    std::vector<int> m_itemset;

    std::shared_ptr<std::vector<std::string>> m_items;

    Node *getOrPutChild(Node *node, int item);

    int fillConditional(Node *thisNode, Node *parent, FPTree &fpcond, int item, int minsup) const;

    int computeConditionalSupport(int item, int target) const;

    int computeConditionalSupport(Node *node, int target) const;
};
