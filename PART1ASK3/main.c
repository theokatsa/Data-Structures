#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataentry.h"
// 8ewrh8hke kaluterh h ulopoihsh twn dataentry se ksexwristo .h files opote 8a prepei na uparxei entos tou arxeiou ths askhshs
#define INITIAL_CAPACITY 1000 // Arxikh xwrhtikothta gia ton pinaka entries
#define CAPACITY_INCREMENT_PERCENT 50

// Xrhsimopoioume thn resizeEntries wste na desmeusoume thn aparaithth mnhmh gia tis kataxwrhseis tou pinaka
void resizeEntries(DataEntry **entries, int *capacity) {
    int newCapacity = (*capacity) + ((*capacity) * CAPACITY_INCREMENT_PERCENT) / 100;
    DataEntry *newEntries = realloc(*entries, newCapacity * sizeof(DataEntry));
    if (newEntries == NULL) {
        printf("Memory reallocation failed for entries array\n");
        // Handle the error
    } else {
        *entries = newEntries;
        *capacity = newCapacity;
    }
}
// Ylopoioume thn countLines giati 8a mas xreiastei argotera otan 8elhsoume to programma mas na diabasei to csv file
int countLines(FILE *file) {
    int count = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    rewind(file);
    return count;
}

// Ylopoioume thn sunarthsh compareDates gia na dwsoume sto programma tis katallhles entoles wste na mporei na sugkrinei 2 hmeromhnies ths morfhs DD/MM/YYYY
// metatrepontas tes se morfh YYYYMMDD
int compareDates(const char *date1, const char *date2) {
    // Compare two dates in the format "DD/MM/YYYY"
    int day1, month1, year1;
    int day2, month2, year2;
    sscanf(date1, "%d/%d/%d", &day1, &month1, &year1);
    sscanf(date2, "%d/%d/%d", &day2, &month2, &year2);

    if (year1 < year2) {
        return -1;
    } else if (year1 > year2) {
        return 1;
    } else {
        if (month1 < month2) {
            return -1;
        } else if (month1 > month2) {
            return 1;
        } else {
            if (day1 < day2) {
                return -1;
            } else if (day1 > day2) {
                return 1;
            } else {
                return 0;
            }
        }
    }
}

// Orizoume thn quicksort gia na taksinomhsoume to arxeio mas kata auksousa seira date, kati to opoio einai aparaithto gia to Binary Search
void quicksort(DataEntry *arr, int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        quicksort(arr, low, pivot - 1);
        quicksort(arr, pivot + 1, high);
    }
}

