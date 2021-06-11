#include "FPTree.h"

#include <assert.h>

FPTree FPTree::make(const Transactions &ts)
{
    FPTree fptree;
    fptree.m_root = Node::makeRoot();
    fptree.m_header = std::vector<HeaderRecord>(ts.getItems().size());
    for (auto &&t : ts.getData())
    {
        auto node = fptree.m_root.get();
        for (auto &&item : t)
        {
            node = fptree.getOrPutChild(node, item);
            ++node->count;
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
    auto child = node->children[item].get();
    if (!m_header[item].head)
    {
        m_header[item].head = child;
    }
    else
    {
        m_header[item].last->next = child;
    }
    m_header[item].last = child;
    return child;
}

int FPTree::computeSupport(int item) const
{
    assert(item >= 0);
    assert(item < m_header.size());

    if (!m_header[item].head)
    {
        return 0;
    }

    int sup = 0;
    for (auto node = m_header[item].head; node; node = node->next)
    {
        sup += node->count;
    }
    return sup;
}

FPTree FPTree::makeConditional(int item, int minsup) const
{
    assert(item >= 0);
    assert(item < m_header.size());
    assert(minsup > 0);

    FPTree fpcond;
    fpcond.m_root = Node::makeRoot();
    fpcond.m_header = std::vector<HeaderRecord>(item);
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
        if (computeConditionalSupport(thisNode->item, item) >= minsup)
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

int FPTree::computeConditionalSupport(int item, int target) const
{
    if (target >= m_header.size() || item >= target)
    {
        return 0;
    }
    if (m_header[item].head == nullptr)
    {
        return 0;
    }

    int sup = 0;
    for (auto node = m_header[item].head; node != nullptr; node = node->next)
    {
        sup += computeConditionalSupport(node, target);
    }
    return sup;
}

int FPTree::computeConditionalSupport(Node *node, int target) const
{
    if (node->item == target)
    {
        return node->count;
    }
    else if (node->isPrefixOf(target))
    {
        int sup = 0;
        for (auto &&child : node->children)
        {
            sup += computeConditionalSupport(child.second.get(), target);
        }
        return sup;
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
        return;
    }
    for (auto &&child : children)
    {
        child.second->initPrefixOf();
        prefixOf.insert(child.first);
        prefixOf.insert(child.second->prefixOf.begin(), child.second->prefixOf.end());
    }
}

bool FPTree::Node::isPrefixOf(int item) const
{
    if (this->item == item)
    {
        return true;
    }
    return prefixOf.find(item) != prefixOf.end();
}
