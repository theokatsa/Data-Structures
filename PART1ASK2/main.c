#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorting.h"
#include "dataentry.h"
// 8ewrh8hke kaluterh h ulopoihsh twn heapsort,quicksort,dataentry se ksexwrista .c kai .h files opote 8a prepei na uparxoun entos tou arxeiou ths askhshs
#define INITIAL_CAPACITY 1000 // Arxikh xwrhtikothta tou pinaka entries
#define CAPACITY_INCREMENT_PERCENT 50

// Xrhsimopoioume thn resizeEntries wste na desmeusoume thn aparaithth mnhmh gia tis kataxwrhseis tou pinaka
void resizeEntries(DataEntry **entries, int *capacity) {
    int newCapacity = (*capacity) + ((*capacity) * CAPACITY_INCREMENT_PERCENT) / 100;
    DataEntry *newEntries = realloc(*entries, newCapacity * sizeof(DataEntry));
    if (newEntries == NULL) {
        printf("Memory reallocation failed for entries array\n");
        // Elegxos sfalmatos
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

// Paromoiws me ta heapsort kai quicksort to effects.csv arxeio einai aparaithto na uparxei entos tou fakelou, egine allagh tou onomatos se effects.csv gia logous eukolias ston kwdika
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
        if (strchr(line, '\"') != NULL){
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


    int choice;
    // Dinoume ston xrhsth tis parakatw epiloges sxetika me to poion algori8mo 8a xrhsimopoihsei
    printf("\nSorting Options:\n");
    printf("1. Heap Sort\n");
    printf("2. Quick Sort\n");
    printf("3. Terminate\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        heapSort(entries, entryCount);
        printf("Heap Sort has been applied.\n");
        break;
    case 2:
        quickSort(entries, 0, entryCount - 1);
        printf("Quick Sort has been applied.\n");
        break;
    case 3:
        printf("Terminating the program.\n");
        free(entries);
        return 0;
    default:
        printf("Invalid choice. Please try again.\n");
        free(entries);
        return 1; // Return an error code if an invalid choice is entered
}
    // Logw mh ektupwshs olwn twn apotelesmatwn apo thn arxh sto command prompt, 8ewrh8hke katallhlotero na ektupw8oun se txt file gia kaluterh analush twn dedomenwn
    FILE *outputFile = fopen("sorted_entries.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening output file\n");
        free(entries);
        return 1;
    }

    fprintf(outputFile, "Sorted entries in ascending order: \n");
    for (int i = 0; i < entryCount; i++) {
        fprintf(outputFile, "%s; %d; %s; %s; %s; %s; %s; %s; $%d; %u\n",
                entries[i].Direction, entries[i].Year, entries[i].Date, entries[i].Weekday,
                entries[i].Country, entries[i].Commodity, entries[i].Transport_mode,
                entries[i].Measure, entries[i].Value, entries[i].Cumulative);
    }

    // Elegxos se periptwsh sfalmatos
    int error = 0;
    for (int i = 1; i < entryCount; i++) {
        if (entries[i].Cumulative < entries[i - 1].Cumulative) {
            fprintf(outputFile, "Error: Entries are not sorted correctly\n");
            error++;
        }
    }

    fprintf(outputFile, "Errors: %d\n", error);

    fclose(outputFile);
    printf("Sorted entries have been printed to the sorted_entries.txt file");
    free(entries);

    // Anoigma tou arxeiou pou tupwnetai mesw txt file editor
    #ifdef _WIN32
        system("start notepad sorted_entries.txt");
    #else
        printf("Please open the file sorted_entries.txt manually.");
    #endif

    return 0;
}
