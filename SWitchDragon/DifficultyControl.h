#ifndef DifficultyControl_h
#define DifficultyControl_h

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

#include "Constans.h"
#include "Moogi.h"
#include "Item.h"
#include "Globals.h"


void setCurrentCursorPos(int, int);
int detectCollision(); // 임시로 추가

/* 난이도 제어 함수 */
void initMoogi();
// void Switch(int, COORD, COORD);
void addBody();
void deleteBody();
void speedUp();
void speedDown();
void createItem();
void deleteItem();
// void pet();
// void showPet();
// void deletePet();
// void randomCoord();


#endif /* DifficultyControl_h */
