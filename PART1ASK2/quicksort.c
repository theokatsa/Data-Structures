#include "sorting.h"
#include <stdio.h>
#include <stdlib.h>



// Ylopoioume sunarthsh gia thn allagh stoixeiwn opou xreiastei
void swap(DataEntry *a, DataEntry *b) {
  DataEntry t = *a;
  *a = *b;
  *b = t;
}

int partition(DataEntry *arr, int low, int high) {
    DataEntry pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].Cumulative < pivot.Cumulative) {
    i++;
    swap(&arr[i], &arr[j]);
}

    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(DataEntry *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
