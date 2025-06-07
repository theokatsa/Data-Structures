#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataentry.h"

#define MAX_SIZE 11 // Mege8os tou hash pinaka

// BST Node structure
typedef struct BSTNode {
    DataEntry record;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

// Domh tou hashNode
typedef struct HashNode {
    DataEntry record;
    struct HashNode* next;
} HashNode;

// Pinakas hash
HashNode* hashTable[MAX_SIZE];

int hashFunction(const char* date, int m) {
    int asciiSum = 0;
    for (int i = 0; i < strlen(date); i++) {
        asciiSum += (int)date[i];
    }
    return asciiSum % m;
}

// Eisagwgh Record ston pinaka hash
void insertRecord(const char* date, int value, int m) {
    int key = hashFunction(date, m);

    // Dhmiourgia neou node
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    strcpy(newNode->record.Date, date);
    newNode->record.Value = value;
    newNode->next = NULL;

    // Eisagwgh tou node sthn arxh
    if (hashTable[key] == NULL) {
        hashTable[key] = newNode;
    } else {
        newNode->next = hashTable[key];
        hashTable[key] = newNode;
    }
}

int searchRecordHash(const char* date, int m) {
    int key = hashFunction(date, m);

    // Anazhthsh gia to record
    HashNode* currentNode = hashTable[key];
    int foundRecords = 0;  // Variable to keep track of the number of found records

    while (currentNode != NULL) {
        if (strcmp(currentNode->record.Date, date) == 0) {
            printf("Value for Date %s: %d\n", date, currentNode->record.Value);
            foundRecords++;  // Increment the count of found records
        }
        currentNode = currentNode->next;
    }

    if (foundRecords == 0) {
        printf("No records found for Date %s\n", date);
    }

    return key;  // Return the index of the found record(s)
}

// Boh8htikh sunarthsh insertBST
BSTNode* insertBST(BSTNode* root, DataEntry entry, int compareByDate) {
    if (root == NULL) {
        BSTNode* newNode = (BSTNode*)malloc(sizeof(BSTNode));
        newNode->record = entry;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (compareByDate) {
        if (strcmp(entry.Date, root->record.Date) < 0) {
            root->left = insertBST(root->left, entry, compareByDate);
        } else {
            root->right = insertBST(root->right, entry, compareByDate);
        }
    } else {
        if (entry.Value < root->record.Value) {
            root->left = insertBST(root->left, entry, compareByDate);
        } else {
            root->right = insertBST(root->right, entry, compareByDate);
        }
    }

    return root;
}

// Anazhthsh record me bash thn hmeromhnia sto BST
void searchRecordBST(BSTNode* root, const char* date, int compareByDate) {
    if (root == NULL) {
        printf("Record not found for Date %s\n", date);
        return;
    }

    int compare;
    int found = 0;
    while (root != NULL) {
        compare = compareByDate ? strcmp(date, root->record.Date) : (strcmp(date, root->record.Date) == 0) ? 0 : ((date > root->record.Date) ? 1 : -1);
        if (compare == 0) {
            printf("Value for Date %s: %d\n", date, root->record.Value);
            found = 1;
            root = root->right; // Continue searching in the right subtree
        } else if (compare < 0) {
            root = root->left;
        } else {
            root = root->right;
        }
    }

    if (!found) {
        printf("Record not found for Date %s\n", date);
    }
}

// Function to perform inorder traversal and write to a file
void inorderTraversalToFile(BSTNode* root, FILE* outputFile) {
    if (root == NULL) {
        return;
    }

    // Traverse left subtree
    inorderTraversalToFile(root->left, outputFile);

    // Write data to file
    fprintf(outputFile, "Direction: %s, Year: %d, Date: %s, Weekday: %s, Country: %s, Commodity: %s, Transport_mode: %s, Measure: %s, Value: %d, Cumulative: %u\n",
            root->record.Direction, root->record.Year, root->record.Date, root->record.Weekday,
            root->record.Country, root->record.Commodity, root->record.Transport_mode,
            root->record.Measure, root->record.Value, root->record.Cumulative);

    // Traverse right subtree
    inorderTraversalToFile(root->right, outputFile);
    }


// Function to display the BST and write to a file
void displayBST(BSTNode* root) {
    if (root == NULL) {
        printf("BST is empty.\n");
        return;
    }

    // Open the output file
    FILE* outputFile = fopen("bst_display.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Perform inorder traversal and write to the file
    inorderTraversalToFile(root, outputFile);

    // Close the output file
    fclose(outputFile);

    printf("BST displayed successfully and written to bst_display.txt.\n");
}


// Function to modify the value of a specific date in the BST
void modifyValue(BSTNode* root, const char* date, int newValue) {
    if (root == NULL) {
        printf("Record not found for Date %s\n", date);
        return;
    }

    int compare = strcmp(date, root->record.Date);
    if (compare == 0) {
        root->record.Value = newValue;
        printf("Value for Date %s modified to %d\n", date, newValue);
    } else if (compare < 0) {
        modifyValue(root->left, date, newValue);
    } else {
        modifyValue(root->right, date, newValue);
    }
}

void modifyRecordValue(const char* date, int size) {
    // Search for the record with the given date
    int index = searchRecordHash(date, size);

    // If the record is found, modify the value
    if (index != -1) {
        printf("Enter the new value: ");
        int newValue;
        scanf("%d", &newValue);

        // Modify the value of the record
        hashTable[index]->record.Value = newValue;

        printf("Record value modified successfully.\n");
    } else {
        printf("No record found for the given date.\n");
    }
}


// Function to delete a specific date from the BST
BSTNode* deleteNode(BSTNode* root, const char* date) {
    if (root == NULL) {
        printf("Record not found for Date %s\n", date);
        return root;
    }

    int compare = strcmp(date, root->record.Date);
    if (compare < 0) {
        root->left = deleteNode(root->left, date);
    } else if (compare > 0) {
        root->right = deleteNode(root->right, date);
    } else {
        // Node to be deleted found
        if (root->left == NULL) {
            BSTNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            BSTNode* temp = root->left;
            free(root);
            return temp;
        }

        // Node to be deleted has two children
        BSTNode* temp = root->right;
        while (temp->left != NULL)
            temp = temp->left;

        root->record = temp->record;
        root->right = deleteNode(root->right, temp->record.Date);
    }

    return root;
}


// Function to find the minimum value in the BST
BSTNode* findMinValue(BSTNode* node) {
    if (node == NULL || node->left == NULL)
        return node;
    return findMinValue(node->left);
}

// Function to find the maximum value in the BST
BSTNode* findMaxValue(BSTNode* node) {
    if (node == NULL || node->right == NULL)
        return node;
    return findMaxValue(node->right);
}

void deleteRecordHash(const char* key) {
    int index = hashFunction(key, MAX_SIZE);
    HashNode* currentNode = hashTable[index];
    HashNode* prevNode = NULL;

    // Traverse the linked list at the given index
    while (currentNode != NULL) {
        if (strcmp(currentNode->record.Date, key) == 0) {
            // Found the record to delete
            if (prevNode == NULL) {
                // If the record is the first node in the linked list
                hashTable[index] = currentNode->next;
            } else {
                // If the record is not the first node
                prevNode->next = currentNode->next;
            }

            // Free the memory allocated for the record
            free(currentNode);
            printf("Record with date '%s' deleted successfully.\n", key);
            return;
        }

        prevNode = currentNode;
        currentNode = currentNode->next;
    }

    printf("Record with date '%s' not found.\n", key);
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
    BSTNode* bstRoot = NULL;
    int choice, compareByDate = 0;

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
            return 1;
        }

        bstRoot = insertBST(bstRoot, entry, 1);
        insertRecord(entry.Date, entry.Value, MAX_SIZE);
    }

    fclose(file);

    //Dhmiourgoume to arxiko menu gia ton xrhsth pou epilegei an 8elei na kanei anazhthsh h na kleisei to programma
        printf("Menu:\n");
        printf("1. Load data into Binary Search Tree (BST)\n");
        printf("2. Load data into Hash Table with chaining\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

    if (choice == 1) {
        //Dhmiourgoume to deytero menu gia ton xrhsth pou epilegei an 8elei na kanei thn anazhthsh twn dedomenwn ston BST me bash thn hmeromhnia h thn timh
        printf("BST Menu:\n");
        printf("1. Load data by date\n");
        printf("2. Load data by value\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

    if (choice == 1) {
        compareByDate = 1;

        printf("Sort by Date Menu:\n");
        printf("1. Display BST with inorder traversal.\n");
        printf("2. Search for Value based on Date given by the user.\n");
        printf("3. Modify the value field corresponding to a specific Date.\n");
        printf("4. Delete a record corresponding to a specific Date.\n");
        printf("5. Exit the application.\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        while (choice != 5) {
            switch (choice) {
                case 1:
                    displayBST(bstRoot);
                    // Open the file automatically
                    system("start notepad bst_display.txt");
                    break;
                case 2: {
                    char date[11];
                    printf("Enter the Date: ");
                    scanf("%s", date);
                    searchRecordBST(bstRoot, date, compareByDate);
                    break;
                }
                case 3: {
                    char date[11];
                    int newValue;
                    printf("Enter the Date: ");
                    scanf("%s", date);
                    printf("Enter the new Value: ");
                    scanf("%d", &newValue);
                    modifyValue(bstRoot, date, newValue);
                    break;
                }
                case 4: {
                    char date[11];
                    printf("Enter the Date: ");
                    scanf("%s", date);
                    bstRoot = deleteNode(bstRoot, date);
                    break;
                }
                default:
                    printf("Invalid choice. Please try again.\n");
            }

            printf("Enter your choice: ");
            scanf("%d", &choice);
        }
    } else if (choice == 2) {
    compareByDate = 0;

    do{
        printf("Value Search Menu:\n");
        printf("1. Find the day(s) with the maximum value.\n");
        printf("2. Find the day(s) with the minimum value.\n");
        printf("3. Exit program.\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
    BSTNode* minNode = findMinValue(bstRoot);
    if (minNode != NULL) {
        printf("Maximum value in BST: %d\n", minNode->record.Value);
        printf("Date of the maximum value: %s\n", minNode->record.Date);
    } else {
        printf("The BST is empty.\n");
    }
} else if (choice == 2) {
    BSTNode* maxNode = findMaxValue(bstRoot);
    if (maxNode != NULL) {
        printf("Minimum value in BST: %d\n", maxNode->record.Value);
        printf("Date of the minimum value: %s\n", maxNode->record.Date);
    } else {
        printf("The BST is empty.\n");
    }
    }else if (choice == 3)
        break;
 else {
    printf("Invalid choice. Exiting the application.\n");
    return 0;
}
    }while (choice !=3);



        // Sort BST by value
}     else {
        printf("Invalid choice. Exiting the application.\n");
    }

     }else if (choice == 2) {
         do{
        printf("Enter your choice:\n");
        printf("1. Search for Value based on the Date provided by the user.\n");
        printf("2. Modify record entries based on the Date provided by the user. Modification only affects the Value field.\n");
        printf("3. Delete a record from the hash table based on the Date provided by the user.\n");
        printf("4. Exit the application.\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
    case 1:
        {
            char date[11];
            printf("Enter date to search (DD/MM/YYYY): ");
            scanf("%s", date);
            searchRecordHash(date, MAX_SIZE);
            break;
        }
    case 2:
        {
            char date[11];
            printf("Enter date to modify (DD/MM/YYYY): ");
            scanf("%s", date);
            modifyRecordValue(date, MAX_SIZE);
            break;
        }
    case 3:
        {
            char date[11];
            printf("Enter date to delete (DD/MM/YYYY): ");
            scanf("%s", date);
            deleteRecordHash(date);
            break;
        }
    case 4:
        printf("Exiting the application. Goodbye!\n");
        break;
    default:
        printf("Invalid choice. Please try again.\n");
        break;

}
    printf("--------------\n");
    }while (choice != 4);


    } else {
        printf("Invalid choice\n");
    }

    return 0;
}
