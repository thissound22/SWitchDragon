#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32
#define ENTER 13
#define ESC 27

// size of gameboard
#define GBOARD_WIDTH 26
#define GBOARD_HEIGHT 18

// starting point of gameboard
#define GBOARD_ORIGIN_X 14
#define GBOARD_ORIGIN_Y 5

// size of ultimate board
#define UBOARD_WIDTH 7
#define UBOARD_HEIGHT 2

// starting point of ultimate board
#define UBOARD_ORIGIN_X 13
#define UBOARD_ORIGIN_Y 24

// size of fever board
#define FBOARD_WIDTH 7
#define FBOARD_HEIGHT 2

// starting point of fever board
#define FBOARD_ORIGIN_X 53
#define FBOARD_ORIGIN_Y 24

// size of controls board
#define CBOARD_WIDTH 8
#define CBOARD_HEIGHT 9

// starting point of fever board
#define CBOARD_ORIGIN_X 71
#define CBOARD_ORIGIN_Y 13

#define INFO_X 77

#define STAGE_Y 5
#define HIGHSCORE_Y 7
#define SCORE_Y 9
#define PET_Y 11
#define FEVER_Y 13
#define ITEM_Y 23
#define PAUSE_Y 17
#define OVER_Y 19

typedef struct Moogi {
    struct Moogi* front;
    struct Moogi* back;
    COORD position;
    char direct;
} Moogi;

typedef struct _Item {
    COORD pos;
    int itemNo;
} Item, * pItem;

typedef struct Wall {
    int direct;
    COORD position;
    struct Wall* next;
} Wall;

typedef struct _Pet {
    COORD pos;
} Pet;

int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];
int direction = 3;
int stage = 0;
int stageBestScore[5] = { 0,0,0,0,0 };
int speed = 15;
int heart = 3;
int fever[5] = { 0,0,0,0,0 };
int dragonBallCount = 0;
int itemCount = 0;

Moogi* head = NULL;
Moogi* body = NULL;
Moogi* tail = NULL;

pItem item = NULL;
time_t itemCreationTime;

int length = 3;
int minLength = 2;
int maxLength = 40;
int minSpeed = 1;
int maxSpeed = 25;

int currentScore = 0;

Wall* wallHead;
int moogiColor = 4;   //5가지 색상만 사용. 0→4(dark red), 1→6(dark yellow), 2→2(dark green), 3→9(blue), 4→15(white)
int dragonBallColor[5];     //0빨 1노 2초 3파 4흰 고정, 어차피 위치랜덤돌릴거니까 얘 순서는 상관ㄴㄴ
COORD dragonBallPos[5];     //빨노초파흰 순서대로 위치 저장. 알맞은 색깔의 여의주를 먹으면 나머지들은 없애줘야하기때문..하..

int petGauge = 0;
Pet* pet = NULL;
time_t petCreationTime;

// Map related functions;

void stage1();
void stage2();
void stage3();

void gotoxycol(int x, int y, int col, char* s);
void introScreen();
void selectStage();
void drawBoard();
void drawUlt();
void drawFever();
void printStage();
void printScore();
void printHighscore();
void drawLife();
void drawControls();
void drawKeys();
void gameOver();

void console();
void setCurrentCursorPos(int x, int y);
COORD getCurrentCursorPos();
void removeCursor();
void gotoxy(int x, int y, char* ch);
void setTextColor(int colorNum);

void initGameBoardInfo();
void drawBoard();

void initMoogi();
void addBody();
void deleteBody();
void speedUp();
void speedDown();
void createItem();
void deleteItem();
void getHeart();
void dragonBallBomb();
void getPet();
void usePet();
void petMove();
void getItemOfPet();
void moogiSwitch();
void waitToRecover();

void inPlayKeyInput();
void shiftUp();
void shiftDown();
void shiftLeft();
void shiftRight();
void pausePlay();
int detectCollision(int currentPosX, int currentPosY);
COORD nextHeadPos();
void drawHead(COORD headPos);
void eraseTail();
Moogi* getNode(Moogi* front, Moogi* back, COORD position);
int moogiMove();
void getSomething();
void wallMove();

void countScore();
void getBestScore();
void setBestScore(int score);
void saveBestScore();
int isGameOver();
void getItem();
void addDragonBall();
void showDragonBall(int arrX, int arrY);
void addWall();

void setTextColor_rygbw(int colorNum);
void changeMoogiColor();
void changeMoogiBodyColor();
void setDragonBallColor();
void setDragonBallPos();
void showColorDragonBall(int x, int y, int color);
void deleteDragonBall();

void printMatrix();

int main()
{
    console();
    removeCursor();
    introScreen();

    switch (stage)
    {
    case 1:
        stage1();
        break;
    case 2:
        stage2();
        break;
    case 3:
        stage3();
        break;
    default:
        break;
    }

    getch();

    return 0;
}

void stage1()
{
    srand((unsigned int)time(NULL));

    getBestScore();
    drawBoard();
    initMoogi();

    addDragonBall();
    createItem();

    while (1)
    {
        if (isGameOver()) break;
        while (1)
        {

            if (moogiMove() == 0)
            {
                break;
            }
            inPlayKeyInput();

        }
    }

    setBestScore(currentScore);
    saveBestScore();
    gameOver();
}

void stage2()
{
    srand((unsigned int)time(NULL));

    getBestScore();
    drawBoard();
    initMoogi();

    addDragonBall();
    createItem();

    wallHead = (Wall*)malloc(sizeof(Wall));
    wallHead->next = NULL;

    while (1)
    {
        if (isGameOver()) break;

        while (1)
        {

            if (moogiMove() == 0)
            {
                break;
            }
            inPlayKeyInput();

        }
    }

    setBestScore(currentScore);
    saveBestScore();

    setTextColor(4);
    gameOver();
}

void stage3()
{
    srand((unsigned int)time(NULL));
    setDragonBallColor();
    setDragonBallPos();

    getBestScore();
    drawBoard();
    initMoogi();

    createItem();

    addDragonBall();

    while (1)
    {
        if (isGameOver()) break;

        while (1)
        {

            if (moogiMove() == 0)
            {
                break;
            }
            inPlayKeyInput();
        }
    }

    setBestScore(currentScore);
    saveBestScore();
    gameOver();
}

void printMatrix()
{
    for (int i = 0; i < GBOARD_HEIGHT + 1; i++)
    {
        setCurrentCursorPos(INFO_X, PAUSE_Y + 2 + i);
        for (int j = 0; j < GBOARD_WIDTH + 2; j++)
        {
            if (!gameBoardInfo[i][j]) printf("  ");
            else printf("%d ", gameBoardInfo[i][j]);
        }
    }
}

