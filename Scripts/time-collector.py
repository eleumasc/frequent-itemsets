import subprocess
import csv
from time import time
'''
times = dict()

sequentialwork = dict()

sequentialwork["chess"] = {"filename":"datasets/chess.dat", "versions":["v1/dist/frequent-itemsets.exe","v2/dist/frequent-itemsets.exe"], "minsup":[639,1278,1918,2557]}
sequentialwork["accidents"] = {"filename":"datasets/accidents.dat", "versions":["v1/dist/frequent-itemsets.exe","v2/dist/frequent-itemsets.exe"], "minsup":[68037,136073,204110,272146]}

for dataset in sequentialwork.keys():
    times[dataset] = []
    for version in sequentialwork[dataset]["versions"]:
        print(version)
        for minsup in sequentialwork[dataset]["minsup"]:
            if version != "v1/dist/frequent-itemsets.exe" or (version == "v1/dist/frequent-itemsets.exe" and (minsup > 140000 or dataset != "accidents")):
                print(minsup)
                trytimes = []
                for i in range(3):
                    print("#",end="")
                    start = time()
                    subprocess.run(args=[str(version), sequentialwork[dataset]["filename"], str(minsup)], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
                    total = time() - start
                    trytimes.append(total)
                mintime = min(trytimes)
                print(trytimes)
                times[dataset].append([version, minsup, mintime])

csv_columns = ['Dataset','Version','minsup','Time']
csv_file = "res_sequential.csv"
try:
    with open(csv_file, 'w') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(csv_columns)
        for k in times.keys():
            for data in times[k]:
                data.insert(0, k)
                writer.writerow(data)
except IOError:
    print("I/O error")
'''
times = dict()

parallelwork = dict()

parallelwork["chess"] = {"filename":"datasets/chess.dat", "versions":["v3/dist/frequent-itemsets.exe","v4/dist/frequent-itemsets.exe","v5/dist/frequent-itemsets.exe"],  "nthreads":[2,4,6,8,12], "minsup":[639,1278,1918,2557], "BoundItem":range(0,75,10)}
parallelwork["accidents"] = {"filename":"datasets/accidents.dat", "versions":["v3/dist/frequent-itemsets.exe","v4/dist/frequent-itemsets.exe","v5/dist/frequent-itemsets.exe"], "nthreads":[2,4,6,8,12], "minsup":[68037,136073,204110,272146], "BoundItem":range(0,486,25)}


for dataset in parallelwork.keys():
    times[dataset] = []
    for version in parallelwork[dataset]["versions"]:
        for nthreads in parallelwork[dataset]["nthreads"]:
            print(version,nthreads)
            for minsup in parallelwork[dataset]["minsup"]:
                print(minsup)
                if version == "v5/dist/frequent-itemsets.exe":
                    for seqBoundItem in parallelwork[dataset]["BoundItem"]:
                        trytimes = []
                        for i in range(3):
                            print("#",end="")
                            start = time()
                            subprocess.run(args=[str(version), parallelwork[dataset]["filename"], str(minsup), "-t", str(nthreads), "-b", str(seqBoundItem)], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
                            total = time() - start
                            trytimes.append(total)
                        mintime = min(trytimes)
                        times[dataset].append([version,nthreads, minsup, seqBoundItem, mintime])
                else:
                    trytimes = []
                    for i in range(3):
                        print("#",end="")
                        start = time()
                        subprocess.run(args=[str(version), parallelwork[dataset]["filename"], str(minsup), "-t", str(nthreads)], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
                        total = time() - start
                        trytimes.append(total)
                    mintime = min(trytimes)
                    times[dataset].append([version,nthreads, minsup, "0", mintime])


csv_columns = ['Dataset','Version','nthreads','minsup','BoundItem', 'Time']
csv_file = "res_parallel.csv"
try:
    with open(csv_file, 'w') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(csv_columns)
        for k in times.keys():
            for data in times[k]:
                data.insert(0, k)
                writer.writerow(data)
except IOError:
    print("I/O error")