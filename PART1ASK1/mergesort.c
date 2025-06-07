#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mergesort.h"
#include "dataentry.h"


    void Merge(DataEntry *arr, int low, int mid, int high) {
        int leftSize = mid - low + 1;
        int rightSize = high - mid;

        // Dhmiourgoume proswrinous pinakes gia aristera kai dexia subarrays
        DataEntry *left = malloc(leftSize * sizeof(DataEntry));
        DataEntry *right = malloc(rightSize * sizeof(DataEntry));

        // Antigrafh dedomenwn stous proswrinous pinakes
        for (int i = 0; i < leftSize; i++) {
            left[i] = arr[low + i];
        }
        for (int j = 0; j < rightSize; j++) {
            right[j] = arr[mid + 1 + j];
        }

        // Enwnoume tous proswrinous pinakes
        int i = 0; // Deikths gia ton aristero pinaka
        int j = 0; // Deikths gia ton dexio pinaka
        int k = low; // Deikths gia ton enwmeno pinaka

        while (i < leftSize && j < rightSize) {
            // Sugkrish ton values kai topo8ethsh tou mikroterou sto arr
            if (left[i].Value <= right[j].Value) {
                arr[k] = left[i];
                i++;
            } else {
                arr[k] = right[j];
                j++;
            }
            k++;
        }

        // Antigrafh twn enapomeinantwn stoixeiwn apo ton aristero upopinaka
        while (i < leftSize) {
            arr[k] = left[i];
            i++;
            k++;
        }

        // Antigrafh twn enapomeinantwn stoixeiwn apo ton dexio upopinaka
        while (j < rightSize) {
            arr[k] = right[j];
            j++;
            k++;
        }

        // Eleu8erwnoume tous upopinakes
        free(left);
        free(right);
    }
    void MergeSort(DataEntry *arr, int low, int high) {
        if (low < high) {
            int mid = low + (high - low) / 2;

            // Taksinomhshs twn aristerwn kai dexiwn miswn tou pinaka
            MergeSort(arr, low, mid);
            MergeSort(arr, mid + 1, high);

            // Taksinomhsh tou merged pinaka
            Merge(arr, low, mid, high);
        }
    }
