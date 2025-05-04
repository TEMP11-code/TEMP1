#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <omp.h>
#include <algorithm> // For std::swap

using namespace std;

void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        #pragma omp parallel for
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                #pragma omp critical
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    while (j <= right) {
        temp[k++] = arr[j++];
    }
    for (int i = left, k = 0; i <= right; ++i, ++k) {
        arr[i] = temp[k];
    }
}

void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    sequentialMergeSort(arr, left, mid);
    sequentialMergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;

    #pragma omp parallel sections
    {
        #pragma omp section
        parallelMergeSort(arr, left, mid);
        #pragma omp section
        parallelMergeSort(arr, mid + 1, right);
    }
    merge(arr, left, mid, right);
}

int main() {
    const int SIZE = 10;
    vector<int> arr1(SIZE), arr2(SIZE), arr3(SIZE), arr4(SIZE);
    srand(time(nullptr)); // Seed the random number generator
    for (int i = 0; i < SIZE; ++i) {
        int val = rand() % 100;
        arr1[i] = val;
        arr2[i] = val;
        arr3[i] = val;
        arr4[i] = val;
    }
    cout << "Array elements are: \n";
    for (int i = 0; i < SIZE; ++i) {
        cout << arr1[i] << "\t";
    }
    cout << "\n";

    // Sequential Bubble Sort
    auto start = chrono::high_resolution_clock::now();
    sequentialBubbleSort(arr1);
    auto end = chrono::high_resolution_clock::now();
    cout << "Sequential Bubble Sort Time: " << chrono::duration<double>(end - start).count() << " sec\n";

    // Parallel Bubble Sort
    start = chrono::high_resolution_clock::now();
    parallelBubbleSort(arr2);
    end = chrono::high_resolution_clock::now();
    cout << "Parallel Bubble Sort Time: " << chrono::duration<double>(end - start).count() << " sec\n";

    // Sequential Merge Sort
    start = chrono::high_resolution_clock::now();
    sequentialMergeSort(arr3, 0, SIZE - 1);
    end = chrono::high_resolution_clock::now();
    cout << "Sequential Merge Sort Time: " << chrono::duration<double>(end - start).count() << " sec\n";

    // Parallel Merge Sort
    start = chrono::high_resolution_clock::now();
    parallelMergeSort(arr4, 0, SIZE - 1);
    end = chrono::high_resolution_clock::now();
    cout << "Parallel Merge Sort Time: " << chrono::duration<double>(end - start).count() << " sec\n";

    cout << "After Sorting Array elements are:\n";
    for (int i = 0; i < SIZE; ++i) {
        cout << arr1[i] << "\t";
    }
    cout << "\n";
    return 0;
}