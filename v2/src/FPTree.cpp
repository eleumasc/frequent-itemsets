#include "FPTree.h"

#include <assert.h>

FPTree FPTree::make(const Transactions &ts)
{
    FPTree fptree;
    fptree.m_root = Node::makeRoot();
    fptree.m_supports = std::vector<int>(ts.getItems().size());
    for (auto &&t : ts.getData())
    {
        auto node = fptree.m_root.get();
        for (auto &&item : t)
        {
            node = fptree.getOrPutChild(node, item);
            ++node->count;
            ++fptree.m_supports[item];
        }
    }
    fptree.m_root->initPrefixOf();
    fptree.m_items = std::make_shared<std::vector<std::string>>(ts.getItems());
    return fptree;
}

FPTree::Node *FPTree::getOrPutChild(Node *node, int item)
{
    auto it = node->children.find(item);
    if (it != node->children.end())
    {
        return it->second.get();
    }
    node->children[item] = std::make_unique<Node>(item);
    return node->children[item].get();
}

int FPTree::computeSupport(int item) const
{
    assert(item >= 0);
    assert(item < m_supports.size());

    return m_supports[item];
}

FPTree FPTree::makeConditional(int item, int minsup) const
{
    assert(item >= 0);
    assert(item < m_supports.size());
    assert(minsup > 0);

    FPTree fpcond;
    fpcond.m_root = Node::makeRoot();
    fpcond.m_supports = computeConditionalSupports(item);
    for (auto &&thisChild : m_root->children)
    {
        fillConditional(thisChild.second.get(), fpcond.m_root.get(), fpcond, item, minsup);
    }
    fpcond.m_root->initPrefixOf();
    fpcond.m_itemset.push_back(item);
    fpcond.m_itemset.insert(fpcond.m_itemset.end(), m_itemset.begin(), m_itemset.end());
    fpcond.m_items = m_items;
    return fpcond;
}

int FPTree::fillConditional(Node *thisNode, Node *parent, FPTree &fpcond, int item, int minsup) const
{
    if (thisNode->item == item)
    {
        return thisNode->count;
    }
    else if (thisNode->isPrefixOf(item))
    {
        int count = 0;
        if (fpcond.m_supports[thisNode->item] >= minsup)
        {
            auto node = fpcond.getOrPutChild(parent, thisNode->item);
            for (auto &&thisChild : thisNode->children)
            {
                count += fillConditional(thisChild.second.get(), node, fpcond, item, minsup);
            }
            node->count += count;
        }
        else
        {
            for (auto &&thisChild : thisNode->children)
            {
                count += fillConditional(thisChild.second.get(), parent, fpcond, item, minsup);
            }
        }
        return count;
    }
    return 0;
}

std::vector<int> FPTree::computeConditionalSupports(int item) const
{
    auto result = std::vector<int>(item);

    for (auto &&child : m_root->children)
    {
        computeConditionalSupports(result, child.second.get(), item);
    }

    return result;
}

int FPTree::computeConditionalSupports(std::vector<int> &result, const Node *node, int item) const
{
    if (node->item == item)
    {
        return node->count;
    }
    else if (node->isPrefixOf(item))
    {
        int count = 0;
        for (auto &&child : node->children)
        {
            count += computeConditionalSupports(result, child.second.get(), item);
        }
        result[node->item] += count;
        return count;
    }
    return 0;
}

const std::vector<int> &FPTree::getItemset() const
{
    return m_itemset;
}

const std::vector<std::string> &FPTree::getItems() const
{
    return *m_items;
}

////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<FPTree::Node> FPTree::Node::makeRoot()
{
    return std::make_unique<Node>(-1);
}

FPTree::Node::Node(int item) : item(item)
{
}

void FPTree::Node::initPrefixOf()
{
    if (children.empty())
    {
        prefixOf = BitRange();
        return;
    }
    int min = -1, max = -1;
    for (auto &&child : children)
    {
        child.second->initPrefixOf();
        int localmin = child.first;
        int localmax =
            child.second->prefixOf.range() > 0
                ? child.second->prefixOf.max()
                : localmin;
        if (min == -1 || localmin < min)
        {
            min = localmin;
        }
        if (max == -1 || localmax > max)
        {
            max = localmax;
        }
    }
    prefixOf = BitRange(min, max);
    for (auto &&child : children)
    {
        prefixOf.set(child.first);
        prefixOf.join(child.second->prefixOf);
    }
}

bool FPTree::Node::isPrefixOf(int item) const
{
    if (this->item == item)
    {
        return true;
    }
    return prefixOf.get(item);
}
