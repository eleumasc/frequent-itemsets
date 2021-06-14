#include "FPGrowth.h"

#include <iostream>

FPGrowth::FPGrowth(const Params &params) : m_logEnabled(params.isLogEnabled())
{
}

std::vector<int> FPGrowth::mine(const FPTree &fptree, int minsup) const
{
    std::vector<int> result(fptree.getItems().size());
    mine(result, fptree, minsup, fptree.getItems().size() - 1);
    return result;
}

void FPGrowth::mine(std::vector<int> &result, const FPTree &fptree, int minsup, int leftItem) const
{
    for (int item = leftItem; item >= 0; --item)
    {
        if (fptree.computeSupport(item) >= minsup)
        {
            ++result[fptree.getItemset().size()];

            if (m_logEnabled)
            {
                std::cout << "{ " << fptree.getItems()[item];
                for (auto &&item1 : fptree.getItemset())
                {
                    std::cout << ' ' << fptree.getItems()[item1];
                }
                std::cout << " }" << std::endl;
            }

            mine(result, fptree.makeConditional(item, minsup), minsup, item - 1);
        }
    }
}
