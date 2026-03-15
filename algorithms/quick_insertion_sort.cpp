#include <vector>
#include "../utils/sorting_utils.h"

namespace {
    const int INSERTION_THRESHOLD = 10;

    void quickInsertionUtil(std::vector<int>& arr, int low, int high) {
        while (low < high) {
            if (high - low + 1 < INSERTION_THRESHOLD) {
                insertionSort(arr, low, high);
                return;
            }

            int p = partition(arr, low, high);

            // Recurse on smaller side first for better stack usage
            if (p - 1 - low < high - (p + 1)) {
                quickInsertionUtil(arr, low, p - 1);
                low = p + 1;
            } else {
                quickInsertionUtil(arr, p + 1, high);
                high = p - 1;
            }
        }
    }
}

void quickInsertionSort(std::vector<int>& arr) {
    if (arr.empty()) return;
    quickInsertionUtil(arr, 0, static_cast<int>(arr.size()) - 1);
}
