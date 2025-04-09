#include <iostream>
#include <vector>
using namespace std;

void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int i = 0; i < n2; i++) R[i] = arr[m + 1 + i];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

int partition(vector<int>& arr, int low, int high) {
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

void hybridQuickMerge(vector<int>& arr, int low, int high) {
    const int threshold = 10;
    if (low < high) {
        if (high - low + 1 < threshold) {
            mergeSort(arr, low, high);
        } else {
            int pi = partition(arr, low, high);
            hybridQuickMerge(arr, low, pi - 1);
            hybridQuickMerge(arr, pi + 1, high);
        }
    }
}

int main() {
    vector<int> arr = {24, 97, 40, 67, 88, 85, 15, 23, 76, 54, 37};
    hybridQuickMerge(arr, 0, arr.size() - 1);
    for (int num : arr) cout << num << " ";
    cout << endl;
    return 0;
}