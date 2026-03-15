#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>

#include "sorting_utils.h"

void introSort(std::vector<int>& arr);
void quickInsertionSort(std::vector<int>& arr);
void quickMergeSort(std::vector<int>& arr);
void quickHeapSort(std::vector<int>& arr);

std::vector<int> generateRandomArray(int size, int minVal = 0, int maxVal = 1000000) {
    std::vector<int> arr(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(minVal, maxVal);

    for(int i = 0; i < size; i++)
        arr[i] = dist(gen);

    return arr;
}

long long measure(void (*sortFunc)(std::vector<int>&), const std::vector<int>& base, int iterations = 50)
{
    long long total = 0;

    for(int i = 0; i < iterations; i++)
    {
        std::vector<int> arr = base;

        auto start = std::chrono::high_resolution_clock::now();

        sortFunc(arr);

        auto end = std::chrono::high_resolution_clock::now();

        total += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }

    return total / iterations;
}

long long averageMeasure(void (*sortFunc)(std::vector<int>&), const std::vector<int>& base, int runs = 5) {

    long long total = 0;

    for(int i = 0; i < runs; i++) {

        std::vector<int> copy = base;

        total += measure(sortFunc, copy);

    }

    return total / runs;
}

std::string formatTime(long long ns)
{
    std::ostringstream out;

    if (ns < 1000)
        out << ns << " ns";
    else if (ns < 1000000)
        out << ns / 1000.0 << " µs";
    else if (ns < 1000000000)
        out << ns / 1000000.0 << " ms";
    else
        out << ns / 1000000000.0 << " s";

    return out.str();
}

int main() {

    std::vector<int> sizes = {1000, 5000, 10000, 50000, 100000};

    std::ofstream csv("benchmark_results.csv");
    csv << "size,quickInsertion,quickMerge,quickHeap,introsort\n";

    for(int SIZE : sizes) {

        std::vector<int> base = generateRandomArray(SIZE);

        long long tQuickInsertion = averageMeasure(quickInsertionSort, base);
        long long tQuickMerge     = averageMeasure(quickMergeSort, base);
        long long tQuickHeap      = averageMeasure(quickHeapSort, base);
        long long tIntro          = averageMeasure(introSort, base);

        std::cout << "\nDataset Size: " << SIZE << "\n";

        std::cout << std::left << std::setw(25) << "Algorithm"
                  << std::setw(20) << "Time (ns)" << "\n";

        std::cout << std::string(45,'-') << "\n";

        std::cout << std::left << std::setw(25) << "Quick + Insertion"
                  << formatTime(tQuickInsertion) << "\n";

        std::cout << std::left << std::setw(25) << "Quick + Merge"
                  << formatTime(tQuickMerge) << "\n";

        std::cout << std::left << std::setw(25) << "Quick + Heap"
                  << formatTime(tQuickHeap) << "\n";

        std::cout << std::left << std::setw(25) << "IntroSort"
                  << formatTime(tIntro) << "\n";

        csv << SIZE << ","
            << tQuickInsertion << ","
            << tQuickMerge << ","
            << tQuickHeap << ","
            << tIntro << "\n";
    }

    csv.close();

    std::cout << "\nBenchmark results saved to benchmark_results.csv\n";

    return 0;
}