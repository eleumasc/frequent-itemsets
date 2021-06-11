COMMON=../common

build:
	mkdir -p dist
	g++ -O3 -o dist/frequent-itemsets -fopenmp -I${COMMON} -Isrc/ ${COMMON}/main.cpp ${COMMON}/Transactions.cpp src/BitRange.cpp src/FPTree.cpp src/FPGrowth.cpp src/Params.cpp
