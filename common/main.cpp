#include <iostream>
#include <chrono>

#include "FPGrowth.h"
#include "FPTree.h"
#include "Params.h"
#include "Transactions.h"

int main(int argc, char **argv)
{
    auto params = Params::parseRaw(argc, argv);
    if (!params.isGood())
    {
        std::cout << "Usage: " << Params::getUsage(argc, argv) << std::endl;
        return 1;
    }

    auto tic = std::chrono::steady_clock::now();

    FPTree fptree;
    {
        std::ifstream inputFile(params.getInputFilename());
        if (inputFile)
        {
            auto ts = Transactions::fromFile(inputFile);
            ts.sortByFrequency();
            fptree = FPTree::make(ts);
        }
        else
        {
            std::cout << "Cannot open file: " << params.getInputFilename() << std::endl;
            return 1;
        }
    }

    FPGrowth fpgrowth(params);
    auto result = fpgrowth.mine(fptree, params.getMinsup());

    auto toc = std::chrono::steady_clock::now();

    std::cout << "card.\tcount" << std::endl;
    for (int i = 0; i < result.size() && result[i] > 0; ++i)
    {
        std::cout << (i + 1) << "\t" << result[i] << std::endl;
    }
    std::cout << "exec. time: " << std::chrono::duration<double>(toc - tic).count() << "s" << std::endl;

    return 0;
}
