#include <iostream>
#include <vector>
#include <ctime>
#include <omp.h>
using namespace std;

void bubbleSort(vector<int>& a, bool parallel) {
    int n = a.size();
    for (int i = 0; i < n - 1; i++) {
        #pragma omp parallel for if(parallel)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1]) swap(a[j], a[j + 1]);
    }
}

void merge(vector<int>& a, int l, int m, int r) {
    vector<int> left(a.begin() + l, a.begin() + m + 1);
    vector<int> right(a.begin() + m + 1, a.begin() + r + 1);
    int i = 0, j = 0, k = l;
    while (i < left.size() && j < right.size())
        a[k++] = (left[i] < right[j]) ? left[i++] : right[j++];
    while (i < left.size()) a[k++] = left[i++];
    while (j < right.size()) a[k++] = right[j++];
}

void mergeSort(vector<int>& a, int l, int r, bool parallel) {
    if (l >= r) return;
    int m = (l + r) / 2;
    if (parallel) {
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSort(a, l, m, true);
            #pragma omp section
            mergeSort(a, m + 1, r, true);
        }
    } else {
        mergeSort(a, l, m, false);
        mergeSort(a, m + 1, r, false);
    }
    merge(a, l, m, r);
}

int main() {
    int n; cout << "Enter array size: "; cin >> n;
    vector<int> arr(n);
    for (int& x : arr) x = rand() % 1000;

    vector<int> temp = arr;
    double t1 = omp_get_wtime();
    bubbleSort(temp, false);
    double t2 = omp_get_wtime();
    cout << "Sequential Bubble Sort: " << t2 - t1 << "s\n";

    temp = arr;
    t1 = omp_get_wtime();
    bubbleSort(temp, true);
    t2 = omp_get_wtime();
    cout << "Parallel Bubble Sort:   " << t2 - t1 << "s\n";

    temp = arr;
    t1 = omp_get_wtime();
    mergeSort(temp, 0, n - 1, false);
    t2 = omp_get_wtime();
    cout << "Sequential Merge Sort:  " << t2 - t1 << "s\n";

    temp = arr;
    t1 = omp_get_wtime();
    mergeSort(temp, 0, n - 1, true);
    t2 = omp_get_wtime();
    cout << "Parallel Merge Sort:    " << t2 - t1 << "s\n";

    return 0;
}

