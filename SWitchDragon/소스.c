#include<stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#pragma warning(disable:4996)

/* Constants.h */
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32
#define ESC 27
#define GBOARD_WIDTH 35
#define GBOARD_HEIGHT 25
#define GBOARD_ORIGIN_X 14
#define GBOARD_ORIGIN_Y 3
#define START_POINT_X 10
#define START_POINT_Y 10
#define GOOD 111
#define BAD -111

typedef struct Moogi {
    int x;
    int y;
    struct Moogi* next;
} Moogi;

/* Globals.h(부분) */
int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2] = { {0} };
int speed;
int item_x, item_y;
COORD itemPos = { 0, 0 };

int x[100], y[100];
int key;
int length;
int dir;
int score;
int speed;
int maxSpeed = 1000;
int minSpeed = 100;

void title();
void gotoxy(int, int, char*);
void init();
void item();
void gameOver();
void move(int);

/* Cursor.h */
COORD getCurrentCursorPos();
void removeCursor();

/* UserControl.h */
int detectCollision(int, int);

/* DifficultyControl.h */
void speedUp();
void speedDown();

/* 추가 */
Moogi* head;
void gotoxy(int, int, char*);
void addBody(int, int);
void deleteTail();
void clearLinkedList();

/******** main ********/
int main() {
    title();
    while (1) {
        if (_kbhit())
            do { key = _getch(); } while (key == 224);
        Sleep(speed);

        switch (key) {
        case LEFT:
        case RIGHT:
        case UP:
        case DOWN:
            if ((dir == LEFT && key != RIGHT) || (dir == RIGHT && key != LEFT) || (dir == UP && key != DOWN) || (dir == DOWN && key != UP)) // 이동방향 제한
                dir = key;
            key = 0;
            break;
        case ESC:
            exit(0);
        }
        move(dir);
    }
    return 0;
}

/* drawMap */
void console() { // set console size
    SMALL_RECT windowSize = { 0 , 0 , 100 , 40 };
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
}
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
    console();
    initGameBoardInfo();
    for (int y = 0; y <= GBOARD_HEIGHT; y++) {
        if (y == 0)
            gotoxy(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y, "?");
        else if (y == GBOARD_HEIGHT)
            gotoxy(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y, "└");
        else
            gotoxy(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y, "│");

        if (y == 0)
            gotoxy(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y, "?");
        else if (y == GBOARD_HEIGHT)
            gotoxy(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y, "┘");
        else
            gotoxy(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y, "│");


    }
    for (int x = 1; x < GBOARD_WIDTH + 1; x++) {
        gotoxy(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y, "─");
        gotoxy(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT, "─");
    }
}

/* item */
void item() {
    int r = 0; // 랜덤 상수(값 변하게 함)
    int item_crush = 0; // 충돌 flag
    int itemNo = 0; // 아이템 종류

    while (1) {
        srand((unsigned)time(NULL) + r);
        item_x = (rand() % GBOARD_WIDTH - 2) + GBOARD_ORIGIN_X + 2;
        item_y = (rand() % GBOARD_HEIGHT - 2) + GBOARD_ORIGIN_Y + 2;
        itemNo = (rand() % 2);

        for (int i = 0; i < length; i++) { // detectCollision
            if (item_x == head->x + i && item_y == head->y + i) {
                item_crush = 1;
                r++;
                break;
            }
        }

        if (item_crush == 1)
            continue;

        switch (itemNo) {
        case 0:
            gotoxy(item_x, item_y, "▲");
            gameBoardInfo[item_x][item_y] = GOOD;
            break;
        case 1:
            gotoxy(item_x, item_y, "▼");
            gameBoardInfo[item_x][item_y] = BAD;
            break;
        defualt:
            break;
        }
        break;
    }
}
void move(int dir) {
    int newBody_X, newBody_Y;

    if (head->x == item_x && head->y == item_y) { // detectCollision(아이템)
        if (gameBoardInfo[item_x][item_y] == GOOD) {
            speedDown();
            gameBoardInfo[item_x][item_y] = 0;
        }
        else if (gameBoardInfo[item_x][item_y] == BAD) {
            speedUp();
            gameBoardInfo[item_x][item_y] = 0;
        }
        score += 10;
        item();
        addBody(head->x, head->y);
        length++;
    }
    deleteTail();
    switch (dir) { // InPlayKeyInput
    case LEFT:
        newBody_X = head->x - 2;
        newBody_Y = head->y;
        break;
    case RIGHT:
        newBody_X = head->x + 2;
        newBody_Y = head->y;
        break;
    case UP:
        newBody_X = head->x;
        newBody_Y = head->y - 1;
        break;
    case DOWN:
        newBody_X = head->x;
        newBody_Y = head->y + 1;
        break;
    }
    addBody(newBody_X, newBody_Y);

    gotoxy(newBody_X, newBody_Y, "●");
    gotoxy(head->x, head->y, "●"); // ◐ -> 방향에 따라 머리 모양이 바뀌어야 함.. 너무 귀찮

    if (newBody_X == GBOARD_ORIGIN_X || newBody_X == GBOARD_ORIGIN_X + GBOARD_WIDTH - 2 || newBody_Y == GBOARD_ORIGIN_Y || newBody_Y == GBOARD_ORIGIN_Y + GBOARD_HEIGHT - 1) { // detectCollision(벽)
        gameOver();
        return;
    }
};

