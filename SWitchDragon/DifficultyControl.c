#pragma warning(disable: 4996)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>

#include "DifficultyControl.h"

void InitMoogi() {
    moogiHead = (Moogi*)malloc(sizeof(Moogi));
    moogiTail = (Moogi*)malloc(sizeof(Moogi));
    addBody = (Moogi*)malloc(sizeof(Moogi));
    item = (Item*)malloc(sizeof(Item));
    
    moogiHead->right = NULL;
    moogiHead->left = addBody;
    addBody->right = moogiHead;
    addBody->left = moogiTail;
    
    addBody->pos.X = GBOARD_WIDTH / 2;
    addBody->pos.Y = GBOARD_HEIGHT / 2;
    addBody->direction = RIGHT;
    moogiTail->right = addBody;
    moogiTail->left = NULL;

    item->itemNo = -1;
    
    AddBody();
}
void AddBody() {
    // 화면 공간 예외 처리
 
    pMoogi newBody = (Moogi*)malloc(sizeof(Moogi));
    pMoogi tmp = moogiTail->right;

    newBody->left = moogiTail;
    newBody->right = moogiTail->right;
    moogiTail->right = newBody;
    newBody->right->left = newBody;
    newBody->direction = tmp->direction;

    newBody->pos.X = tmp->pos.X;
    newBody->pos.Y = tmp->pos.Y;

    length++;

    switch (tmp->direction) {
    case LEFT:
        newBody->pos.X++; break;
    case RIGHT:
        newBody->pos.X--; break;
    case UP:
        newBody->pos.Y++; break;
    case DOWN:
        newBody->pos.Y--; break;
    default:
        break;
    }
}

void DeleteBody() {
    if (length > minLength) { // 최소길이보다 클 때만 실행
        pMoogi tmp = moogiTail->left;
        tmp->left->right = moogiTail;
        moogiTail->left = tmp->left;
        free(tmp);
        length--;
    }
}
void SpeedUp() {
    if (speed < maxSpeed) // 최대 속도 리밋
        speed += 10;
}
void SpeedDown() {
    if (speed > minSpeed) // 최소 속도 리밋
        speed -= 10;
}
void CreateItem() {
    item = (Item*)sizeof(Item);

    time(&itemCreationTime);

    item->itemNo = rand() % 2; // 아이템 종류 2가지
    item->pos.X = rand() % GBOARD_WIDTH;
    item->pos.Y = rand() % GBOARD_HEIGHT;
    
    SetCurrentCursorPos(item->pos.X, item->pos.Y); // 몸 제외 위치
    
    switch (item->itemNo) {
        case 0:
            printf("▲"); break;
        case 1:
            printf("▼"); break;
        default:
            break;
    }
}
void DeleteItem() { // 삭제만 처리 or 과정까지 처리
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

