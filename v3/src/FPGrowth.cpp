#include "FPGrowth.h"

#include <iostream>
#include <thread>

FPGrowth::FPGrowth(const Params &params) : m_logEnabled(params.isLogEnabled()),
                                           m_numThreads(params.hasNumThreads() ? params.getNumThreads() : std::thread::hardware_concurrency())
{
}

std::vector<int> FPGrowth::mine(const FPTree &fptree, int minsup) const
{
    std::vector<int> result(fptree.getItems().size());
    mine(result, fptree, minsup, fptree.getItems().size() - 1, m_numThreads);
    return result;
}

void FPGrowth::mine(std::vector<int> &result, const FPTree &fptree, int minsup, int leftItem, int numThreads) const
{
#pragma omp parallel for schedule(dynamic) num_threads(numThreads) if (numThreads > 1)
    for (int item = leftItem; item >= 0; --item)
    {
        if (fptree.computeSupport(item) >= minsup)
        {
#pragma omp atomic
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

            mine(result, fptree.makeConditional(item, minsup), minsup, item - 1, 0);
        }
    }
}
