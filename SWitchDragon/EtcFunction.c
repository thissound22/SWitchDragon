#pragma warning(disable: 4996)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>

#include "EtcFunction.h"

int countScore(int currentScore) {
    if (detectCollision(head->position.X, head->position.Y) == 3) {
        currentScore += 5;
    }

    return currentScore;
}

void getBestScore() {    //게임 시작 시 플레이 전에 반드시 불러야 한다...(1번만)
    FILE* fp_r = fopen("BestScore.txt", "r");
    if (fp_r == NULL) {
        //printf("없음\n");
        return;
    }
    //printf("있음!!\n");
    for (int i = 0; i < 5; i++) {
        fscanf(fp_r, "%d", &stageBestScore[i]);
    }
    fclose(fp_r);        //파일에서 최고점수 얻어옴 → 파일 비우고 stageBestScore배열에 저장
}

void setBestScore(int currentScore) {            //게임오버됐을때만 부르면 될 듯?
    if (currentScore > stageBestScore[stage - 1]) {
        stageBestScore[stage - 1] = currentScore;
    }
}

void saveBestScore() {    //게임 종료 시 다시 최고점수를 파일에 저장하는 역할
    FILE* fp_w = fopen("BestScore.txt", "w");
    for (int i = 0; i < 5; i++) {
        fprintf(fp_w, "%d\n", stageBestScore[i]);
    }
    fclose(fp_w);
}

int isGameOver() {        //리턴값이 1이면 게임종료해야됨
    if (detectCollision(head->position.X, head->position.Y) == 1) {
        return 1;        //박으면 1리턴
    }
    return 0;            //아니면 0
}

void getItem() {
    /*메인에서 이거 불러야됨
    srand((unsigned int)time(NULL));*/
    int goodOrBad = detectCollision(head->position.X, head->position.Y);
    if (goodOrBad == 4) {    //좋은아이템(길이↓, 속도↓, FEVER, 실드, 여의주생성)
        int item = (rand() % 2) + 1;//(rand() % 5) + 1; 원래 이건데 지금은 아이템 2개만
        switch (item) {
        case 1:        //길이↓
            deleteBody();
            gotoxy(INFO_X, ITEM_Y, "길이가 줄어듭니다!");
            break;
        case 2:        //속도↓
            speedDown();
            gotoxy(INFO_X, ITEM_Y, "속도가 감소합니다!");
            break;
        }
    }
    else if (goodOrBad == 5) {    //안좋은아이템 (길이↑, 속도↑)
        int item = (rand() % 2) + 1;
        switch (item) {
        case 1:        //길이↑
            addBody();
            gotoxy(INFO_X, ITEM_Y, "길이가 늘어납니다!");

            break;
        case 2:        //속도↑
            speedUp();
            gotoxy(INFO_X, ITEM_Y, "속도가 증가합니다!"); 

            break;
        }
    }

    //0 아무것도없는거, 1 벽장애물, 2 이무기몸, 3 여의주, 4 좋은아이템, 5 나쁜아이템,
}

void addDragonBall() {
    while (1) {
        int arrX = (rand() % (GBOARD_WIDTH - GBOARD_ORIGIN_X)) / 2;
        int arrY = rand() % (GBOARD_HEIGHT - GBOARD_ORIGIN_Y);
        if (gameBoardInfo[arrX][arrY] == 0) {
            gameBoardInfo[arrX][arrY] = 3;
            showDragonBall(arrX, arrY);
            return;
        }
    }
}

void showDragonBall(int arrX, int arrY) {
    setCurrentCursorPos(arrX, arrY);
    printf("⊙");
}