/****** 함수 정의 ******/

/* Cursor.c*/
COORD getCurrentCursorPos() { // get current cursor position
    COORD curPos;
    CONSOLE_SCREEN_BUFFER_INFO curInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curPos.X = curInfo.dwCursorPosition.X;
    curPos.Y = curInfo.dwCursorPosition.Y;

    return curPos;
}
void removeCursor() { // remove cursor blinking
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

/* Moogi */
void addBody(int x, int y) {
    Moogi* newNode = (Moogi*)malloc(sizeof(Moogi));
    newNode->x = x;
    newNode->y = y;
    newNode->next = head;
    head = newNode;
}
void deleteTail() {
    if (length <= 1)
        return;

    Moogi* current = head;
    Moogi* prev = NULL;

    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    gotoxy(current->x, current->y, "  ");
    free(current);

    prev->next = NULL;
}
void clearLinkedList() {
    while (head != NULL) {
        Moogi* temp = head;
        head = head->next;
        free(temp);
    }
}

/* Basic */
void gotoxy(int x, int y, char* s) {
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("%s", s);
}

/* UserControl.h */
int detectCollision(int posX, int posY) // 충돌 감지
// 이 함수의 입력 인자로 이무기 머리 위치를 넣어주면 됨
// 충돌 감지하고 싶으면 direction에 따라 한칸 움직인 위치
// 획득 감지하고 싶으면 그냥 현재 위치
// isGameOver는 어차피 그 이후로 이무기 다시 안 그리니까 그냥 현재 위치 넣으면 됨!
{
    int arrX = (posX - GBOARD_ORIGIN_X) / 2;
    int arrY = (posY - GBOARD_ORIGIN_Y);

    switch (gameBoardInfo[arrY][arrX])
    {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    case 4:
        return 4;
    case 5:
        return 5;
    case 6:
        return 6;
    default:
        return 0;
    }
}
/* DifficultyControl.c */
void speedUp() {
    if (speed < maxSpeed) // 최대 속도 리밋
        speed += 100;
}
void speedDown() {
    if (speed > minSpeed) // 최소 속도 리밋
        speed -= 100;
}

/* ETC */
void title() {
    removeCursor();
    gotoxy(34, 12, "");
    printf("Press 'SPACE' to start the game");

    while (1) {
        if (_kbhit())
            key = _getch();
        if (key == 32) {
            system("cls");
            break;
        }
    }
    init();
}
void init() {
    system("cls");
    drawBoard();
    // removeCursor();
    while (_kbhit())
        getch();

    dir = DOWN;
    speed = 300;
    length = 5;
    score = 0;

    head = NULL;

    for (int i = 0; i < length; i++) {
        addBody(GBOARD_ORIGIN_X + GBOARD_WIDTH / 2 + i, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2);
        gotoxy(head->x, head->y, "●");
    }
    gotoxy(head->x, head->y, "●"); // ◐
    item();
}
void gameOver() {
    clearLinkedList();
    system("cls");
    gotoxy(20, 20, "");
    printf("Your score is %d", score);

    return;
}

