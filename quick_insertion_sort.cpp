#include <iostream>
using namespace std;

void insertionSort(int arr[], int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void hybridQuickSort(int arr[], int low, int high) {
    const int threshold = 10;
    while (low < high) {
        if (high - low + 1 < threshold) {
            insertionSort(arr, low, high);
            break;
        } else {
            int pi = partition(arr, low, high);
            if (pi - low < high - pi) {
                hybridQuickSort(arr, low, pi - 1);
                low = pi + 1;
            } else {
                hybridQuickSort(arr, pi + 1, high);
                high = pi - 1;
            }
        }
    }
}

int main() {
    int arr[] = {24, 97, 40, 67, 88, 85, 15, 23, 76, 54, 37};
    int n = sizeof(arr) / sizeof(arr[0]);
    hybridQuickSort(arr, 0, n - 1);
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
    return 0;
}