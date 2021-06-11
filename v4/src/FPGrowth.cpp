#include "FPGrowth.h"

#include <iostream>
#include <omp.h>
#include <thread>

FPGrowth::FPGrowth(const Params &params) : m_logEnabled(params.isLogEnabled()),
                                           m_numThreads(params.hasNumThreads() ? params.getNumThreads() : std::thread::hardware_concurrency())
{
}

std::vector<int> FPGrowth::mine(const FPTree &fptree, int minsup) const
{
    std::vector<int> result(fptree.getItems().size());
    omp_set_nested(true);
    mine(result, fptree, minsup, fptree.getItems().size() - 1, m_numThreads);
    return result;
}

void FPGrowth::mine(std::vector<int> &result, const FPTree &fptree, int minsup, int item, int numThreads) const
{
    if (fptree.computeSupport(item) >= minsup)
    {
        auto fpcond = fptree.makeConditional(item, minsup);

#pragma omp atomic
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

        if (item > 0)
        {
            if (numThreads > 1)
            {
#pragma omp parallel sections num_threads(numThreads)
                {
#pragma omp section
                    {
                        mine(result, fpcond, minsup, item - 1, (numThreads + 1) >> 1); //go down
                    }
#pragma omp section
                    {
                        mine(result, fptree, minsup, item - 1, numThreads >> 1); //go right
                    }
                }
            }
            else
            {
                mine(result, fpcond, minsup, item - 1, 1); //go right
                mine(result, fptree, minsup, item - 1, 1); //go right
            }
        }
    }
    else if (item > 0)
    {
        mine(result, fptree, minsup, item - 1, numThreads); //go right
    }
}
