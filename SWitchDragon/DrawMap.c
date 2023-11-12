#pragma warning(disable: 4996)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>

#include "DrawMap.h"

void initGameBoardInfo() { // setting up boundaries
    for (int y = 0; y < GBOARD_HEIGHT; y++) {
        gameBoardInfo[y][0] = 1;
        gameBoardInfo[y][GBOARD_WIDTH + 1] = 1;
    }
    for (int x = 0; x < GBOARD_WIDTH + 2; x++) {
        gameBoardInfo[GBOARD_HEIGHT][x] = 1;
    }
}

void drawBoard() {
    initGameBoardInfo();
    for (int y = 0; y <= GBOARD_HEIGHT; y++) {
        setCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
        if (y == 0) printf("¶Æ");
        else if (y == GBOARD_HEIGHT) printf("¶±");
        else printf("¶≠");

        setCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y);
        if (y == 0) printf("¶Ø");
        else if (y == GBOARD_HEIGHT) printf("¶∞");
        else printf("¶≠");
    }
    for (int x = 1; x < GBOARD_WIDTH + 1; x++) {
        setCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y);
        printf("¶¨");

        setCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
        printf("¶¨");
    }
}
