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
            auto fpcond = fptree.makeConditional(item, minsup);

            ++result[fpcond.getItemset().size() - 1];

            if (m_logEnabled)
            {
                std::cout << '{';
                for (auto &&item1 : fpcond.getItemset())
                {
                    std::cout << ' ' << fpcond.getItems()[item1];
                }
                std::cout << ' ' << '}' << std::endl;
            }

            mine(result, fpcond, minsup, item - 1);
        }
    }
}
