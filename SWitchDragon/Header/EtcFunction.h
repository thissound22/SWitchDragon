#ifndef __ETCFUNCTION_H__
#define __ETCFUNCTION_H__

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

int countScore(int currentScore);
void getBestScore();
void setBestScore(int currentScore);
void saveBestScore();
int isGameOver();
void getItem();
void addDragonBall();
void showDragonBall(int arrX, int arrY);

#endif
