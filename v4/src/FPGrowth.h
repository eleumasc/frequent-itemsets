#pragma once

#include <vector>

#include "FPTree.h"
#include "Params.h"

class FPGrowth
{
public:
    FPGrowth(const Params &params);

    std::vector<int> mine(const FPTree &fptree, int minsup) const;

private:
    bool m_logEnabled;

    int m_numThreads;

    void mine(std::vector<std::vector<int>> &partResult, const FPTree &fptree, int minsup, int item, int numThreads) const;
};
