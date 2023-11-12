#ifndef __USERCONTROL_H__
#define __USERCONTROL_H__

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

int inPlayKeyInput();
void shiftUp();
void shiftDown();
void shiftLeft();
void shiftRight();
int pausePlay();
void goToStartScreen();
int detectCollision(int currentPosX, int currentPosY);
COORD nextHeadPos();
void drawHead(COORD headPos);
void eraseTail();
Moogi* getNode(Moogi* left, Moogi* right, COORD position);

#endif
