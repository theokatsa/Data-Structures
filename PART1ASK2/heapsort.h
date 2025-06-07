#ifndef HEAPSORT_H
#define HEAPSORT_H

#include <stdlib.h>
#include <stdio.h>
#include "dataentry.h"

void heapify(int arr[], int size, int root);
void swap(int *a, int *b);
void heapSort(int arr[], int size);
void printArray(int arr[], int size);

#endif // HEAPSORT_H
