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

    void mine(std::vector<int> &result, const FPTree &fptree, int minsup, int leftItem) const;
};
