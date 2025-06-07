#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "countingsort.h"
#include "dataentry.h"

void CountingSort(DataEntry* arr, int n, int maxValue)
{
    // Dhmiourgoume ena count array gia na apo8hkeuoume tis times
    int* count = malloc((maxValue + 1) * sizeof(int));
    if (count == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    // Arxikopoioume to count array me 0
    for (int i = 0; i <= maxValue; i++)
    {
        count[i] = 0;
    }

    // Exagwgh ths timhs
    for (int i = 0; i < n; i++)
    {
        int value = arr[i].Value;
        count[value]++;
    }

    // Enhmerwsh tou count array gia na apo8hkeuei to sunolo twn metrhsewn
    for (int i = 1; i <= maxValue; i++)
    {
        count[i] += count[i - 1];
    }

    // Dhmiourgoume enan proswrino pinaka gia na apo8hkeuontai oi taxinomhmenes times
    DataEntry* sortedArray = malloc(n * sizeof(DataEntry));
    if (sortedArray == NULL)
    {
        printf("Memory allocation failed\n");
        free(count);
        return;
    }

    // Gemizoume ton taxinomhmeno pinaka se auxousa seira
    for (int i = n - 1; i >= 0; i--)
    {
        int value = arr[i].Value;
        int index = count[value] - 1;
        sortedArray[index] = arr[i];
        count[value]--;
    }

    // Antigrafoume ta stoixeia ston arxiko pinaka
    memcpy(arr, sortedArray, n * sizeof(DataEntry));

    // Eleu8erwnoume th mnhmh
    free(sortedArray);
    free(count);
}

