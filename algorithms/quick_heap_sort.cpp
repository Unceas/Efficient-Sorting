#include <vector>
#include <algorithm>
#include "../utils/sorting_utils.h"

namespace {
    const int HEAP_THRESHOLD = 20;

    void heapify(std::vector<int>& arr, int low, int high, int root) {
        int size = high - low + 1;
        int largest = root;
        int left = 2 * root + 1;
        int right = 2 * root + 2;

        if (left < size && arr[low + left] > arr[low + largest]) {
            largest = left;
        }
        if (right < size && arr[low + right] > arr[low + largest]) {
            largest = right;
        }
        if (largest != root) {
            std::swap(arr[low + root], arr[low + largest]);
            heapify(arr, low, high, largest);
        }
    }

    void heapSortRange(std::vector<int>& arr, int low, int high) {
        int size = high - low + 1;
        for (int i = size / 2 - 1; i >= 0; --i) {
            heapify(arr, low, high, i);
        }
        for (int i = size - 1; i > 0; --i) {
            std::swap(arr[low], arr[low + i]);
            heapify(arr, low, low + i - 1, 0);
        }
    }

    void quickHeapUtil(std::vector<int>& arr, int low, int high) {
        while (low < high) {
            if (high - low + 1 < HEAP_THRESHOLD) {
                heapSortRange(arr, low, high);
                return;
            }

            int p = partition(arr, low, high);

            if (p - 1 - low < high - (p + 1)) {
                quickHeapUtil(arr, low, p - 1);
                low = p + 1;
            } else {
                quickHeapUtil(arr, p + 1, high);
                high = p - 1;
            }
        }
    }
}

void quickHeapSort(std::vector<int>& arr) {
    if (arr.empty()) return;
    quickHeapUtil(arr, 0, static_cast<int>(arr.size()) - 1);
}
