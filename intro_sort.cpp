#include <vector>
#include <algorithm>
#include <cmath>
#include "sorting_utils.h"

namespace {

    const int INSERTION_THRESHOLD = 16;

    void heapify(std::vector<int>& arr, int low, int size, int root) {

        int largest = root;
        int left = 2 * root + 1;
        int right = 2 * root + 2;

        if (left < size && arr[low + left] > arr[low + largest])
            largest = left;

        if (right < size && arr[low + right] > arr[low + largest])
            largest = right;

        if (largest != root) {
            std::swap(arr[low + root], arr[low + largest]);
            heapify(arr, low, size, largest);
        }
    }

    void heapSortRange(std::vector<int>& arr, int low, int high) {

        int size = high - low + 1;

        for (int i = size / 2 - 1; i >= 0; --i)
            heapify(arr, low, size, i);

        for (int i = size - 1; i > 0; --i) {
            std::swap(arr[low], arr[low + i]);
            heapify(arr, low, i, 0);
        }
    }

    void introSortUtil(std::vector<int>& arr, int low, int high, int depthLimit) {

        while (low < high) {

            int size = high - low + 1;

            if (size <= INSERTION_THRESHOLD) {
                insertionSort(arr, low, high);
                return;
            }

            if (depthLimit == 0) {
                heapSortRange(arr, low, high);
                return;
            }

            int p = partition(arr, low, high);
            --depthLimit;

            // Tail recursion optimization
            if (p - 1 - low < high - (p + 1)) {
                introSortUtil(arr, low, p - 1, depthLimit);
                low = p + 1;
            }
            else {
                introSortUtil(arr, p + 1, high, depthLimit);
                high = p - 1;
            }
        }
    }

}

void introSort(std::vector<int>& arr) {

    if (arr.empty())
        return;

    int n = static_cast<int>(arr.size());

    int depthLimit = 2 * static_cast<int>(std::log2(std::max(2, n)));

    introSortUtil(arr, 0, n - 1, depthLimit);
}