/*-----------------------------------------------------------------------------------------*/

void console() { // set console size
    system("mode con:cols=120 lines=33");
}

void setCurrentCursorPos(int x, int y) { // set current cursor position
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

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

void gotoxy(int x, int y, char* s) {
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("%s", s);
}

void setTextColor(int colorNum) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

void gotoxycol(int x, int y, int col, char* s) {
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col);
    printf("%s", s);
}

// 0(Black) 1(Dark Blue) 2(Dark Green) 3(Dark Sky Blue) 4(Dark Red)
// 5(Dark Purple) 6(Dark Yellow) 7(Gray) 8(Dark Gray) 9(Blue)
// 10(Green) 11(Sky Blue) 12(Red) 13(Purple) 14(Yellow) 15(White)


/*-----------------------------------------------------------------------------------------*/

void introScreen() {
    gotoxycol(6, 10, 14, "┏━━━┓┏┓┏┓┏┓━━━┏┓━━━━━┏┓━━━━━━┏━━━┓━━━━━━━━━━━━━━━━━━━━");
    gotoxycol(6, 11, 14, "┃┏━┓┃┃┃┃┃┃┃━━┏┛┗┓━━━━┃┃━━━━━━┗┓┏┓┃━━━━━━━━━━━━━━━━━━━━");
    gotoxycol(6, 12, 14, "┃┗━━┓┃┃┃┃┃┃┏┓┗┓┏┛┏━━┓┃┗━┓━━━━━┃┃┃┃┏━┓┏━━┓━┏━━┓┏━━┓┏━┓━");
    gotoxycol(6, 13, 14, "┗━━┓┃┃┗┛┗┛┃┣┫━┃┃━┃┏━┛┃┏┓┃━━━━━┃┃┃┃┃┏┛┗━┓┃━┃┏┓┃┃┏┓┃┃┏┓┓");
    gotoxycol(6, 14, 14, "┃┗━┛┃┗┓┏┓┏┛┃┃━┃┗┓┃┗━┓┃┃┃┃━━━━┏┛┗┛┃┃┃━┃┗┛┗┓┃┗┛┃┃┗┛┃┃┃┃┃");
    gotoxycol(6, 15, 14, "┗━━━┛━┗┛┗┛━┗┛━┗━┛┗━━┛┗┛┗┛━━━━┗━━━┛┗┛━┗━━━┛┗━┓┃┗━━┛┗┛┗┛");
    gotoxycol(6, 16, 14, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┏━┛┃━━━━━━━━");
    gotoxycol(6, 17, 14, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┗━━┛━━━━━━━━");

    while (1) {
        if (kbhit()) break;
        gotoxycol(47, 22, 14, "[ Press any key to start ]");
        Sleep(700);
        gotoxycol(47, 22, 14, "                          ");
        Sleep(100);
    }
    getch();
    system("cls");
    selectStage();
}

void selectStage() {
    gotoxycol(46, 8, 14, "▶ Press stage number to play: ");
    gotoxycol(50, 11, 14, "┏Stage 1: Classic");
    gotoxycol(50, 13, 14, "┏Stage 2: Off The Wall");
    gotoxycol(50, 15, 14, "┏Stage 3: 이무지개");
    gotoxycol(50, 17, 14, "┏Stage 4: 순간이무기동");
    gotoxycol(50, 19, 14, "┏Stage 5: 반짝이무기");

    while (1) {
        if (_kbhit()) {
            gotoxycol(46, 8, 14, "                              ");
            gotoxycol(48, 8, 14, "Press stage number to play: ");
            stage = getch();
            stage -= '0';
            switch (stage) {
            case 1: {
                gotoxycol(49, 11, 2, "▶ ┏Stage 1: Classic");
                Sleep(600);
                for (int i = 0; i < 7; i++) {
                    gotoxycol(49, 11, 2, "▶ ┏Stage 1: Classic");
                    Sleep(100);
                    gotoxycol(49, 11, 15, "▶ ┏Stage 1: Classic");
                    Sleep(100);
                }
                break;
            }
            case 2: {
                gotoxycol(50, 13, 2, "▶ ┏Stage 2: Off The Wall");
                Sleep(600);
                for (int i = 0; i < 7; i++) {
                    gotoxycol(50, 13, 2, "▶ ┏Stage 2: Off The Wall");
                    Sleep(100);
                    gotoxycol(50, 13, 15, "▶ ┏Stage 2: Off The Wall");
                    Sleep(100);
                }
                break;
            }
            case 3: {
                gotoxycol(50, 15, 2, "▶ ┏Stage 3: 이무지개");
                Sleep(600);
                for (int i = 0; i < 7; i++) {
                    gotoxycol(50, 15, 2, "▶ ┏Stage 3: 이무지개");
                    Sleep(100);
                    gotoxycol(50, 15, 15, "▶ ┏Stage 3: 이무지개");
                    Sleep(100);
                }
                break;
            }
            case 4: {
                gotoxycol(50, 17, 2, "▶ ┏Stage 4: 순간이무기동");
                Sleep(600);
                for (int i = 0; i < 7; i++) {
                    gotoxycol(50, 17, 2, "▶ ┏Stage 4: 순간이무기동");
                    Sleep(100);
                    gotoxycol(50, 17, 15, "▶ ┏Stage 4: 순간이무기동");
                    Sleep(100);
                }
                break;
            }
            case 5: {
                gotoxycol(50, 19, 14, "▶ ┏Stage 5: 반짝이무기");
                Sleep(600);
                for (int i = 0; i < 7; i++) {
                    gotoxycol(50, 19, 2, "▶ ┏Stage 5: 반짝이무기");
                    Sleep(100);
                    gotoxycol(50, 19, 15, "▶ ┏Stage 5: 반짝이무기");
                    Sleep(100);
                }
                break;
            }
            default: selectStage();
            }
            system("cls");
            drawBoard();
            break;
        }
    }
}

void initGameBoardInfo() { // setting up boundaries
    for (int y = 0; y < GBOARD_HEIGHT; y++) {
        gameBoardInfo[y][0] = 1;
        gameBoardInfo[y][GBOARD_WIDTH + 1] = 1;
    }
    for (int x = 0; x < GBOARD_WIDTH + 2; x++) {
        gameBoardInfo[0][x] = 1;
        gameBoardInfo[GBOARD_HEIGHT][x] = 1;
    }
}

void drawBoard() {
    // game title
    gotoxycol(54, 2, 14, "SWITCH DRAGON");
    initGameBoardInfo();
    // draw game board
    for (int y = 0; y <= GBOARD_HEIGHT; y++) {
        if (y == 0) gotoxycol(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y, 14, "▧");
        else if (y == GBOARD_HEIGHT) gotoxycol(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y, 14, "▧");
        else gotoxycol(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y, 14, "▧");

        if (y == 0) gotoxycol(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y, 14, "▧");
        else if (y == GBOARD_HEIGHT) gotoxycol(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y, 14, "▧");
        else gotoxycol(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y, 14, "▧");
    }
    for (int x = 1; x < GBOARD_WIDTH + 1; x++) {
        gotoxycol(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y, 14, "▧");
        gotoxycol(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT, 14, "▧");
    }
    // print ultimate
    drawUlt();
    drawFever();
    printStage();
    printScore();
    printHighscore();
    drawLife();
    drawControls();
    drawKeys();
}

void drawUlt() {
    // draw ultimate board
    for (int y = 0; y <= UBOARD_HEIGHT; y++) {
        if (y == 0) gotoxycol(UBOARD_ORIGIN_X, UBOARD_ORIGIN_Y + y, 14, "┏");
        else if (y == UBOARD_HEIGHT) gotoxycol(UBOARD_ORIGIN_X, UBOARD_ORIGIN_Y + y, 14, "┗");
        else gotoxycol(UBOARD_ORIGIN_X, UBOARD_ORIGIN_Y + y, 14, "┃");

        if (y == 0) gotoxycol(UBOARD_ORIGIN_X + (UBOARD_WIDTH + 1) * 2, UBOARD_ORIGIN_Y + y, 14, "┓");
        else if (y == UBOARD_HEIGHT) gotoxycol(UBOARD_ORIGIN_X + (UBOARD_WIDTH + 1) * 2, UBOARD_ORIGIN_Y + y, 14, "┛");
        else gotoxycol(UBOARD_ORIGIN_X + (UBOARD_WIDTH + 1) * 2, UBOARD_ORIGIN_Y + y, 14, "┃");
    }
    for (int x = 1; x < UBOARD_WIDTH + 1; x++) {
        gotoxycol(UBOARD_ORIGIN_X + x * 2, UBOARD_ORIGIN_Y, 14, "━");
        gotoxycol(UBOARD_ORIGIN_X + x * 2, UBOARD_ORIGIN_Y + UBOARD_HEIGHT, 14, "━");
    }
    // print ult
    gotoxycol(15, 25, 15, "ULT:");
    gotoxycol(19, 25, 15, "□□□□□");
}

void drawFever() {
    // draw fever board
    for (int y = 0; y <= FBOARD_HEIGHT; y++) {
        if (y == 0) gotoxycol(FBOARD_ORIGIN_X, FBOARD_ORIGIN_Y + y, 14, "┏");
        else if (y == FBOARD_HEIGHT) gotoxycol(FBOARD_ORIGIN_X, FBOARD_ORIGIN_Y + y, 14, "┗");
        else gotoxycol(FBOARD_ORIGIN_X, FBOARD_ORIGIN_Y + y, 14, "┃");

        if (y == 0) gotoxycol(FBOARD_ORIGIN_X + (FBOARD_WIDTH + 1) * 2, FBOARD_ORIGIN_Y + y, 14, "┓");
        else if (y == FBOARD_HEIGHT) gotoxycol(FBOARD_ORIGIN_X + (FBOARD_WIDTH + 1) * 2, FBOARD_ORIGIN_Y + y, 14, "┛");
        else gotoxycol(FBOARD_ORIGIN_X + (FBOARD_WIDTH + 1) * 2, FBOARD_ORIGIN_Y + y, 14, "┃");
    }
    for (int x = 1; x < FBOARD_WIDTH + 1; x++) {
        gotoxycol(FBOARD_ORIGIN_X + x * 2, FBOARD_ORIGIN_Y, 14, "━");
        gotoxycol(FBOARD_ORIGIN_X + x * 2, FBOARD_ORIGIN_Y + FBOARD_HEIGHT, 14, "━");
    }
    // print fever
    gotoxycol(56, 25, 15, "F  E  V  E  R");
}

void printStage() {
    switch (stage) {
    case 1: gotoxycol(72, 5, 15, "Stage 1: Classic"); break;
    case 2: gotoxycol(72, 5, 15, "Stage 2: Off The Wall"); break;
    case 3: gotoxycol(72, 5, 15, "Stage 3: 이무지개"); break;
    case 4: gotoxycol(72, 5, 15, "Stage 4: 순간이무기동"); break;
    case 5: gotoxycol(72, 5, 15, "Stage 5: 반짝이무기"); break;
    default: break;
    }
}

void printHighscore() {
    gotoxycol(72, 7, 15, "Highscore: ");
    setCurrentCursorPos(7, 80);
    printf("%d", stageBestScore[stage - 1]);
    //print Highscore after setting cursor position and calling from text file
}

void printScore() {
    gotoxycol(72, 9, 15, "Score: ");
    gotoxycol(79, 9, 15, "");
    printf("%d", currentScore);
    // print Score after setting cursor position
}

void drawLife() {
    gotoxycol(72, 11, 15, "Life:");
    gotoxycol(78, 11, 12, "");
    for (int i = 0; i < heart; i++) printf(" ♥");
    for (int i = heart; i < 3; i++) printf(" ♡");
}

void drawControls() {
    // draw fever board
    for (int y = 0; y <= CBOARD_HEIGHT; y++) {
        if (y == 0) gotoxycol(CBOARD_ORIGIN_X, CBOARD_ORIGIN_Y + y, 14, "┏");
        else if (y == CBOARD_HEIGHT) gotoxycol(CBOARD_ORIGIN_X, CBOARD_ORIGIN_Y + y, 14, "┗");
        else gotoxycol(CBOARD_ORIGIN_X, CBOARD_ORIGIN_Y + y, 14, "┃");

        if (y == 0) gotoxycol(CBOARD_ORIGIN_X + (CBOARD_WIDTH + 1) * 2, CBOARD_ORIGIN_Y + y, 14, "┓");
        else if (y == CBOARD_HEIGHT) gotoxycol(CBOARD_ORIGIN_X + (CBOARD_WIDTH + 1) * 2, CBOARD_ORIGIN_Y + y, 14, "┛");
        else gotoxycol(CBOARD_ORIGIN_X + (CBOARD_WIDTH + 1) * 2, CBOARD_ORIGIN_Y + y, 14, "┃");
    }
    for (int x = 1; x < CBOARD_WIDTH + 1; x++) {
        gotoxycol(CBOARD_ORIGIN_X + x * 2, CBOARD_ORIGIN_Y, 14, "━");
        gotoxycol(CBOARD_ORIGIN_X + x * 2, CBOARD_ORIGIN_Y + CBOARD_HEIGHT, 14, "━");
    }
}

void drawKeys() {
    gotoxycol(77, 14, 14, "CONTROLS");
    gotoxycol(80, 16, 7, "↑");
    gotoxycol(78, 17, 7, "←  →");
    gotoxycol(80, 18, 7, "↓");
    gotoxycol(73, 20, 7, "[R] to use ULT");
    gotoxycol(73, 21, 7, "▶: [P] ∥: [S]");
}

void gameOver() {
    system("cls");
    gotoxycol(38, 7, 12, "■■■■■  ■■■■■  ■■  ■■  ■■■■■");
    gotoxycol(38, 8, 12, "■          ■      ■  ■  ■  ■  ■");
    gotoxycol(38, 9, 12, "■  ■■■  ■■■■■  ■  ■  ■  ■■■■■");
    gotoxycol(38, 10, 12, "■      ■  ■      ■  ■      ■  ■");
    gotoxycol(38, 11, 12, "■■■■■  ■      ■  ■      ■  ■■■■■");
    gotoxycol(38, 13, 12, "■■■■■  ■      ■  ■■■■■  ■■■■");
    gotoxycol(38, 14, 12, "■      ■  ■      ■  ■          ■      ■");
    gotoxycol(38, 15, 12, "■      ■  ■      ■  ■■■■■  ■■■■");
    gotoxycol(38, 16, 12, "■      ■    ■  ■    ■          ■      ■");
    gotoxycol(38, 17, 12, "■■■■■      ■      ■■■■■  ■      ■");

    gotoxycol(58, 20, 15, "SCORE ");
    // print final score
    gotoxycol(60, 22, 14, "");
    printf("%d", currentScore);
    gotoxycol(56, 24, 15, "HIGHSCORE ");
    gotoxycol(60, 26, 14, "");
    printf("%d", stageBestScore[stage - 1]);
    // print highscore

    while (1) {
        if (kbhit()) {
            system("cls");
            selectStage(); // Should change to Story/Item/Game Play Screen
            break;
        }
        gotoxycol(47, 28, 14, "[ Press any key to restart ]");
        Sleep(700);
        gotoxycol(47, 28, 14, "                            ");
        Sleep(100);
    }
}
/*----------------------------------------------------------------------------------------------*/

void initMoogi() {

    COORD headPos = { GBOARD_ORIGIN_X + GBOARD_WIDTH , GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 };
    COORD bodyPos = { GBOARD_ORIGIN_X + GBOARD_WIDTH - 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 };
    COORD tailPos = { GBOARD_ORIGIN_X + GBOARD_WIDTH - 4, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 };

    head = getNode(NULL, NULL, headPos);
    body = getNode(NULL, NULL, bodyPos);
    tail = getNode(NULL, NULL, tailPos);

    head->back = body;
    body->front = head;
    body->back = tail;
    tail->front = body;

    head->direct = RIGHT;
    body->direct = RIGHT;
    tail->direct = RIGHT;

    item = (Item*)malloc(sizeof(Item));
    item->itemNo = -1;

    int arrX = (head->position.X - GBOARD_ORIGIN_X) / 2;
    int arrY = head->position.Y - GBOARD_ORIGIN_Y;
    gameBoardInfo[arrY][arrX] = 2;
    gameBoardInfo[arrY][arrX - 1] = 2;
    gameBoardInfo[arrY][arrX - 2] = 2;

    if (stage == 3) {
        changeMoogiColor();
        changeMoogiBodyColor();
    }
    gotoxy(head->position.X, head->position.Y, "◎");
    gotoxy(body->position.X, body->position.Y, "●");
    gotoxy(tail->position.X, tail->position.Y, "●");
}

void addBody() {
    if (length + 1 > maxLength) return;

    length++;

    COORD newTailPos = { tail->position.X, tail->position.Y };

    switch (tail->direct) {
    case LEFT:
        newTailPos.X += 2; break;
    case RIGHT:
        newTailPos.X -= 2; break;
    case UP:
        newTailPos.Y++; break;
    case DOWN:
        newTailPos.Y--; break;
    default:
        break;
    }

    Moogi* p = getNode(tail, NULL, newTailPos);
    p->direct = tail->direct;
    tail->back = p;
    tail = p;

    if (!detectCollision(tail->position.X, tail->position.Y))
    {
        gotoxy(tail->position.X, tail->position.Y, "●");

        int arrX = (tail->position.X - GBOARD_ORIGIN_X) / 2;
        int arrY = tail->position.Y - GBOARD_ORIGIN_Y;
        gameBoardInfo[arrY][arrX] = 2;
    }
}

void deleteBody() {
    if (length > minLength) { // 최소길이보다 클 때만 실행
        eraseTail();
        length--;
    }
}

void speedUp() {
    speed -= 5;
    if (speed < minSpeed) speed = minSpeed; // 최대 속도 리밋
}

void speedDown() {
    speed += 5;
    if (speed > maxSpeed) speed = maxSpeed; // 최소 속도 리밋
}

void createItem() {
    if (itemCount != 0) return; // 아이템 생성 조건, 화면에 아이템이 있으면 생성하지 않음
    time(&itemCreationTime);

    item->itemNo = rand() % 2 + 4; // 아이템 종류 2가지
    item->pos.X = rand() % GBOARD_WIDTH + GBOARD_ORIGIN_X;
    item->pos.Y = rand() % GBOARD_HEIGHT + GBOARD_ORIGIN_Y;

    do {
        item->pos.X = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (item->pos.X % 2 == 1) item->pos.X++;
        item->pos.Y = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
    } while (detectCollision(item->pos.X, item->pos.Y)); // 아무것도 없으면 0 반환됨

    int arrX = (item->pos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (item->pos.Y - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = item->itemNo;

    switch (item->itemNo) {
    case 4:
        setTextColor(1);
        gotoxy(item->pos.X, item->pos.Y, "▲"); break;
    case 5:
        setTextColor(4);
        gotoxy(item->pos.X, item->pos.Y, "▼"); break;
    default:
        gotoxy(INFO_X, ITEM_Y, "아이템 생성에 실패함");
        break;
    }

    itemCount++;

    setTextColor(15);
}

void deleteItem() { // 삭제만 처리 or 과정까지 처리
    time_t currentTime;
    time(&currentTime);

    int arrX = (item->pos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (item->pos.Y - GBOARD_ORIGIN_Y);

    if ((int)difftime(currentTime, itemCreationTime) >= 7) {
        item->itemNo = -1;
        gotoxy(item->pos.X, item->pos.Y, "  ");
        gameBoardInfo[arrY][arrX] = 0;
        itemCount--;
        createItem();
    }
}

void getHeart() {
    if (heart < 3) // 생명 개수 제한... 그렇게 많이 부딪히지 않음. 약간 재미 요소 down..
        heart++;
}

void getPet() {
    // 화면에 궁 표시
    // press enter 궁 사용
}

void usePet() {
    if (petGauge < 5) return;

    pet = (Pet*)malloc(sizeof(Pet));
    time(&petCreationTime);

    // 현재 꼬리의 좌표, 좌표를 떼어서 아이템의 좌표로 이동(최단거리로)
    pet->pos.X = tail->position.X;
    pet->pos.Y = tail->position.Y;
    eraseTail();
    gotoxy(pet->pos.X, pet->pos.Y, "⊙"); // 수정 : 확인용, 위치 확인 필요

    int arrX = (pet->pos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = pet->pos.Y - GBOARD_ORIGIN_Y;
    gameBoardInfo[arrY][arrX] = 13;
}

void petMove()
{
    time_t currentTime;
    time(&currentTime);
    if ((int)difftime(currentTime, petCreationTime) >= 15)
    {
        gotoxy(pet->pos.X, pet->pos.Y, "  ");
        int arrX = (pet->pos.X - GBOARD_ORIGIN_X) / 2;
        int arrY = pet->pos.Y - GBOARD_ORIGIN_Y;
        gameBoardInfo[arrY][arrX] = 0;

        free(pet);
        pet = NULL;
        petGauge = 0;

        addBody();

        return;
    }

    int xPlus = pet->pos.X < item->pos.X ? 2 : (pet->pos.X == item->pos.X ? 0 : -2);
    int yPlus = pet->pos.Y < item->pos.Y ? 1 : (pet->pos.Y == item->pos.Y ? 0 : -1);

    int move = rand() % 2;
    COORD pos = { pet->pos.X, pet->pos.Y };

    if (move) pos.Y += yPlus;
    else pos.X += xPlus;

    if (pos.X == item->pos.X && pos.Y == item->pos.Y)
    {
        gotoxy(pet->pos.X, pet->pos.Y, "  ");
        pet->pos.X = pos.X;
        pet->pos.Y = pos.Y;
        gotoxycol(pet->pos.X, pet->pos.Y, 2, "⊙");
        setTextColor(15);

        getItemOfPet();

        gotoxy(pet->pos.X, pet->pos.Y, "  ");
        int arrX = (pet->pos.X - GBOARD_ORIGIN_X) / 2;
        int arrY = pet->pos.Y - GBOARD_ORIGIN_Y;
        gameBoardInfo[arrY][arrX] = 0;

        free(pet);
        pet = NULL;
        petGauge = 0;

        gotoxycol(19, 25, 2, "");
        setTextColor(15);
        for (int i = 0; i < 5; i++) printf("□");

        addBody();

        return;
    }

    if (detectCollision(pos.X, pos.Y))
    {
        pos.X = pet->pos.X; pos.Y = pet->pos.Y;
        if (!move) pos.Y += yPlus;
        else pos.X += xPlus;
    }

    if (detectCollision(pos.X, pos.Y))
    {
        pos.X = pet->pos.X; pos.Y = pet->pos.Y;
        if (move) pos.Y -= yPlus;
        else pos.X -= xPlus;

        if (detectCollision(pos.X, pos.Y))
        {
            pos.X = pet->pos.X; pos.Y = pet->pos.Y;
            if (!move) pos.Y -= yPlus;
            else pos.X -= xPlus;
        }
    }

    gotoxycol(pet->pos.X, pet->pos.Y, 15, "  ");
    int arrX = (pet->pos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = pet->pos.Y - GBOARD_ORIGIN_Y;
    gameBoardInfo[arrY][arrX] = 0;

    pet->pos.X = pos.X;
    pet->pos.Y = pos.Y;
    gotoxycol(pet->pos.X, pet->pos.Y, 2, "⊙");
    setTextColor(15);

    arrX = (pet->pos.X - GBOARD_ORIGIN_X) / 2;
    arrY = pet->pos.Y - GBOARD_ORIGIN_Y;
    gameBoardInfo[arrY][arrX] = 13;
}

void getItemOfPet()
{
    itemCount--;
    if (item->itemNo == 4) {    //좋은아이템(길이↓, 속도↓, FEVER, 실드, 여의주생성)
        //int itemType = (rand() % 5) + 1;//(rand() % 5) + 1; 원래 이건데 지금은 아이템 2개만
        int itemType = 4;

        switch (itemType) {
        case 1:        //길이↓
            deleteBody();
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "길이 감소!");
            break;
        case 2:        //속도↓
            speedDown();
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "속도 감소!");
            break;
        case 3:
            getHeart();
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "생명 +1"); // 수정 : 멘트, 이전 멘트를 지워야 할 거 같음
            break;
        case 4:
            dragonBallBomb();
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "여의주 폭탄"); // 수정 : 멘트
            break;
        case 5:
            //getPet();
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "펫 획득"); // 수정 : 멘트
            break;
        default:
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "!!!!!!!");
            break;
        }

        createItem();
    }
    else if (item->itemNo == 5) {    //안좋은아이템 (길이↑, 속도↑)
        int itemType = (rand() % 2) + 1;

        switch (itemType) {
        case 1:        //길이↑
            addBody();
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "길이 증가!");
            break;
        case 2:        //속도↑
            speedUp();
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "속도 증가!");
            break;
        default:
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "!!!!!!!");
            break;
        }

        createItem();
    }
}

