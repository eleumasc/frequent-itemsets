# frequent-itemsets

Sequential and parallel implementation of FPGrowth for solving the Frequent Itemsets Mining problem.

Made by Samuele Casarin (@eleumasc) and Roberto Perissa (@RPComputer)

## Summary

 - v1: sequential implementation of FPGrowth as in literature
 - v2: optimization through support memoization and header removal
 - v2.5: optimization through memory reduction (tree projection using _climbing algorithm_) \[final sequential\]
 - v3: parallel implementation by parallelizing top-level itemsets
 - v4: parallel implementation with static task creation
 - v5: parallel implementation with dynamic task creation \[final parallel\]

## Build

 - Enter in the folder of a version (es. `v1/`)
 - Build with `make`
 - Execute with `dist/frequent-itemsets <input-filename> <minsup> ...` (the last args vary from one version to the other)
