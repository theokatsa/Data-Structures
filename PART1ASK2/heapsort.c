#include "sorting.h"
#include <stdio.h>
#include <stdlib.h>

// arr: pointer ston pinaka, n: ari8mos stoixeiwn, i: deikths tou trexontos stoixeiou
void heapify(DataEntry *arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].Cumulative > arr[largest].Cumulative) {
    largest = left;
}
    if (right < n && arr[right].Cumulative > arr[largest].Cumulative) {
    largest = right;
}

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(DataEntry *arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}
