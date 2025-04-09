#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

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

void heapify(vector<int>& arr, int n, int i, int low) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && arr[low + l] > arr[low + largest]) largest = l;
    if (r < n && arr[low + r] > arr[low + largest]) largest = r;
    if (largest != i) {
        swap(arr[low + i], arr[low + largest]);
        heapify(arr, n, largest, low);
    }
}

void heapSort(vector<int>& arr, int low, int high) {
    int n = high - low + 1;
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i, low);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[low], arr[low + i]);
        heapify(arr, i, 0, low);
    }
}

void insertionSort(vector<int>& arr, int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        int key = arr[i], j = i - 1;
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void introSortUtil(vector<int>& arr, int low, int high, int depthLimit) {
    int size = high - low + 1;
    if (size < 16) {
        insertionSort(arr, low, high);
        return;
    }
    if (depthLimit == 0) {
        heapSort(arr, low, high);
        return;
    }
    int pi = partition(arr, low, high);
    introSortUtil(arr, low, pi - 1, depthLimit - 1);
    introSortUtil(arr, pi + 1, high, depthLimit - 1);
}

void introSort(vector<int>& arr) {
    int depthLimit = 2 * log(arr.size());
    introSortUtil(arr, 0, arr.size() - 1, depthLimit);
}

int main() {
    vector<int> arr = {24, 97, 40, 67, 88, 85, 15, 23, 76, 54, 37};
    introSort(arr);
    for (int num : arr) cout << num << " ";
    cout << endl;
    return 0;
}