void dragonBallBomb() {
    int multipleDragonBall = rand() % 3 + 2; // 2~4
    for (int i = 0; i < multipleDragonBall; i++) {
        addDragonBall();
    }
}

void moogiSwitch() {
    Moogi* p = tail, * pp = p;
    Moogi* pH = NULL, * pT = NULL;
    Moogi* pi = NULL, * pii = NULL;

    while (p != NULL)
    {
        if (pii != NULL)
        {
            pi = getNode(pii, NULL, p->position);
            pii->back = pi;
        }
        else pi = getNode(NULL, NULL, p->position);

        pii = pi;
        pp = p;

        if (p == tail) pH = pi;

        switch (p->direct)
        {
        case UP:
            pi->direct = DOWN;
            break;
        case DOWN:
            pi->direct = UP;
            break;
        case LEFT:
            pi->direct = RIGHT;
            break;
        case RIGHT:
            pi->direct = LEFT;
            break;
        default:
            break;
        }

        p = p->front;
        pi = pi->back;

        free(pp);
    }
    pT = pii;

    head = pH;
    tail = pT;

    gotoxy(head->position.X, head->position.Y, "◎");
    Moogi* tmp = head->back;
    while (tmp != NULL)
    {
        gotoxy(tmp->position.X, tmp->position.Y, "●");
        tmp = tmp->back;
    }

    int arrX = (tail->position.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (tail->position.Y - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = 2;

    switch (head->direct)
    {
    case UP:
        direction = 0; break;
    case DOWN:
        direction = 1; break;
    case LEFT:
        direction = 2; break;
    case RIGHT:
        direction = 3; break;
    }

    Sleep(100);
}

/*----------------------------------------------------------------------------------------------*/

void inPlayKeyInput() // 플레이 중의 키 입력
// 이무기 조작 및 일시중지
{
    int i, key; // , isDone = 0;

    for (i = 0; i < speed; i++)
    {
        if (_kbhit() != 0)
        {
            key = _getch();
            switch (key)
            {
            case UP:
                shiftUp();
                break;
            case DOWN:
                shiftDown();
                break;
            case LEFT:
                shiftLeft();
                break;
            case RIGHT:
                shiftRight();
                break;
            case SPACE:
                pausePlay();
                break;
            case ENTER:
                usePet();
                break;
            default:
                break;
            }

        }

        Sleep(15); // 플레이 속도 조절
    }
}

void shiftUp() // 플레이 중 up 방향키 입력 시
{
    // 충돌 확인, 적절치 못한 방향 전환인지 확인
    if (direction == 0 || direction == 1) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;

    // 방향 업데이트, 이동에 따라 head 및 tail만 redraw
    direction = 0;
    drawHead(nextPos);
    eraseTail();
}

void shiftDown() // 플레이 중 down 방향키 입력 시
{
    if (direction == 0 || direction == 1) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;

    direction = 1;
    drawHead(nextPos);
    eraseTail();
}

void shiftLeft() // 플레이 중 left 방향키 입력 시
{
    if (direction == 2 || direction == 3) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;

    direction = 2;
    drawHead(nextPos);
    eraseTail();
}

void shiftRight() // 플레이 중 right 방향키 입력 시
{
    if (direction == 2 || direction == 3) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;

    direction = 3;
    drawHead(nextPos);
    eraseTail();
}

void pausePlay() // 일시정지 -> 재시작 혹은 리셋을 기다림
// 화면이 어떻게 구성되는지 몰라서
// 일단 아래쪽 방향키 누르면 홈화면으로 돌아가기
// 위쪽 방향키 누르면 그냥 이어서 게임 진행
// 스페이스 누르면 선택 확정으로 해놨음
{
    while (1)
    {
        if (_kbhit() != 0)
        {
            int key = _getch();

            if (key == SPACE) break;
        }
        Sleep(speed);
    }
}

int detectCollision(int posX, int posY) // 충돌 감지
// 이 함수의 입력 인자로 이무기 머리 위치를 넣어주면 됨
// 충돌 감지하고 싶으면 direction에 따라 한칸 움직인 위치
// 그냥 nextHeadPos() 값을 넣어주면 됨
{
    int arrX = (posX - GBOARD_ORIGIN_X) / 2;
    int arrY = (posY - GBOARD_ORIGIN_Y);

    return gameBoardInfo[arrY][arrX];
}

COORD nextHeadPos() // head의 다음 위치 반환
{
    COORD curPos;
    curPos.X = head->position.X;
    curPos.Y = head->position.Y;

    switch (direction)
    {
    case 0:
        curPos.Y--;
        break;
    case 1:
        curPos.Y++;
        break;
    case 2:
        curPos.X -= 2;
        break;
    case 3:
        curPos.X += 2;
        break;
    default:
        break;
    }

    return curPos;
}

void drawHead(COORD headPos) // 이무기의 이동을 출력(head)
// 이무기 연결리스트 앞에 노드 하나 추가하고
// 머리랑 그 바로 뒤 한 칸(이전 위치에 출력된 머리◎를 지우기 위해)을 출력
{
    Moogi* node = getNode(NULL, head, headPos);
    head->front = node;
    head = head->front;

    switch (direction)
    {
    case 0:
        head->direct = UP;
        break;
    case 1:
        head->direct = DOWN;
        break;
    case 2:
        head->direct = LEFT;
        break;
    case 3:
        head->direct = RIGHT;
        break;
    default:
        head->direct = UP;
        break;
    }

    if (stage == 3) {
        setTextColor_rygbw(moogiColor);
    }

    gotoxy(head->position.X, head->position.Y, "◎");
    gotoxy(head->back->position.X, head->back->position.Y, "●");

    getSomething();

    int arrX = (headPos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (headPos.Y - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = 2;
}

void eraseTail() // 이무기의 이동을 출력(tail)
// 꼬리를 한 칸 앞으로 이동시키고 마지막 노드를 삭제(drawHead에서 하나 추가하니까 length는 똑같음)
// 그리고 지나간 자리에 남은 출력을 공백문자로 지움
{
    if (tail == NULL) return;

    if (detectCollision(tail->position.X, tail->position.Y) == 2)
    {
        int arrX = (tail->position.X - GBOARD_ORIGIN_X) / 2;
        int arrY = (tail->position.Y - GBOARD_ORIGIN_Y);
        gameBoardInfo[arrY][arrX] = 0;

        gotoxy(tail->position.X, tail->position.Y, "  ");
    }

    Moogi* pi = tail;
    tail = tail->front;
    tail->back = NULL;
    free(pi);
}

Moogi* getNode(Moogi* front, Moogi* back, COORD position) // 이무기 노드 하나 생성
{
    Moogi* node = (Moogi*)malloc(sizeof(Moogi));
    node->front = front;
    node->back = back;
    node->position = position;

    return node;
}

int moogiMove()
{
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1 || detectCollision(nextPos.X, nextPos.Y) == 2)
    {
        if (heart > 0) { // 목숨 남았을 때
            waitToRecover();
            return 1;
        }
        else return 0;
    }
    gotoxy(0, 0, "                  ");

    if (detectCollision(nextPos.X, nextPos.Y) != 13)
    {
        drawHead(nextPos);      //얘가 getSomething 부름, getSomething이 countScore랑 getItem 부름,
        eraseTail();
    }

    if (stage == 2) wallMove();
    if (pet != NULL) petMove();

    return 1;
}

void getSomething()
{
    deleteItem();
    countScore();
    getItem();
}

void wallMove()
{
    Wall* p = wallHead;

    while (p->next != NULL)
    {
        p = p->next;

        COORD pos = { p->position.X, p->position.Y };

        while (1)
        {
            pos.X = p->position.X;
            pos.Y = p->position.Y;

            switch (p->direct)
            {
            case 0:
                pos.Y--;
                break;
            case 1:
                pos.Y++;
                break;
            case 2:
                pos.X -= 2;
                break;
            case 3:
                pos.X += 2;
                break;
            default:
                break;
            }

            if (!detectCollision(pos.X, pos.Y)) break;

            p->direct = rand() % 4;
        }

        int arrX = (p->position.X - GBOARD_ORIGIN_X) / 2;
        int arrY = (p->position.Y - GBOARD_ORIGIN_Y);
        gameBoardInfo[arrY][arrX] = 0;

        arrX = (pos.X - GBOARD_ORIGIN_X) / 2;
        arrY = (pos.Y - GBOARD_ORIGIN_Y);
        gameBoardInfo[arrY][arrX] = 1;

        setTextColor(14);
        gotoxy(p->position.X, p->position.Y, "  ");
        gotoxy(pos.X, pos.Y, "▨");
        setTextColor(15);

        p->position.X = pos.X;
        p->position.Y = pos.Y;
    }
}

/*-------------------------------------------------------------------------------------------*/

void countScore() {
    if (detectCollision(head->position.X, head->position.Y) == 3) {
        if (dragonBallCount > 0) // 개수가 음수가 되지 않게
            dragonBallCount--;
        currentScore += 10;

        gotoxycol(79, 9, 15, "");
        printf("%d", currentScore);

        if (currentScore > 0 && currentScore % 10 == 0)
        {
            petGauge++; if (petGauge > 5) petGauge = 5;
            gotoxycol(19, 25, 2, "");
            for (int i = 0; i < petGauge; i++) printf("■");
            setTextColor(15);
            for (int i = petGauge; i < 5; i++) printf("□");
        }

        if (stage < 4) // 스테이지 4, 5는 스위치 기능 off
            moogiSwitch();

        addBody();

        if (stage == 3)
        {
            changeMoogiColor();
            changeMoogiBodyColor();
        }

        if (dragonBallCount == 0) { // 여의주가 많으면 추가로 생성하지 않음
            addDragonBall();
        }
        if (stage == 2) addWall();
    }
}

void getBestScore() {    //게임 시작 시 플레이 전에 반드시 불러야 한다...(1번만)
    FILE* fp_r = fopen("BestScore.txt", "r");
    if (fp_r == NULL) {
        return;
    }

    for (int i = 0; i < 5; i++) {
        fscanf(fp_r, "%d", &stageBestScore[i]);
    }
    fclose(fp_r);        //파일에서 최고점수 얻어옴 → 파일 비우고 stageBestScore배열에 저장
}

void setBestScore(int score) {            //게임오버됐을때만 부르면 될 듯?
    if (score > stageBestScore[stage - 1]) {
        stageBestScore[stage - 1] = score;
    }
}

void saveBestScore() {    //게임 종료 시 다시 최고점수를 파일에 저장하는 역할
    FILE* fp_w = fopen("BestScore.txt", "w");
    for (int i = 0; i < 5; i++) {
        fprintf(fp_w, "%d\n", stageBestScore[i]);
    }
    fclose(fp_w);
}

int isGameOver() {        //리턴값이 1이나 2면 게임종료해야됨
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1 || detectCollision(nextPos.X, nextPos.Y) == 2) {
        if (heart > 1)
        {
            waitToRecover();
            return 0;
        }
        return 1;        //박으면 1리턴
    }
    return 0;            //아니면 0
}

void getItem() {
    if (detectCollision(head->position.X, head->position.Y) == 4 || detectCollision(head->position.X, head->position.Y) == 5)
    {
        itemCount--;
        if (item->itemNo == 4) {    //좋은아이템(길이↓, 속도↓, FEVER, 실드, 여의주생성)
            //int itemType = (rand() % 5) + 1;//(rand() % 5) + 1; 원래 이건데 지금은 아이템 2개만
            int itemType = 4;

            switch (itemType) {
            case 1:        //길이↓
                deleteBody();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "길이 감소!");
                break;
            case 2:        //속도↓
                speedDown();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "속도 감소!");
                break;
            case 3:
                getHeart();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "생명 +1"); // 수정 : 멘트, 이전 멘트를 지워야 할 거 같음
                break;
            case 4:
                dragonBallBomb();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "여의주 폭탄"); // 수정 : 멘트
                break;
            case 5:
                getPet();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "펫 획득"); // 수정 : 멘트
                break;
            default:
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "!!!!!!!");
                break;
            }

            createItem();
        }
        else if (item->itemNo == 5) {    //안좋은아이템 (길이↑, 속도↑)
            int itemType = (rand() % 2) + 1;

            switch (itemType) {
            case 1:        //길이↑
                addBody();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "길이 증가!");
                break;
            case 2:        //속도↑
                speedUp();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "속도 증가!");
                break;
            default:
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "!!!!!!!");
                break;
            }

            createItem();
        }
    }
    //0 아무것도없는거, 1 벽장애물, 2 이무기몸, 3 여의주, 4 좋은아이템, 5 나쁜아이템,
}

