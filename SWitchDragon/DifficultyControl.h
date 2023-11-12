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


void SetCurrentCursorPos(int, int);
int detectCollision(); // 임시로 추가

/* 난이도 제어 함수 */
void InitMoogi();
// void Switch(int, COORD, COORD);
void AddBody();
void DeleteBody();
void SpeedUp();
void SpeedDown();
void CreateItem();
void DeleteItem();
// void Pet();
// void ShowPet();
// void DeletePet();
// void RandomCoord();


#endif /* DifficultyControl_h */
