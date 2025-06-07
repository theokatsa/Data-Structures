#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataentry.h"

#define MAX_SIZE 11 // Size of the hash table

typedef struct Node {
    DataEntry record;
    struct Node* next;
} Node;

Node* hashTable[MAX_SIZE];

// Hash function
int hashFunction(const char* date, int m) {
    int asciiSum = 0;
    for (int i = 0; i < strlen(date); i++) {
        asciiSum += (int)date[i];
    }
    return asciiSum % m;
}

// Insert a record into the hash table
void insertRecord(const char* date, int value, int m) {
    int key = hashFunction(date, m);

    // Create a new node
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->record.Date, date);
    newNode->record.Value = value;
    newNode->next = NULL;

    // Insert the node at the beginning of the corresponding bucket
    if (hashTable[key] == NULL) {
        hashTable[key] = newNode;
    } else {
        newNode->next = hashTable[key];
        hashTable[key] = newNode;
    }
}

// Search for a record based on the date
void searchRecord(const char* date, int m) {
    int key = hashFunction(date, m);

    // Traverse the bucket to find the record
    Node* currentNode = hashTable[key];
    while (currentNode != NULL) {
        if (strcmp(currentNode->record.Date, date) == 0) {
            printf("Value for Date %s: %d\n", date, currentNode->record.Value);
            return;
        }
        currentNode = currentNode->next;
    }

    printf("Record not found for Date %s\n", date);
}

// Modify the value of a record based on the date
void modifyRecord(const char* date, int value, int m) {
    int key = hashFunction(date, m);

    // Traverse the bucket to find the record
    Node* currentNode = hashTable[key];
    while (currentNode != NULL) {
        if (strcmp(currentNode->record.Date, date) == 0) {
            currentNode->record.Value = value;
            printf("Value modified for Date %s\n", date);
            return;
        }
        currentNode = currentNode->next;
    }

    printf("Record not found for Date %s\n", date);
}

// Delete a record based on the date
void deleteRecord(const char* date, int m) {
    int key = hashFunction(date, m);

    // Check if the first node is the record to be deleted
    if (hashTable[key] != NULL && strcmp(hashTable[key]->record.Date, date) == 0) {
        Node* temp = hashTable[key];
        hashTable[key] = hashTable[key]->next;
        free(temp);
        printf("Record deleted for Date %s\n", date);
        return;
    }

    // Traverse the bucket to find the record to be deleted
    Node* currentNode = hashTable[key];
    while (currentNode != NULL && currentNode->next != NULL) {
        if (strcmp(currentNode->next->record.Date, date) == 0) {
            Node* temp = currentNode->next;
            currentNode->next = currentNode->next->next;
            free(temp);
            printf("Record deleted for Date %s\n", date);
            return;
        }
        currentNode = currentNode->next;
    }

    printf("Record not found for Date %s\n", date);
}

int main() {
    FILE* file = fopen("effects.csv", "r");
    if (file == NULL) {
        printf("Error opening effects.csv\n");
        return 1;
    }

    char headers[256];
    if (fgets(headers, sizeof(headers), file) == NULL) {
        printf("Error reading headers\n");
        fclose(file);
        return 1;
    }

    char line[256];
    Node* root = NULL;

    while (fgets(line, sizeof(line), file)) {
        DataEntry entry;
        int result;
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

            return 1;
        }


    }

    fclose(file);
    int choice;
    char date[11];
    int value;

    // Initialize hash table buckets to NULL
    for (int i = 0; i < MAX_SIZE; i++) {
        hashTable[i] = NULL;
    }

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Search for a record\n");
        printf("2. Modify a record\n");
        printf("3. Delete a record\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the date: ");
                scanf("%s", date);
                searchRecord(date, MAX_SIZE);
                break;
            case 2:
                printf("Enter the date: ");
                scanf("%s", date);
                printf("Enter the new value: ");
                scanf("%d", &value);
                modifyRecord(date, value, MAX_SIZE);
                break;
            case 3:
                printf("Enter the date: ");
                scanf("%s", date);
                deleteRecord(date, MAX_SIZE);
                break;
            case 4:
                printf("Exiting the application...\n");
                exit(0);
            default:
                printf("Invalid choice\n");
                break;
        }
    }

    return 0;
}