void addDragonBall() {
    int ballX;
    int ballY;

    if (stage == 3) {
        deleteDragonBall();
        for (int i = 0; i < 5; i++) {
            do {
                ballX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (ballX % 2 == 1) ballX++;
                ballY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
            } while (detectCollision(ballX, ballY));

            int arrX = (ballX - GBOARD_ORIGIN_X) / 2;
            int arrY = (ballY - GBOARD_ORIGIN_Y);
            if (moogiColor == dragonBallColor[i])
                gameBoardInfo[arrY][arrX] = 3;
            else
                gameBoardInfo[arrY][arrX] = 1;
            dragonBallPos[i].X = ballX;
            dragonBallPos[i].Y = ballY;
            showColorDragonBall(ballX, ballY, dragonBallColor[i]);
        }
    }
    else {
        do {
            ballX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (ballX % 2 == 1) ballX++;
            ballY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
        } while (detectCollision(ballX, ballY));

        int arrX = (ballX - GBOARD_ORIGIN_X) / 2;
        int arrY = (ballY - GBOARD_ORIGIN_Y);
        gameBoardInfo[arrY][arrX] = 3;
        showDragonBall(ballX, ballY);dragonBallCount++;
    }


}

void showDragonBall(int x, int y) {
    setTextColor(6);
    gotoxy(x, y, "○");
    setTextColor(15);
}


