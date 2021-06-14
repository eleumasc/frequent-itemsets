#include "FPTree.h"

#include <assert.h>

FPTree FPTree::make(const Transactions &ts)
{
    std::unique_ptr<Node> rootChild;
    for (auto &&t : ts.getData())
    {
        std::unique_ptr<Node> *node_ptr = &rootChild;
        Node *parent = nullptr;
        for (auto &&item : t)
        {
            while (*node_ptr && (*node_ptr)->item < item)
            {
                node_ptr = &(*node_ptr)->rightSibling;
            }
            if (!*node_ptr)
            {
                *node_ptr = std::make_unique<Node>(item, parent);
            }
            else if ((*node_ptr)->item != item)
            {
                std::unique_ptr<Node> next = std::move(*node_ptr);
                *node_ptr = std::make_unique<Node>(item, parent);
                (*node_ptr)->rightSibling = std::move(next);
            }
            ++(*node_ptr)->count;
            parent = (*node_ptr).get();
            node_ptr = &(*node_ptr)->leftChild;
        }
    }

    FPTree fptree;
    fptree.m_tree = std::move(rootChild);
    fptree.m_header = Header(ts.getItems().size());
    fptree.enumerate();
    fptree.m_items = std::make_shared<std::vector<std::string>>(ts.getItems());
    return fptree;
}

int FPTree::computeSupport(int item) const
{
    assert(item >= 0);
    assert(item < m_header.size());

    int sup = 0;
    for (auto &&rec : m_header[item])
    {
        sup += rec.count;
    }
    return sup;
}

FPTree FPTree::makeConditional(int item, int minsup) const
{
    assert(item >= 0);
    assert(item < m_header.size());
    assert(minsup > 0);

    FPTree fptree;
    fptree.m_tree = m_tree;
    fptree.m_header = climb(item);
    fptree.m_itemset.push_back(item);
    fptree.m_itemset.insert(fptree.m_itemset.end(), m_itemset.begin(), m_itemset.end());
    fptree.m_items = m_items;
    return fptree;
}

const std::vector<int> &FPTree::getItemset() const
{
    return m_itemset;
}

const std::vector<std::string> &FPTree::getItems() const
{
    return *m_items;
}

inline void FPTree::enumerate()
{
    enumerate(m_tree.get(), 1);
}

int FPTree::enumerate(Node *node, int nextIndex)
{
    for (; node; node = node->rightSibling.get())
    {
        int nodeIndex = enumerate(node->leftChild.get(), nextIndex);
        node->index = nodeIndex;
        m_header[node->item].push_back(HeaderRecord{.node = node, .count = node->count});
        nextIndex = nodeIndex + 1;
    }
    return nextIndex;
}

FPTree::Header FPTree::climb(int item) const
{
    Header result(item);
    std::vector<HeaderRecord> countStack;
    for (auto currRecord = m_header[item].begin(); currRecord != m_header[item].end(); ++currRecord)
    {
        auto nextRecord = currRecord + 1;
        int count = currRecord->count;
        for (Node *node = currRecord->node->parent; node; node = node->parent)
        {
            if (nextRecord != m_header[item].end() && node->index > nextRecord->node->index)
            {
                if (!countStack.empty() && countStack.back().node == node)
                {
                    countStack.back().count += count;
                }
                else
                {
                    countStack.push_back(HeaderRecord{.node = node, .count = count});
                }
                break;
            }
            if (!countStack.empty() && countStack.back().node == node)
            {
                count += countStack.back().count;
                countStack.pop_back();
            }
            result[node->item].push_back(HeaderRecord{.node = node, .count = count});
        }
    }
    assert(countStack.empty());
    return result;
}

////////////////////////////////////////////////////////////////////////////////

FPTree::Node::Node(int item, Node *parent) : item(item), parent(parent)
{
}
