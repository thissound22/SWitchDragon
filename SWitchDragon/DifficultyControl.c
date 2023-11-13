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
