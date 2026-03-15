#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "../utils/sorting_utils.h"

void introSort(std::vector<int>& arr);
void quickInsertionSort(std::vector<int>& arr);
void quickMergeSort(std::vector<int>& arr);
void quickHeapSort(std::vector<int>& arr);

void runBenchmark();
void runCustomTest();


// ---------------- DATASET GENERATORS ----------------

std::vector<int> generateRandomArray(int size, int minVal = 0, int maxVal = 1000000)
{
    std::vector<int> arr(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(minVal, maxVal);

    for(int i = 0; i < size; i++)
        arr[i] = dist(gen);

    return arr;
}

std::vector<int> generateNearlySortedArray(int size)
{
    std::vector<int> arr(size);

    for(int i = 0; i < size; i++)
        arr[i] = i;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, size - 1);

    for(int i = 0; i < size / 10; i++)
        std::swap(arr[dist(gen)], arr[dist(gen)]);

    return arr;
}

std::vector<int> generateReverseSortedArray(int size)
{
    std::vector<int> arr(size);

    for(int i = 0; i < size; i++)
        arr[i] = size - i;

    return arr;
}

std::vector<int> generateDuplicateHeavyArray(int size)
{
    std::vector<int> arr(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 20);

    for(int i = 0; i < size; i++)
        arr[i] = dist(gen);

    return arr;
}


// ---------------- BENCHMARK MEASUREMENT ----------------

long long measure(void (*sortFunc)(std::vector<int>&),
                  const std::vector<int>& base,
                  int iterations = 50)
{
    long long total = 0;

    for(int i = 0; i < iterations; i++)
    {
        std::vector<int> arr = base;

        auto start = std::chrono::high_resolution_clock::now();
        sortFunc(arr);
        auto end = std::chrono::high_resolution_clock::now();

        total += std::chrono::duration_cast
                 <std::chrono::nanoseconds>(end - start).count();
    }

    return total / iterations;
}

long long averageMeasure(void (*sortFunc)(std::vector<int>&),
                         const std::vector<int>& base,
                         int runs = 5)
{
    long long total = 0;

    for(int i = 0; i < runs; i++)
        total += measure(sortFunc, base);

    return total / runs;
}


// ---------------- TIME FORMAT ----------------

std::string formatTime(long long ns)
{
    std::ostringstream out;

    if(ns < 1000)
        out << ns << " ns";
    else if(ns < 1000000)
        out << ns / 1000.0 << " us";
    else if(ns < 1000000000)
        out << ns / 1000000.0 << " ms";
    else
        out << ns / 1000000000.0 << " s";

    return out.str();
}


// ---------------- BENCHMARK MODE ----------------

void runBenchmark()
{
    std::vector<int> sizes = {1000, 5000, 10000, 50000, 100000};

    std::ofstream csv("benchmark_results.csv");
    csv << "size,quickInsertion,quickMerge,quickHeap,introsort\n";

    for(int SIZE : sizes)
    {
        std::vector<int> base = generateRandomArray(SIZE);

        long long tQuickInsertion = averageMeasure(quickInsertionSort, base);
        long long tQuickMerge     = averageMeasure(quickMergeSort, base);
        long long tQuickHeap      = averageMeasure(quickHeapSort, base);
        long long tIntro          = averageMeasure(introSort, base);

        csv << SIZE << ","
            << tQuickInsertion << ","
            << tQuickMerge << ","
            << tQuickHeap << ","
            << tIntro << "\n";


        std::vector<std::pair<std::string,long long>> results =
        {
            {"Quick + Insertion", tQuickInsertion},
            {"Quick + Merge",     tQuickMerge},
            {"Quick + Heap",      tQuickHeap},
            {"IntroSort",         tIntro}
        };

        std::sort(results.begin(), results.end(),
                  [](auto &a, auto &b){ return a.second < b.second; });

        long long fastest = results[0].second;

        std::cout << "\nDataset Size: " << SIZE << "\n\n";

        std::cout << std::left << std::setw(6)  << "Rank"
                  << std::setw(20) << "Algorithm"
                  << std::setw(15) << "Time"
                  << "Relative\n";

        std::cout << std::string(50,'-') << "\n";

        for(int i = 0; i < results.size(); i++)
        {
            double ratio = (double)results[i].second / fastest;

            std::cout << std::left
                      << std::setw(6)  << i+1
                      << std::setw(20) << results[i].first
                      << std::setw(15) << formatTime(results[i].second)
                      << std::fixed << std::setprecision(2)
                      << ratio << "x\n";
        }
    }

    csv.close();

    std::cout << "\nBenchmark results saved to benchmark_results.csv\n";
}


// ---------------- INTERACTIVE MODE ----------------

void runCustomTest()
{
    while(true)
    {
        int n;

        std::cout << "\nEnter array size: ";
        std::cin >> n;

        std::cout << "\nChoose dataset type\n";
        std::cout << "1 Random\n";
        std::cout << "2 Nearly Sorted\n";
        std::cout << "3 Reverse Sorted\n";
        std::cout << "4 Duplicate Heavy\n";

        int dataset;
        std::cin >> dataset;

        std::vector<int> arr;

        if(dataset == 1)
            arr = generateRandomArray(n);
        else if(dataset == 2)
            arr = generateNearlySortedArray(n);
        else if(dataset == 3)
            arr = generateReverseSortedArray(n);
        else
            arr = generateDuplicateHeavyArray(n);


        std::cout << "\nChoose algorithm\n";
        std::cout << "1 IntroSort\n";
        std::cout << "2 Quick + Heap\n";
        std::cout << "3 Quick + Merge\n";
        std::cout << "4 Quick + Insertion\n";

        int choice;
        std::cin >> choice;

        auto start = std::chrono::high_resolution_clock::now();

        if(choice == 1) introSort(arr);
        else if(choice == 2) quickHeapSort(arr);
        else if(choice == 3) quickMergeSort(arr);
        else if(choice == 4) quickInsertionSort(arr);

        auto end = std::chrono::high_resolution_clock::now();

        long long time =
            std::chrono::duration_cast
            <std::chrono::nanoseconds>(end - start).count();

        if(!std::is_sorted(arr.begin(), arr.end()))
            std::cout << "Sorting failed!\n";

        std::cout << "\nExecution Time: " << formatTime(time) << "\n";

        char again;
        std::cout << "\nRun another test? (y/n): ";
        std::cin >> again;

        if(again != 'y' && again != 'Y')
            break;
    }
}


// ---------------- MAIN ----------------

int main()
{
    std::cout << "Select Mode\n";
    std::cout << "1 Run Benchmark Study\n";
    std::cout << "2 Custom Sorting Test\n";

    int mode;
    std::cin >> mode;

    if(mode == 1)
        runBenchmark();
    else
        runCustomTest();

    return 0;
}