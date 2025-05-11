#include <iostream>
#include <omp.h>
#include <limits>
#include <vector>

int main() {
    int n;
    std::cout << "Enter number of elements: ";
    std::cin >> n;

    std::vector<int> arr(n);
    std::cout << "Enter " << n << " elements:\n";
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    int minVal = std::numeric_limits<int>::max();
    int maxVal = std::numeric_limits<int>::min();
    long long sum = 0;

    // OpenMP Parallel Reduction
    #pragma omp parallel for reduction(min:minVal) reduction(max:maxVal) reduction(+:sum)
    for (int i = 0; i < n; ++i) {
        minVal = std::min(minVal, arr[i]);
        maxVal = std::max(maxVal, arr[i]);
        sum += arr[i];
    }

    float average = static_cast<float>(sum) / n;

    std::cout << "\nResults:\n";
    std::cout << "Min: " << minVal << "\n";
    std::cout << "Max: " << maxVal << "\n";
    std::cout << "Sum: " << sum << "\n";
    std::cout << "Average: " << average << "\n";

    return 0;
}
