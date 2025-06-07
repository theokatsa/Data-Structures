#ifndef SORTING_H
#define SORTING_H

#include <stdlib.h>
#include <stdio.h>
#include "dataentry.h"

// Heap Sort functions
void heapify(DataEntry *arr, int n, int i);
void heapSort(DataEntry *arr, int n);

// Quick Sort functions
int partition(DataEntry *arr, int low, int high);
void quickSort(DataEntry *arr, int low, int high);

// Common functions
void swap(DataEntry *a, DataEntry *b);


#endif // SORTING_H

