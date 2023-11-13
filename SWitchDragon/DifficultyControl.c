#pragma warning(disable: 4996)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>

#include "DifficultyControl.h"

void initMoogi() {
    moogiHead = (Moogi*)malloc(sizeof(Moogi));
    moogiTail = (Moogi*)malloc(sizeof(Moogi));
    moogiBody = (Moogi*)malloc(sizeof(Moogi));
    item = (Item*)malloc(sizeof(Item));
    
    moogiHead->right = NULL;
    moogiHead->left = moogiBody;
    moogiBody->right = moogiHead;
    moogiBody->left = moogiTail;
    
    moogiBody->position.X = GBOARD_WIDTH / 2;
    moogiBody->position.Y = GBOARD_HEIGHT / 2;
    moogiBody->direct = RIGHT;
    moogiTail->right = moogiBody;
    moogiTail->left = NULL;

    item->itemNo = -1; // 아무 아이템도 출력되지 않음
}
void addBody() {
    // 화면 공간 예외 처리
 
    pMoogi newBody = (Moogi*)malloc(sizeof(Moogi));
    pMoogi tmp = moogiTail->right;

    newBody->left = moogiTail;
    newBody->right = moogiTail->right;
    moogiTail->right = newBody;
    newBody->right->left = newBody;
    newBody->direct = tmp->direct;

    newBody->position.X = tmp->position.X;
    newBody->position.Y = tmp->position.Y;

    length++;

    switch (tmp->direct) {
    case LEFT:
        newBody->position.X++; break;
    case RIGHT:
        newBody->position.X--; break;
    case UP:
        newBody->position.Y++; break;
    case DOWN:
        newBody->position.Y--; break;
    default:
        break;
    }
}

void deleteBody() {
    if (length > minLength) { // 최소길이보다 클 때만 실행
        pMoogi tmp = moogiTail->left;
        tmp->left->right = moogiTail;
        moogiTail->left = tmp->left;
        free(tmp);
        length--;
    }
}
void speedUp() {
    if (speed < maxSpeed) // 최대 속도 리밋
        speed += 10;
}
void speedDown() {
    if (speed > minSpeed) // 최소 속도 리밋
        speed -= 10;
}
void createItem() {
    item = (Item*)sizeof(Item);

    time(&itemCreationTime);

    item->itemNo = rand() % 2; // 아이템 종류 2가지
    item->pos.X = rand() % GBOARD_WIDTH;
    item->pos.Y = rand() % GBOARD_HEIGHT;
    
    setCurrentCursorPos(item->pos.X, item->pos.Y); // 몸 제외 위치
    
    switch (item->itemNo) {
        case 0:
            printf("▲"); break;
        case 1:
            printf("▼"); break;
        default:
            break;
    }
}
void deleteItem() { // 삭제만 처리 or 과정까지 처리
    if (detectCollision()) { // 수정
        item->itemNo = -1;
        printf(" ");
    }
    else {
        time_t currentTime;
        time(&currentTime);

        if (difftime(currentTime, itemCreationTime) >= 7) {
            item->itemNo = -1;
            printf(" ");
        }
    }
}
// 호출 : 구슬을 먹으면 실행
// 이미 벽 또는 아이템이 출력된 곳에 출력하면 X
void addWall() {
   int wallX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X;
   int wallY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;

   do {
      wallX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X;
      wallY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
   } while (detectCollision(wallX, wallY)); // 아무것도 없으면 0 반환됨

   gotoxy(wallX, wallY, "▨");
   gameBoardInfo[wallX][wallY] = 1; // 벽 1
}

// 여기서부터는 EtcFunction-getItem()과 겹침
// 아이템 종류1(좋은, 나쁜) 별로 색깔 다르게 출력하면 될 듯
void goodItem(int itemNo) {
    int itemNo2 = rand() % 2;
    switch (itemNo2) {
    case 0:
        speedDown();
        gotoxy(INFO_X, ITEM_Y, "속도 감소 아이템을 얻었습니다!"); // 위치 수정
        break;
    case 1:
        lengthDec();
        gotoxy(INFO_X, ITEM_Y, "길이 감소 아이템을 얻었습니다!"); // 위치 수정
        break;
    default:
        break;
    }
}
void badItem(int itemNo) {
    int itemNo2 = rand() % 2;
    switch (itemNo2) {
    case 0:
        speedUp();
        gotoxy(INFO_X, ITEM_Y, "속도 증가 아이템을 얻었습니다!"); // 위치 수정
        break;
    case 1:
        lengthInc();
        gotoxy(INFO_X, ITEM_Y, "속도 감소 아이템을 얻었습니다!"); // 위치 수정
        break;
    default:
        break;
    }
}
void lengthDec() {
    deleteBody();
}
void lengthInc() {
    addBody();
}
