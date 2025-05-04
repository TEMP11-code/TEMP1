#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <cstdlib>
#include <limits>

using namespace std;

int main() {
    const int SIZE = 10;
    vector<int> arr(SIZE);

    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % 100;
    }

    // Sequential Operations
    auto start = chrono::high_resolution_clock::now();
    int min_val = numeric_limits<int>::max();
    int max_val = numeric_limits<int>::min();
    long long sum = 0;

    for (int i = 0; i < SIZE; i++) {
        if (arr[i] < min_val) min_val = arr[i];
        if (arr[i] > max_val) max_val = arr[i];
        sum += arr[i];
    }
    double avg = static_cast<double>(sum) / SIZE;
    auto end = chrono::high_resolution_clock::now();

    cout << "Sequential Min: " << min_val << "\n";
    cout << "Sequential Max: " << max_val << "\n";
    cout << "Sequential Sum: " << sum << "\n";
    cout << "Sequential Avg: " << avg << "\n";
    cout << "Sequential Execution Time: " << chrono::duration<double>(end - start).count() << " sec\n";

    // Parallel Operations using OpenMP Reduction
    start = chrono::high_resolution_clock::now();
    int p_min = numeric_limits<int>::max();
    int p_max = numeric_limits<int>::min();
    long long p_sum = 0;

    #pragma omp parallel for reduction(min:p_min) reduction(max:p_max) reduction(+:p_sum)
    for (int i = 0; i < SIZE; i++) {
        p_min = min(p_min, arr[i]);
        p_max = max(p_max, arr[i]);
        p_sum += arr[i];
    }
    double p_avg = static_cast<double>(p_sum) / SIZE;
    end = chrono::high_resolution_clock::now();

    cout << "Parallel Min: " << p_min << "\n";
    cout << "Parallel Max: " << p_max << "\n";
    cout << "Parallel Sum: " << p_sum << "\n";
    cout << "Parallel Avg: " << p_avg << "\n";
    cout << "Parallel Execution Time: " << chrono::duration<double>(end - start).count() << " sec\n";

    return 0;
}