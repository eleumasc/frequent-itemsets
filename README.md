# frequent-itemsets

Sequential and parallel implementation of FP-Growth for solving the Frequent Itemsets Mining problem.

Made by Samuele Casarin (@eleumasc) and Roberto Perissa (@RPComputer)

## Summary

 - v1: sequential implementation of FPGrowth as in literature
 - v2: optimization through support memoization and header removal
 - v2.5: optimization through memory reduction (tree projection using _climbing algorithm_) \[FINAL SEQUENTIAL\]
 - v3: parallel implementation by parallelizing top-level itemsets
 - v4: parallel implementation with static task creation
 - v5: parallel implementation with dynamic task creation \[FINAL PARALLEL\]

## Build

 - Enter in the folder of a version (es. `v1/`)
 - Build with `make`
 - Execute with `dist/frequent-itemsets <input-filename> <minsup> [-f] [-t <numThreads>] [-b <seqBound>]`, where:
   - `<input-filename>` is the path to the dataset
   - `<minsup>` is the minimum support threshold
   - if `-f` is specified, then all the frequent itemsets are written to stdout
   - \[only for v3, v4, v5\] `-t <numThreads>` sets the number of threads to `<numThreads>`; if not specified, the default value `std::thread::hardware_concurrency()` is used
   - \[only for v5\] `-b <seqBound>` sets the sequential bound to `<seqBound>`; if not specified, the default value `20` is used
