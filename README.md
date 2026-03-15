# 🚀 Hybrid Sorting Algorithms in C++

This repository contains the implementation and analysis of Hybrid Sorting Algorithms, focusing on combinations of classical sorting techniques such as QuickSort, Insertion Sort, Merge Sort, and HeapSort. The project culminates in the implementation of Introsort, an adaptive three-way hybrid that ensures both performance and reliability.

## 📌 Project Highlights

- ✅ Implemented in C++
- 🔁 Three Hybrid Models:
  - QuickSort + Insertion Sort
  - QuickSort + Merge Sort
  - QuickSort + HeapSort

  - Introsort: 3 way merge.
    

- 📊 Performance Analysis on:
  - Random arrays
  - Nearly sorted arrays
  - Reverse sorted arrays
  - Datasets with duplicates

## 🧑‍💻 Authors
- Ayush Kushwaha
- Gaurav Kumar Pandey
- Azaan Moiz

## Benchmark Results

The algorithms were benchmarked across multiple dataset sizes.

![Benchmark Graph](benchmark_graph.png)

### Observations

• IntroSort consistently performs best due to adaptive switching.
• Quick + Heap provides stable worst-case behavior.
• Quick + Merge is slower due to additional memory overhead.
• Quick + Insertion performs well on smaller partitions.
