#include <iostream>
#include <omp.h>
#include <limits>
#include <vector>
using namespace std;
int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter " << n << " elements:\n";
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    int minVal = numeric_limits<int>::max();
    int maxVal = numeric_limits<int>::min();
    long long sum = 0;

    // OpenMP Parallel Reduction
    #pragma omp parallel for reduction(min:minVal) reduction(max:maxVal) reduction(+:sum)
    for (int i = 0; i < n; ++i) {
        minVal = min(minVal, arr[i]);
        maxVal = max(maxVal, arr[i]);
        sum += arr[i];
    }

    float average = static_cast<float>(sum) / n;

    cout << "\nResults:\n";
    cout << "Min: " << minVal << "\n";
    cout << "Max: " << maxVal << "\n";
    cout << "Sum: " << sum << "\n";
    cout << "Average: " << average << "\n";
    
    return 0;
}
