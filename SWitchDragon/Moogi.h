#ifndef __MOOGI_H__
#define __MOOGI_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>

#include "Constants.h"


typedef struct Moogi
{
    struct Moogi* left;
    struct Moogi* right;
    COORD position;
    char direct
}Moogi, *pMoogi;

#endif