// 호출 : 구슬을 먹으면 실행
// 이미 벽 또는 아이템이 출력된 곳에 출력하면 X
void addWall() {
    if (dragonBallCount > 1) return; // 벽 생성 조건(구슬 개수)

    int wallX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X;
    int wallY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;

    do {
        wallX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (wallX % 2 == 1) wallX++;
        wallY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
    } while (detectCollision(wallX, wallY)); // 아무것도 없으면 0 반환됨

    setTextColor(14);
    gotoxy(wallX, wallY, "▨");
    setTextColor(15);

    int arrX = (wallX - GBOARD_ORIGIN_X) / 2;
    int arrY = (wallY - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = 1; // 벽 1

    Wall* node = (Wall*)malloc(sizeof(Wall));
    node->direct = rand() % 4;
    node->position.X = wallX;
    node->position.Y = wallY;
    node->next = NULL;

    Wall* p = wallHead;
    while (p->next != NULL) p = p->next;
    p->next = node;
}

//function for stage3
void setTextColor_rygbw(int colorNum) {
    switch (colorNum) {
    case 0: colorNum = 4; break;
    case 1: colorNum = 6; break;
    case 2: colorNum = 2; break;
    case 3: colorNum = 9; break;
    case 4: colorNum = 15; break;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

void changeMoogiColor() {      //이무기 색깔 세팅, 0빨 1노 2초 3파 4흰
    moogiColor = rand() % 5;
}
void changeMoogiBodyColor() {   //이무기 몸 색깔 바꿈(countScore에서 불림)
    setTextColor_rygbw(moogiColor);
    gotoxy(head->position.X, head->position.Y, "◎");
    Moogi* p = head->back;
    while (p->back != NULL) {
        gotoxy(p->position.X, p->position.Y, "●");
        p = p->back;
    }
}

void setDragonBallColor() {     //한 번만 호출, 초기화용도
    for (int i = 0; i < 5; i++)
        dragonBallColor[i] = i;     //0빨 1노 2초 3파 4흰
}

void setDragonBallPos() {       //한 번만 호출, 초기화용도.
    for (int i = 0; i < 5; i++) {
        dragonBallPos[i].X = 0;
        dragonBallPos[i].Y = 0;
    }
}

void showColorDragonBall(int x, int y, int color) {
    setTextColor_rygbw(color);  //0빨 1노 2초 3파 4흰
    gotoxy(x, y, "○");
    setTextColor(15);
}

void deleteDragonBall() {       //여의주 먹은 후 같은 색 아닌 여의주 지우는 용도(addDragonBall에서 불림)
    for (int i = 0; i < 5; i++) {
        if (dragonBallPos[i].X != 0 && dragonBallPos[i].Y != 0) {
            int arrX = (dragonBallPos[i].X - GBOARD_ORIGIN_X) / 2;
            int arrY = (dragonBallPos[i].Y - GBOARD_ORIGIN_Y);
            if (gameBoardInfo[arrY][arrX] == 1) {
                gameBoardInfo[arrY][arrX] = 0;
                gotoxy(dragonBallPos[i].X, dragonBallPos[i].Y, "  ");
            }
            dragonBallPos[i].X = 0;
            dragonBallPos[i].Y = 0;
        }
    }
}

void waitToRecover() { // 여기서 키를 새로 입력 받을지, stage로 보내서(moogimove return 1) 입력받을지
    int key, flag = 0;
    gotoxycol(INFO_X, ITEM_Y + 1, 4, "PRESS TO CONTINUE");
    Moogi* p = head->back;
    COORD nextPos;

    for (int i = 0; i < 5; i++) {
        for (int i = 0; ; i++) {
            if (stage == 3) {
                setTextColor_rygbw(moogiColor);
                gotoxy(p->position.X, p->position.Y, "○");
            }
            else
                gotoxycol(p->position.X, p->position.Y, 15, "○");
            if (p->back == NULL) break;
            p = p->back;
        }
        Sleep(300);
        p = head->back;
        for (int i = 0; ; i++) {
            if (stage == 3) {
                setTextColor_rygbw(moogiColor);
                gotoxy(p->position.X, p->position.Y, "●");
            }
            else
                gotoxycol(p->position.X, p->position.Y, 15, "●");
            if (p->back == NULL) break;
            p = p->back;
        }
        Sleep(100);
        for (int i = 0; i < speed; i++)
        {
            if (_kbhit() != 0)
            {
                key = _getch();

                if (key == UP)
                {
                    if (direction == 0 || direction == 1) continue;
                    direction = 0;
                    nextPos = nextHeadPos();
                    drawHead(nextPos);
                    eraseTail();
                    flag = 1;
                }
                else if (key == DOWN)
                {
                    if (direction == 0 || direction == 1) continue;
                    direction = 1;
                    nextPos = nextHeadPos();
                    drawHead(nextPos);
                    eraseTail();
                    flag = 1;
                }
                else if (key == LEFT)
                {
                    if (direction == 2 || direction == 3) continue;
                    direction = 2;
                    nextPos = nextHeadPos();
                    drawHead(nextPos);
                    eraseTail();
                    flag = 1;
                }
                else if (key == RIGHT)
                {
                    if (direction == 2 || direction == 3) continue;
                    direction = 3;
                    nextPos = nextHeadPos();
                    drawHead(nextPos);
                    eraseTail();
                    flag = 1;
                }
                else if (key == SPACE)
                {
                    pausePlay();
                }
            }

            Sleep(15); // 플레이 속도 조절
        }
        p = head->back;
        if (flag == 1) break;
    }

    heart--;
    drawLife();
    gotoxycol(INFO_X, ITEM_Y + 1, 14, "                   ");
    gotoxy(GBOARD_WIDTH + 3, GBOARD_HEIGHT + 5, "");
    setTextColor(15);

    if (flag == 0) {
        return;
    }
}
