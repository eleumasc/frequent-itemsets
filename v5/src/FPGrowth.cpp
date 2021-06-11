#include "FPGrowth.h"

#include <iostream>
#include <omp.h>
#include <thread>

FPGrowth::FPGrowth(const Params &params) : m_logEnabled(params.isLogEnabled()),
                                           m_numThreads(params.hasNumThreads() ? params.getNumThreads() : std::thread::hardware_concurrency()),
                                           m_seqBound(params.hasSeqBound() ? params.getSeqBound() : 0)
{
}

std::vector<int> FPGrowth::mine(const FPTree &fptree, int minsup) const
{
    auto partResult = std::vector<std::vector<int>>(m_numThreads);
    for (int i = 0; i < m_numThreads; ++i)
    {
        partResult[i] = std::vector<int>(fptree.getItems().size());
    }

#pragma omp parallel num_threads(m_numThreads)
    {
#pragma omp single
        {
#pragma omp task shared(partResult)
            {
                mine(partResult, fptree, minsup, fptree.getItems().size() - 1);
            }
        }
    }

    auto result = std::vector<int>(fptree.getItems().size());
    for (int i = 0; i < result.size(); ++i)
    {
        for (int j = 0; j < m_numThreads; ++j)
        {
            result[i] += partResult[j][i];
        }
    }
    return result;
}

void FPGrowth::mine(std::vector<std::vector<int>> &partResult, const FPTree &fptree, int minsup, int item) const
{
    if (fptree.computeSupport(item) >= minsup)
    {
#pragma omp atomic
        ++partResult[omp_get_thread_num()][fptree.getItemset().size()];

        if (m_logEnabled)
        {
            std::cout << "{ " << fptree.getItems()[item];
            for (auto &&item1 : fptree.getItemset())
            {
                std::cout << ' ' << fptree.getItems()[item1];
            }
            std::cout << " }" << std::endl;
        }

        if (item > 0)
        {
            if (item >= m_seqBound)
            {
#pragma omp task shared(partResult)
                {
                    mine(partResult, fptree.makeConditional(item, minsup), minsup, item - 1); //go down
                }
#pragma omp task shared(partResult)
                {
                    mine(partResult, fptree, minsup, item - 1); //go right
                }
            }
            else
            {
                mine(partResult, fptree.makeConditional(item, minsup), minsup, item - 1); //go right
                mine(partResult, fptree, minsup, item - 1);                               //go right
            }
        }
    }
    else if (item > 0)
    {
        mine(partResult, fptree, minsup, item - 1); //go right
    }
}
