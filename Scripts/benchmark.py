import csv
import os
from pathlib import Path
import subprocess
import time


NUM_THREADS = [2, 3, 4, 6, 8]  # [2, 4, 6, 8, 12]

benchmark_info = [
    {
        "dataset": "chess",
        "filename": Path("datasets/chess.dat"),
        "ts_count": 3196,
        "minsup_%": [20, 40, 60, 80],
        "numThreads": NUM_THREADS,
        "seqBound": sorted(list(dict.fromkeys(list(range(0, 75, 10)) + list(range(0, 30, 2)))))
    },
    {
        "dataset": "accidents",
        "filename": Path("datasets/accidents.dat"),
        "ts_count": 340183,
        "minsup_%": [10, 20, 40, 60, 80],
        "numThreads": NUM_THREADS,
        "seqBound": sorted(list(dict.fromkeys(list(range(0, 486, 25)) + list(range(0, 30, 2)))))
    }
]


def path_for_dist(version):
    binary_path = \
        Path("dist/frequent-itemsets") if os.name != 'nt' else Path("dist/frequent-itemsets.exe")
    return str(Path(version).joinpath(binary_path))


def absolute_minsup(minsup_r, ts_count):
    return (minsup_r * ts_count) // 100


def benchmark(args, n=3):
    print(args)
    try:
        samples = []
        for i in range(n):
            tic = time.time()
            subprocess.run(args=args, stdout=subprocess.DEVNULL,
                           stderr=subprocess.DEVNULL, timeout=1200)  # timeout after 20 minutes
            toc = time.time()
            samples.append(toc - tic)
            print("#", end="", flush=True)
        print()
        return min(samples)
    except subprocess.TimeoutExpired:
        return -1


def benchmark_1(version, info):
    for minsup_r in info["minsup_%"]:
        minsup = absolute_minsup(minsup_r, info["ts_count"])
        t = benchmark([
            path_for_dist(version),
            str(info["filename"]),
            str(minsup)
        ])
        yield [t, version, info["dataset"], minsup_r]


def benchmark_2(version, info):
    for minsup_r in info["minsup_%"]:
        minsup = absolute_minsup(minsup_r, info["ts_count"])
        for numThreads in info["numThreads"]:
            t = benchmark([
                path_for_dist(version),
                str(info["filename"]),
                str(minsup),
                "-t", str(numThreads)
            ])
            yield [t, version, info["dataset"], minsup_r, numThreads]


def benchmark_3(version, info):
    for minsup_r in info["minsup_%"]:
        minsup = absolute_minsup(minsup_r, info["ts_count"])
        for numThreads in info["numThreads"]:
            for seqBound in info["seqBound"]:
                t = benchmark([
                    path_for_dist(version),
                    str(info["filename"]),
                    str(minsup),
                    "-t", str(numThreads),
                    "-b", str(seqBound)
                ])
                yield [t, version, info["dataset"], minsup_r, numThreads, seqBound]


def benchmark_v1(info):
    yield from benchmark_1("v1", info)


def benchmark_v2(info):
    yield from benchmark_1("v2", info)


def benchmark_v25(info):
    yield from benchmark_1("v2.5", info)


def benchmark_v3(info):
    yield from benchmark_2("v3", info)


def benchmark_v4(info):
    yield from benchmark_2("v4", info)


def benchmark_v5(info):
    yield from benchmark_3("v5", info)


def benchmark_all():
    for info in benchmark_info:
        yield from benchmark_v1(info)
        yield from benchmark_v2(info)
        yield from benchmark_v25(info)
        yield from benchmark_v3(info)
        yield from benchmark_v4(info)
        yield from benchmark_v5(info)


def report_benchmark(bench, filename):
    with open(filename, 'w') as file:
        writer = csv.writer(file)
        writer.writerow(["time", "version", "dataset",
                        "minsup", "numThreads", "seqBound"])
        for row in bench:
            print(row)
            writer.writerow(row)


report_benchmark(benchmark_all(), "res_all.csv")