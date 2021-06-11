#pragma once

#include <fstream>
#include <vector>

struct ItemComparatorByFrequencyDesc
{
    std::vector<int> frequencyByItem;

    bool operator()(const int &a, const int &b) const;
};

class Transactions
{
public:
    static Transactions fromFile(std::ifstream &file);

    void sort();

    void sortByFrequency();

    const std::vector<std::vector<int>> &getData() const;

    const std::vector<std::string> &getItems() const;

private:
    std::vector<std::vector<int>> m_data;

    std::vector<std::string> m_items;

    ItemComparatorByFrequencyDesc m_itemComparatorByFrequencyDesc;

    template <typename T>
    void reorder(std::vector<T> &v, std::vector<int> index);
};
