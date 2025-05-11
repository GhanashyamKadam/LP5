#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void quickSort(int arr[], int left, int right) {
    if (left < right) {
        int pivot = arr[right], i = left - 1;
        for (int j = left; j < right; ++j)
            if (arr[j] <= pivot) swap(arr[++i], arr[j]);
        swap(arr[i + 1], arr[right]);
        int pi = i + 1;
        quickSort(arr, left, pi - 1);
        quickSort(arr, pi + 1, right);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size, n;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int* fullArray = nullptr;
    if (rank == 0) {
        cout << "Enter number of elements: ";
        cin >> n;
        fullArray = new int[n];
        cout << "Enter elements:\n";
        for (int i = 0; i < n; ++i) cin >> fullArray[i];
    }

    // Broadcast n to all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int chunk = n / size;
    int* subArray = new int[chunk];

    MPI_Scatter(fullArray, chunk, MPI_INT, subArray, chunk, MPI_INT, 0, MPI_COMM_WORLD);
    quickSort(subArray, 0, chunk - 1);
    MPI_Gather(subArray, chunk, MPI_INT, fullArray, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        sort(fullArray, fullArray + n);  // simple merge
        cout << "Sorted array:\n";
        for (int i = 0; i < n; ++i) cout << fullArray[i] << " ";
        cout << endl;
        delete[] fullArray;
    }

    delete[] subArray;
    MPI_Finalize();
    return 0;
}
