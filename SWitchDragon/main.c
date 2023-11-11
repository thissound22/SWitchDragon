#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define WIDTH 65
#define HEIGHT 35

typedef struct _Moogi {
    COORD pos;
    char direction;
    struct _Moogi *left;
    struct _Moogi *right;
} Moogi, *pMoogi;

typedef struct _Item {
    COORD pos;
    int itemNo;
} Item, *pItem;

/* 전역변수(기존) */
int direction;
int stage;
int stageBestScore[5];
int speed;
int shield;
int fever[5];
int headPosX;
int headPosY; // COORD headPos(의견)
int tailPosX;
int tailPosY; // COORD tailPos(의견)

/* (추가) */
pMoogi moogiHead;
pMoogi moogiTail;
pMoogi addBody;
pItem item;
time_t itemCreationTime;

int length;
int minLength = 1;
int maxLength = 40;
int minSpeed = 10;
int maxSpeed = 100;

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

void InitMoogi() {
    moogiHead = (Moogi*)malloc(sizeof(Moogi));
    moogiTail = (Moogi*)malloc(sizeof(Moogi));
    addBody = (Moogi*)malloc(sizeof(Moogi));
    item = (Item*)malloc(sizeof(Item));
    
    moogiHead->right = NULL;
    moogiHead->left = addBody;
    addBody->right = moogiHead;
    addBody->left = moogiTail;
    
    addBody->pos.X = WIDTH / 2;
    addBody->pos.Y = HEIGHT / 2;
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
    item->pos.X = rand() % WIDTH;
    item->pos.Y = rand() % HEIGHT;
    
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
void DeleteItem() {
    if (detectCollision()) { // 수정
        item->itemNo = -1;
        
    }
    else {
        time_t currentTime;
        time(&currentTime);

        if (difftime(currentTime, itemCreationTime) >= 7) {
            DeleteItem();
        }

    }
}

