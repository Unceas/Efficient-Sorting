#include <vector>
#include <algorithm>
#include "../utils/sorting_utils.h"

namespace {
    const int MERGE_THRESHOLD = 20;

    void merge(std::vector<int>& arr, int low, int mid, int high) {
        int n1 = mid - low + 1;
        int n2 = high - mid;

        std::vector<int> left(n1);
        std::vector<int> right(n2);

        for (int i = 0; i < n1; ++i) left[i] = arr[low + i];
        for (int j = 0; j < n2; ++j) right[j] = arr[mid + 1 + j];

        int i = 0, j = 0, k = low;
        while (i < n1 && j < n2) {
            if (left[i] <= right[j]) {
                arr[k++] = left[i++];
            } else {
                arr[k++] = right[j++];
            }
        }
        while (i < n1) arr[k++] = left[i++];
        while (j < n2) arr[k++] = right[j++];
    }

    void mergeSortRange(std::vector<int>& arr, int low, int high) {
        if (low >= high) return;
        int mid = low + (high - low) / 2;
        mergeSortRange(arr, low, mid);
        mergeSortRange(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }

    void quickMergeUtil(std::vector<int>& arr, int low, int high) {
        while (low < high) {
            if (high - low + 1 < MERGE_THRESHOLD) {
                mergeSortRange(arr, low, high);
                return;
            }

            int p = partition(arr, low, high);

            if (p - 1 - low < high - (p + 1)) {
                quickMergeUtil(arr, low, p - 1);
                low = p + 1;
            } else {
                quickMergeUtil(arr, p + 1, high);
                high = p - 1;
            }
        }
    }
}

void quickMergeSort(std::vector<int>& arr) {
    if (arr.empty()) return;
    quickMergeUtil(arr, 0, static_cast<int>(arr.size()) - 1);
}
