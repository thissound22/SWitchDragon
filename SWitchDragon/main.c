#pragma warning(disable:4996)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>

#include "Constants.h"
#include "Moogi.h"
#include "Globals.h"
#include "Cursor.h"
#include "DrawMap.h"
#include "UserControl.h"
#include "EtcFunction.h"
#include "Item.h"

int main() {
    console();
    removeCursor();
    system("cls");
    drawBoard();
    getch();

    return 0;
}
