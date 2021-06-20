#include "FPGrowth.h"

#include <algorithm>
#include <iostream>
#include <thread>

#pragma omp declare reduction(vec_int_plus                                                                                          \
                              : std::vector <int>                                                                                   \
                              : std::transform(omp_out.begin(), omp_out.end(), omp_in.begin(), omp_out.begin(), std::plus <int>())) \
    initializer(omp_priv = decltype(omp_orig)(omp_orig.size()))

FPGrowth::FPGrowth(const Params &params) : m_logEnabled(params.isLogEnabled()),
                                           m_numThreads(params.hasNumThreads() ? params.getNumThreads() : std::thread::hardware_concurrency())
{
}

std::vector<int> FPGrowth::mine(const FPTree &fptree, int minsup) const
{
    return mineTopLevel(fptree, minsup, fptree.getItems().size() - 1, m_numThreads);
}

std::vector<int> FPGrowth::mineTopLevel(const FPTree &fptree, int minsup, int leftItem, int numThreads) const
{
    auto result = std::vector<int>(fptree.getItems().size());
#pragma omp parallel for schedule(dynamic) num_threads(numThreads) reduction(vec_int_plus \
                                                                                             : result)
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
