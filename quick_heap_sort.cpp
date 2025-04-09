#include <iostream>
#include <vector>
using namespace std;

void heapify(vector<int>& arr, int n, int i) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr, int low, int high) {
    int n = high - low + 1;
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
    for (int i = 0; i < n; i++) swap(arr[low + i], arr[i]);
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high], i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void hybridQuickHeap(vector<int>& arr, int low, int high) {
    const int threshold = 10;
    if (low < high) {
        if (high - low + 1 < threshold) {
            heapSort(arr, low, high);
        } else {
            int pi = partition(arr, low, high);
            hybridQuickHeap(arr, low, pi - 1);
            hybridQuickHeap(arr, pi + 1, high);
        }
    }
}

int main() {
    vector<int> arr = {24, 97, 40, 67, 88, 85, 15, 23, 76, 54, 37};
    hybridQuickHeap(arr, 0, arr.size() - 1);
    for (int num : arr) cout << num << " ";
    cout << endl;
    return 0;
}