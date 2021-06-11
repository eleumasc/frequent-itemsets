#include "Transactions.h"

#include <algorithm>
#include <map>
#include <sstream>

Transactions Transactions::fromFile(std::ifstream &inputFile)
{
    Transactions ts;

    std::map<std::string, int> indexByItem;

    for (std::string line; std::getline(inputFile, line);)
    {
        std::vector<int> t;

        std::istringstream tokens(line);

        for (std::string item; std::getline(tokens, item, ' ');)
        {
            auto it = indexByItem.find(item);
            if (it != indexByItem.end())
            {
                ts.m_itemComparatorByFrequencyDesc.frequencyByItem[it->second] += 1;
                t.push_back(it->second);
            }
            else
            {
                int index = ts.m_items.size();
                ts.m_items.push_back(item);
                ts.m_itemComparatorByFrequencyDesc.frequencyByItem.push_back(1);
                indexByItem[item] = index;
                t.push_back(index);
            }
        }

        if (t.size() != 0)
        {
            ts.m_data.push_back(t);
        }
    }

    return ts;
}

template <typename T>
void Transactions::reorder(std::vector<T> &v, std::vector<int> index)
{
    int v_size = v.size();
    for (int i = 0; i < v_size; ++i)
    {
        while (index[i] != i)
        {
            int oldIndex = index[index[i]];
            T oldElement = v[index[i]];

            v[index[i]] = v[i];
            index[index[i]] = index[i];

            index[i] = oldIndex;
            v[i] = oldElement;
        }
    }
}

void Transactions::sort()
{
    for (auto &&t : m_data)
    {
        std::sort(t.begin(), t.end());
    }
}

void Transactions::sortByFrequency()
{
    int items_size = m_items.size();

    std::vector<int> indexes;
    for (int i = 0; i < items_size; ++i)
    {
        indexes.push_back(i);
    }

    std::sort(indexes.begin(), indexes.end(), m_itemComparatorByFrequencyDesc);

    std::vector<int> remapped(items_size);
    for (int i = 0; i < items_size; ++i)
    {
        remapped[indexes[i]] = i;
    }

    for (auto &&t : m_data)
    {
        for (auto &&item : t)
        {
            item = remapped[item];
        }
        std::sort(t.begin(), t.end());
    }

    reorder(m_items, remapped);

    reorder(m_itemComparatorByFrequencyDesc.frequencyByItem, remapped);
}

const std::vector<std::vector<int>> &Transactions::getData() const
{
    return m_data;
}

const std::vector<std::string> &Transactions::getItems() const
{
    return m_items;
}

bool ItemComparatorByFrequencyDesc::operator()(const int &a, const int &b) const
{
    return (frequencyByItem[a] > frequencyByItem[b]);
};
