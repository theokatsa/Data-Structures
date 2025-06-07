#ifndef DATAENTRY_H
#define DATAENTRY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct DataEntry {
    char Direction[20];
    int Year;
    char Date[11];
    char Weekday[20];
    char Country[20];
    char Commodity[20];
    char Transport_mode[20];
    char Measure[3];
    int Value;
    unsigned int Cumulative;
} DataEntry;


#endif /* DATAENTRY_H */