int partition(DataEntry *arr, int low, int high) {
    DataEntry pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (compareDates(arr[j].Date, pivot.Date) <= 0) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void swap(DataEntry *a, DataEntry *b) {
    DataEntry temp = *a;
    *a = *b;
    *b = temp;
}

// Ylopoioume ton binarySearch algori8mo opws fainetai parakatw
int binarySearch(DataEntry *entries, int low, int high, const char *searchDate) {
    int foundIndex = -1;
    while (low <= high) {
        int mid = (low + high) / 2;
        int comparison = compareDates(entries[mid].Date, searchDate);
        if (comparison == 0) {
            foundIndex = mid; // Bre8hke tairiasma, apo8hkeush tou deikth
            // Sunexise na psaxneis kai sta 2 misa tou pinaka logw epanalhpshs twn hmeromhniwn
            high = mid - 1; // Psakse sto aristero miso
        } else if (comparison < 0) {
            low = mid + 1; // Psakse sto dexi miso
        } else {
            high = mid - 1; // Psakse sto aristero miso
        }
    }
    return foundIndex; // Epestrepse ton deixth pou bre8hke alliws -1
}

// Ylopoioume ton interpolationSearch algori8mo opws fainetai parakatw
int interpolationSearch(DataEntry *entries, int low, int high, const char *searchDate) {
    int foundIndex = -1;
    while (low <= high && compareDates(entries[low].Date, entries[high].Date) != 0 && compareDates(searchDate, entries[low].Date) >= 0 && compareDates(searchDate, entries[high].Date) <= 0) {
        int pos = low + ((high - low) * (compareDates(searchDate, entries[low].Date))) / (compareDates(entries[high].Date, entries[low].Date));

        int comparison = compareDates(entries[pos].Date, searchDate);
        if (comparison == 0) {
            foundIndex = pos; // Bre8hke tairiasma, apo8hkeush tou deikth
            // Sunexise na psaxneis kai sta 2 misa tou pinaka logw epanalhpshs twn hmeromhniwn
            high = pos - 1; //Psakse sto dexi miso
        } else if (comparison < 0) {
            low = pos + 1; //Psakse sto aristero miso
        } else {
            high = pos - 1; //Psakse sto dexi miso
        }
    }
    return foundIndex; // Epestrepse ton deixth pou bre8hke alliws -1
}

// Ylopoioume thn countEntries gia na emfanisoume sto telos to sunolo twn epanalhpsewn mias hmeromhnias
int countEntries(DataEntry *entries, int entryCount, const char *searchDate) {
    int count = 0;
    for (int i = 0; i < entryCount; i++) {
        if (compareDates(entries[i].Date, searchDate) == 0) {
            count++;
        }
    }
    return count;
}

// Paromoiws, to effects.csv arxeio einai aparaithto na uparxei entos tou fakelou, egine allagh tou onomatos se effects.csv gia logous eukolias ston kwdika
int main() {
    FILE *file = fopen("effects.csv", "r");
    if (file == NULL) {
        printf("Error opening effects.csv\n");
        return 1;
    }

    char headers[256];
        // Dinoume sto programma thn parakatw entolh gia na agnohsei tis epikefalides sto arxeio excel
    if (fgets(headers, sizeof(headers), file) == NULL) {
        printf("Error reading headers\n");
        fclose(file);
        return 1;
    }

    char line[256];
    int entryCount = 0;
    int entryCapacity = INITIAL_CAPACITY;
    // Desmeuoume dunamika me th xrhsh ths malloc mnhmh gia th domh mas
    DataEntry *entries = malloc(entryCapacity * sizeof(DataEntry));
    if (entries == NULL) {
        printf("Memory allocation failed for entries array\n");
        fclose(file);
        return 1;
    }

    while (fgets(line, sizeof(line), file)) {
        DataEntry entry;
        int result;
        // Xrhsimopoioume auth thn if dioti antimetwpizame problhma sthn anagnwsh sugkekrimenwn xarakthrwn, epomenws dhmiourgoume duo diaforetikes sun8hkes diabasmatos
        if (strchr(line, '\"') != NULL) {
            result = sscanf(line, " %[^,],%d,%[^,],%[^,],%[^,],\"%[^\"]\",%[^,],%[^,],%d,%u",
                            entry.Direction, &entry.Year, entry.Date, entry.Weekday,
                            entry.Country, entry.Commodity, entry.Transport_mode,
                            entry.Measure, &entry.Value, &entry.Cumulative);
        } else {
            result = sscanf(line, " %[^,],%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%u",
                            entry.Direction, &entry.Year, entry.Date, entry.Weekday,
                            entry.Country, entry.Commodity, entry.Transport_mode,
                            entry.Measure, &entry.Value, &entry.Cumulative);
        }

        if (result != 10) {
            printf("Error reading data from line: %s\n", line);
            fclose(file);
            free(entries);
            return 1;
        }
        // Otan h mnhmh pou exoume desmeusei arxika den eparkei pleon, kaloume thn resizeEntries
        if (entryCount == entryCapacity) {
            resizeEntries(&entries, &entryCapacity);
        }

        entries[entryCount++] = entry;
    }

    fclose(file);


    int shouldContinue = 1;
while (shouldContinue) {
    // Dinoume ston xrhsth tis parakatw epiloges sxetika me to poion algori8mo 8a xrhsimopoihsei
    int searchAlgorithm;
    printf("Select a search algorithm:\n");
    printf("1. Binary Search\n");
    printf("2. Interpolation Search\n");
    printf("Enter your choice: ");
    scanf("%d", &searchAlgorithm);

    if (searchAlgorithm != 1 && searchAlgorithm != 2) {
        printf("Invalid search algorithm\n");
        continue;  // Restart the loop and prompt the user again
    }

    // Zhtame apo ton xrhsth na dwsei mia hmeromhnia ths morfhs DD/MM/YYYY
    char searchDate[11];
    printf("Enter a date (DD/MM/YYYY): ");
    scanf("%s", searchDate);

    printf("Search Date: %s\n",searchDate);
    printf("Please wait while the file is being sorted(~45 seconds)");
        // Taksinomhsh twn stoixeiwn se auksousa seira
        quicksort(entries, 0, entryCount - 1);

        printf("Sorted\n");  // Debugging statement

    int index = -1;
    int count = 0;

    switch (searchAlgorithm) {
        case 1:
            index = binarySearch(entries, 0, entryCount - 1, searchDate);
            break;
        case 2:
            index = interpolationSearch(entries, 0, entryCount - 1, searchDate);
            break;
    }

    if (index != -1) {
            // Zhtame apo ton xrhsth na epilexei poia plhroforia 8a tupw8ei
        int printOption;
        printf("Select information to print:\n");
        printf("1. Value\n");
        printf("2. Cumulative\n");
        printf("3. Both\n");
        printf("Enter your choice: ");
        scanf("%d", &printOption);

        // Metrame tis epanalhpseis ths hmeromhnias
        for (int i = index ; i >= 0 && compareDates(entries[i].Date, searchDate) == 0 ; i--){
                count++;
                switch (printOption) {
                    case 1:
                        printf("Value: $%u\n", entries[i].Value);
                        break;
                    case 2:
                        printf("Cumulative: $%u\n", entries[i].Cumulative);
                        break;
                    case 3:
                        printf("Value: $%u\n", entries[i].Value);
                        printf("Cumulative: $%u\n", entries[i].Cumulative);
                        break;
                    default:
                        printf("Invalid print option\n");
                        break;
                }
            }
        for(int i = index +1; i < entryCount && compareDates(entries[i].Date, searchDate) == 0 ; i++){
            count++;
            switch (printOption) {
                    case 1:
                        printf("Value: $%u\n", entries[i].Value);
                        break;
                    case 2:
                        printf("Cumulative: $%u\n", entries[i].Cumulative);
                        break;
                    case 3:
                        printf("Value: $%u\n", entries[i].Value);
                        printf("Cumulative: $%u\n", entries[i].Cumulative);
                        break;
                    default:
                        printf("Invalid print option\n");
                        break;
                }
        }

        printf("Entries found for date %s: %d\n", searchDate, count);

    } else {
        printf("No entries found for date %s\n", searchDate);
    }

    // Dinoume thn epilogh na trexei pali to programm h na kleisei
    printf("Do you want to continue searching? (1 for Yes, 0 for No): ");
    scanf("%d", &shouldContinue);
    printf("\n----------\n");
}

    // Apodesmeuoume th mnhmh
    free(entries);

    return 0;
}
