import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("benchmark_results.csv")

plt.figure(figsize=(8,5))

plt.plot(df["size"], df["quickInsertion"], label="Quick + Insertion")
plt.plot(df["size"], df["quickMerge"], label="Quick + Merge")
plt.plot(df["size"], df["quickHeap"], label="Quick + Heap")
plt.plot(df["size"], df["introsort"], label="IntroSort")

plt.xlabel("Dataset Size")
plt.ylabel("Time (ns)")
plt.title("Hybrid Sorting Algorithm Performance")
plt.legend()
plt.grid(True)

plt.savefig("benchmark_graph.png")
plt.show()