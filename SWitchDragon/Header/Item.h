#ifndef Item_h
#define Item_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>

typedef struct _Item {
    COORD pos;
    int itemNo;
} Item, *pItem;

#endif /* Item_h */
