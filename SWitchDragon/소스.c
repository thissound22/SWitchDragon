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
int dragonBallCount = 0;
int itemCount = 0;

int fever[5] = { 0,0,0,0,0 };
int feverStart = 0;
int feverScore = 20;
time_t ferverStartTime;

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
int moogiColor = 4;   //5∞°¡ˆ ªˆªÛ∏∏ ªÁøÎ. 0°Ê4(dark red), 1°Ê6(dark yellow), 2°Ê2(dark green), 3°Ê9(blue), 4°Ê15(white)
int dragonBallColor[5];     //0ª° 1≥Î 2√  3∆ƒ 4»Ú ∞Ì¡§, æÓ¬˜«« ¿ßƒ°∑£¥˝µπ∏±∞≈¥œ±Ó æÍ º¯º≠¥¬ ªÛ∞¸§§§§
COORD dragonBallPos[5];     //ª°≥Î√ ∆ƒ»Ú º¯º≠¥Î∑Œ ¿ßƒ° ¿˙¿Â. æÀ∏¬¿∫ ªˆ±Ú¿« ø©¿«¡÷∏¶ ∏‘¿∏∏È ≥™∏”¡ˆµÈ¿∫ æ¯æ÷¡‡æﬂ«œ±‚∂ßπÆ..«œ..

int petGauge = 0;
int petScore = 0;
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

void createFever();
void getFever();
int isFever();
void fillFever();
void removeFever();

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
    gotoxycol(6, 10, 14, "¶Æ¶¨¶¨¶¨¶Ø¶Æ¶Ø¶Æ¶Ø¶Æ¶Ø¶¨¶¨¶¨¶Æ¶Ø¶¨¶¨¶¨¶¨¶¨¶Æ¶Ø¶¨¶¨¶¨¶¨¶¨¶¨¶Æ¶¨¶¨¶¨¶Ø¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨");
    gotoxycol(6, 11, 14, "¶≠¶Æ¶¨¶Ø¶≠¶≠¶≠¶≠¶≠¶≠¶≠¶¨¶¨¶Æ¶∞¶±¶Ø¶¨¶¨¶¨¶¨¶≠¶≠¶¨¶¨¶¨¶¨¶¨¶¨¶±¶Ø¶Æ¶Ø¶≠¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨");
    gotoxycol(6, 12, 14, "¶≠¶±¶¨¶¨¶Ø¶≠¶≠¶≠¶≠¶≠¶≠¶Æ¶Ø¶±¶Ø¶Æ¶∞¶Æ¶¨¶¨¶Ø¶≠¶±¶¨¶Ø¶¨¶¨¶¨¶¨¶¨¶≠¶≠¶≠¶≠¶Æ¶¨¶Ø¶Æ¶¨¶¨¶Ø¶¨¶Æ¶¨¶¨¶Ø¶Æ¶¨¶¨¶Ø¶Æ¶¨¶Ø¶¨");
    gotoxycol(6, 13, 14, "¶±¶¨¶¨¶Ø¶≠¶≠¶±¶∞¶±¶∞¶≠¶≤¶¥¶¨¶≠¶≠¶¨¶≠¶Æ¶¨¶∞¶≠¶Æ¶Ø¶≠¶¨¶¨¶¨¶¨¶¨¶≠¶≠¶≠¶≠¶≠¶Æ¶∞¶±¶¨¶Ø¶≠¶¨¶≠¶Æ¶Ø¶≠¶≠¶Æ¶Ø¶≠¶≠¶Æ¶Ø¶Ø");
    gotoxycol(6, 14, 14, "¶≠¶±¶¨¶∞¶≠¶±¶Ø¶Æ¶Ø¶Æ¶∞¶≠¶≠¶¨¶≠¶±¶Ø¶≠¶±¶¨¶Ø¶≠¶≠¶≠¶≠¶¨¶¨¶¨¶¨¶Æ¶∞¶±¶∞¶≠¶≠¶≠¶¨¶≠¶±¶∞¶±¶Ø¶≠¶±¶∞¶≠¶≠¶±¶∞¶≠¶≠¶≠¶≠¶≠");
    gotoxycol(6, 15, 14, "¶±¶¨¶¨¶¨¶∞¶¨¶±¶∞¶±¶∞¶¨¶±¶∞¶¨¶±¶¨¶∞¶±¶¨¶¨¶∞¶±¶∞¶±¶∞¶¨¶¨¶¨¶¨¶±¶¨¶¨¶¨¶∞¶±¶∞¶¨¶±¶¨¶¨¶¨¶∞¶±¶¨¶Ø¶≠¶±¶¨¶¨¶∞¶±¶∞¶±¶∞");
    gotoxycol(6, 16, 14, "¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶Æ¶¨¶∞¶≠¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨");
    gotoxycol(6, 17, 14, "¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶±¶¨¶¨¶∞¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨");

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
    gotoxycol(46, 8, 14, "¢∫ Press stage number to play: ");
    gotoxycol(50, 11, 14, "¶ÆStage 1: Classic");
    gotoxycol(50, 13, 14, "¶ÆStage 2: Off The Wall");
    gotoxycol(50, 15, 14, "¶ÆStage 3: ¿Ãπ´¡ˆ∞≥");
    gotoxycol(50, 17, 14, "¶ÆStage 4: º¯∞£¿Ãπ´±‚µø");
    gotoxycol(50, 19, 14, "¶ÆStage 5: π›¬¶¿Ãπ´±‚");

    while (1) {
        if (_kbhit()) {
            gotoxycol(46, 8, 14, "                              ");
            gotoxycol(48, 8, 14, "Press stage number to play: ");
            stage = getch();
            stage -= '0';
            switch (stage) {
            case 1: {
                gotoxycol(49, 11, 2, "¢∫ ¶ÆStage 1: Classic");
                Sleep(600);
                for (int i = 0; i < 7; i++) {
                    gotoxycol(49, 11, 2, "¢∫ ¶ÆStage 1: Classic");
                    Sleep(100);
                    gotoxycol(49, 11, 15, "¢∫ ¶ÆStage 1: Classic");
                    Sleep(100);
                }
                break;
            }
            case 2: {
                gotoxycol(50, 13, 2, "¢∫ ¶ÆStage 2: Off The Wall");
                Sleep(600);
                for (int i = 0; i < 7; i++) {
                    gotoxycol(50, 13, 2, "¢∫ ¶ÆStage 2: Off The Wall");
                    Sleep(100);
                    gotoxycol(50, 13, 15, "¢∫ ¶ÆStage 2: Off The Wall");
                    Sleep(100);
                }
                break;
            }
            case 3: {
                gotoxycol(50, 15, 2, "¢∫ ¶ÆStage 3: ¿Ãπ´¡ˆ∞≥");
                Sleep(600);
                for (int i = 0; i < 7; i++) {
                    gotoxycol(50, 15, 2, "¢∫ ¶ÆStage 3: ¿Ãπ´¡ˆ∞≥");
                    Sleep(100);
                    gotoxycol(50, 15, 15, "¢∫ ¶ÆStage 3: ¿Ãπ´¡ˆ∞≥");
                    Sleep(100);
                }
                break;
            }
            case 4: {
                gotoxycol(50, 17, 2, "¢∫ ¶ÆStage 4: º¯∞£¿Ãπ´±‚µø");
                Sleep(600);
                for (int i = 0; i < 7; i++) {
                    gotoxycol(50, 17, 2, "¢∫ ¶ÆStage 4: º¯∞£¿Ãπ´±‚µø");
                    Sleep(100);
                    gotoxycol(50, 17, 15, "¢∫ ¶ÆStage 4: º¯∞£¿Ãπ´±‚µø");
                    Sleep(100);
                }
                break;
            }
            case 5: {
                gotoxycol(50, 19, 14, "¢∫ ¶ÆStage 5: π›¬¶¿Ãπ´±‚");
                Sleep(600);
                for (int i = 0; i < 7; i++) {
                    gotoxycol(50, 19, 2, "¢∫ ¶ÆStage 5: π›¬¶¿Ãπ´±‚");
                    Sleep(100);
                    gotoxycol(50, 19, 15, "¢∫ ¶ÆStage 5: π›¬¶¿Ãπ´±‚");
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
        if (y == 0) gotoxycol(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y, 14, "¢ ");
        else if (y == GBOARD_HEIGHT) gotoxycol(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y, 14, "¢ ");
        else gotoxycol(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y, 14, "¢ ");

        if (y == 0) gotoxycol(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y, 14, "¢ ");
        else if (y == GBOARD_HEIGHT) gotoxycol(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y, 14, "¢ ");
        else gotoxycol(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y, 14, "¢ ");
    }
    for (int x = 1; x < GBOARD_WIDTH + 1; x++) {
        gotoxycol(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y, 14, "¢ ");
        gotoxycol(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT, 14, "¢ ");
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
        if (y == 0) gotoxycol(UBOARD_ORIGIN_X, UBOARD_ORIGIN_Y + y, 14, "¶Æ");
        else if (y == UBOARD_HEIGHT) gotoxycol(UBOARD_ORIGIN_X, UBOARD_ORIGIN_Y + y, 14, "¶±");
        else gotoxycol(UBOARD_ORIGIN_X, UBOARD_ORIGIN_Y + y, 14, "¶≠");

        if (y == 0) gotoxycol(UBOARD_ORIGIN_X + (UBOARD_WIDTH + 1) * 2, UBOARD_ORIGIN_Y + y, 14, "¶Ø");
        else if (y == UBOARD_HEIGHT) gotoxycol(UBOARD_ORIGIN_X + (UBOARD_WIDTH + 1) * 2, UBOARD_ORIGIN_Y + y, 14, "¶∞");
        else gotoxycol(UBOARD_ORIGIN_X + (UBOARD_WIDTH + 1) * 2, UBOARD_ORIGIN_Y + y, 14, "¶≠");
    }
    for (int x = 1; x < UBOARD_WIDTH + 1; x++) {
        gotoxycol(UBOARD_ORIGIN_X + x * 2, UBOARD_ORIGIN_Y, 14, "¶¨");
        gotoxycol(UBOARD_ORIGIN_X + x * 2, UBOARD_ORIGIN_Y + UBOARD_HEIGHT, 14, "¶¨");
    }
    // print ult
    gotoxycol(15, 25, 15, "ULT:");
    gotoxycol(19, 25, 15, "°‡°‡°‡°‡°‡");
}

void drawFever() {
    // draw fever board
    for (int y = 0; y <= FBOARD_HEIGHT; y++) {
        if (y == 0) gotoxycol(FBOARD_ORIGIN_X, FBOARD_ORIGIN_Y + y, 14, "¶Æ");
        else if (y == FBOARD_HEIGHT) gotoxycol(FBOARD_ORIGIN_X, FBOARD_ORIGIN_Y + y, 14, "¶±");
        else gotoxycol(FBOARD_ORIGIN_X, FBOARD_ORIGIN_Y + y, 14, "¶≠");

        if (y == 0) gotoxycol(FBOARD_ORIGIN_X + (FBOARD_WIDTH + 1) * 2, FBOARD_ORIGIN_Y + y, 14, "¶Ø");
        else if (y == FBOARD_HEIGHT) gotoxycol(FBOARD_ORIGIN_X + (FBOARD_WIDTH + 1) * 2, FBOARD_ORIGIN_Y + y, 14, "¶∞");
        else gotoxycol(FBOARD_ORIGIN_X + (FBOARD_WIDTH + 1) * 2, FBOARD_ORIGIN_Y + y, 14, "¶≠");
    }
    for (int x = 1; x < FBOARD_WIDTH + 1; x++) {
        gotoxycol(FBOARD_ORIGIN_X + x * 2, FBOARD_ORIGIN_Y, 14, "¶¨");
        gotoxycol(FBOARD_ORIGIN_X + x * 2, FBOARD_ORIGIN_Y + FBOARD_HEIGHT, 14, "¶¨");
    }
    // print fever
    gotoxycol(56, 25, 15, "F  E  V  E  R");
}

void printStage() {
    switch (stage) {
    case 1: gotoxycol(72, 5, 15, "Stage 1: Classic"); break;
    case 2: gotoxycol(72, 5, 15, "Stage 2: Off The Wall"); break;
    case 3: gotoxycol(72, 5, 15, "Stage 3: ¿Ãπ´¡ˆ∞≥"); break;
    case 4: gotoxycol(72, 5, 15, "Stage 4: º¯∞£¿Ãπ´±‚µø"); break;
    case 5: gotoxycol(72, 5, 15, "Stage 5: π›¬¶¿Ãπ´±‚"); break;
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
    for (int i = 0; i < heart; i++) printf(" ¢æ");
    for (int i = heart; i < 3; i++) printf(" ¢Ω");
}

void drawControls() {
    // draw fever board
    for (int y = 0; y <= CBOARD_HEIGHT; y++) {
        if (y == 0) gotoxycol(CBOARD_ORIGIN_X, CBOARD_ORIGIN_Y + y, 14, "¶Æ");
        else if (y == CBOARD_HEIGHT) gotoxycol(CBOARD_ORIGIN_X, CBOARD_ORIGIN_Y + y, 14, "¶±");
        else gotoxycol(CBOARD_ORIGIN_X, CBOARD_ORIGIN_Y + y, 14, "¶≠");

        if (y == 0) gotoxycol(CBOARD_ORIGIN_X + (CBOARD_WIDTH + 1) * 2, CBOARD_ORIGIN_Y + y, 14, "¶Ø");
        else if (y == CBOARD_HEIGHT) gotoxycol(CBOARD_ORIGIN_X + (CBOARD_WIDTH + 1) * 2, CBOARD_ORIGIN_Y + y, 14, "¶∞");
        else gotoxycol(CBOARD_ORIGIN_X + (CBOARD_WIDTH + 1) * 2, CBOARD_ORIGIN_Y + y, 14, "¶≠");
    }
    for (int x = 1; x < CBOARD_WIDTH + 1; x++) {
        gotoxycol(CBOARD_ORIGIN_X + x * 2, CBOARD_ORIGIN_Y, 14, "¶¨");
        gotoxycol(CBOARD_ORIGIN_X + x * 2, CBOARD_ORIGIN_Y + CBOARD_HEIGHT, 14, "¶¨");
    }
}

void drawKeys() {
    gotoxycol(77, 14, 14, "CONTROLS");
    gotoxycol(80, 16, 7, "°Ë");
    gotoxycol(78, 17, 7, "°Á  °Ê");
    gotoxycol(80, 18, 7, "°È");
    gotoxycol(73, 20, 7, "[R] to use ULT");
    gotoxycol(73, 21, 7, "¢∫: [P] °´: [S]");
}

void gameOver() {
    system("cls");
    gotoxycol(38, 7, 12, "°·°·°·°·°·  °·°·°·°·°·  °·°·  °·°·  °·°·°·°·°·");
    gotoxycol(38, 8, 12, "°·          °·      °·  °·  °·  °·  °·");
    gotoxycol(38, 9, 12, "°·  °·°·°·  °·°·°·°·°·  °·  °·  °·  °·°·°·°·°·");
    gotoxycol(38, 10, 12, "°·      °·  °·      °·  °·      °·  °·");
    gotoxycol(38, 11, 12, "°·°·°·°·°·  °·      °·  °·      °·  °·°·°·°·°·");
    gotoxycol(38, 13, 12, "°·°·°·°·°·  °·      °·  °·°·°·°·°·  °·°·°·°·");
    gotoxycol(38, 14, 12, "°·      °·  °·      °·  °·          °·      °·");
    gotoxycol(38, 15, 12, "°·      °·  °·      °·  °·°·°·°·°·  °·°·°·°·");
    gotoxycol(38, 16, 12, "°·      °·    °·  °·    °·          °·      °·");
    gotoxycol(38, 17, 12, "°·°·°·°·°·      °·      °·°·°·°·°·  °·      °·");

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
    gotoxy(head->position.X, head->position.Y, "°›");
    gotoxy(body->position.X, body->position.Y, "°‹");
    gotoxy(tail->position.X, tail->position.Y, "°‹");
}

void addBody() {
    if (isFever()) return;

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
        gotoxy(tail->position.X, tail->position.Y, "°‹");

        int arrX = (tail->position.X - GBOARD_ORIGIN_X) / 2;
        int arrY = tail->position.Y - GBOARD_ORIGIN_Y;
        gameBoardInfo[arrY][arrX] = 2;
    }
}

void deleteBody() {
    if (length > minLength) { // √÷º“±Ê¿Ã∫∏¥Ÿ ≈¨ ∂ß∏∏ Ω««‡
        eraseTail();
        length--;
    }
}

void speedUp() {
    speed -= 5;
    if (speed < minSpeed) speed = minSpeed; // √÷¥Î º”µµ ∏Æπ‘
}

void speedDown() {
    speed += 5;
    if (speed > maxSpeed) speed = maxSpeed; // √÷º“ º”µµ ∏Æπ‘
}

void createItem() {
    if (isFever()) return;

    if (itemCount != 0) return; // æ∆¿Ã≈€ ª˝º∫ ¡∂∞«, »≠∏Èø° æ∆¿Ã≈€¿Ã ¿÷¿∏∏È ª˝º∫«œ¡ˆ æ ¿Ω
    time(&itemCreationTime);

    item->itemNo = rand() % 2 + 4; // æ∆¿Ã≈€ ¡æ∑˘ 2∞°¡ˆ
    item->pos.X = rand() % GBOARD_WIDTH + GBOARD_ORIGIN_X;
    item->pos.Y = rand() % GBOARD_HEIGHT + GBOARD_ORIGIN_Y;

    do {
        item->pos.X = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (item->pos.X % 2 == 1) item->pos.X++;
        item->pos.Y = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
    } while (detectCollision(item->pos.X, item->pos.Y)); // æ∆π´∞Õµµ æ¯¿∏∏È 0 π›»Øµ

    int arrX = (item->pos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (item->pos.Y - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = item->itemNo;

    switch (item->itemNo) {
    case 4:
        setTextColor(1);
        gotoxy(item->pos.X, item->pos.Y, "°„"); break;
    case 5:
        setTextColor(4);
        gotoxy(item->pos.X, item->pos.Y, "°Â"); break;
    default:
        gotoxy(INFO_X, ITEM_Y, "æ∆¿Ã≈€ ª˝º∫ø° Ω«∆–«‘");
        break;
    }

    itemCount++;

    setTextColor(15);
}

void deleteItem() { // ªË¡¶∏∏ √≥∏Æ or ∞˙¡§±Ó¡ˆ √≥∏Æ
    if (isFever()) return;

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
    if (heart < 3) // ª˝∏Ì ∞≥ºˆ ¡¶«—... ±◊∑∏∞‘ ∏π¿Ã ∫Œµ˙»˜¡ˆ æ ¿Ω. æ‡∞£ ¿ÁπÃ ø‰º“ down..
        heart++;
}

void getPet() {
    // »≠∏Èø° ±√ «•Ω√
    // press enter ±√ ªÁøÎ
}

void usePet() {
    if (petGauge < 5) return;

    pet = (Pet*)malloc(sizeof(Pet));
    time(&petCreationTime);

    // «ˆ¿Á ≤ø∏Æ¿« ¡¬«•, ¡¬«•∏¶ ∂ºæÓº≠ æ∆¿Ã≈€¿« ¡¬«•∑Œ ¿Ãµø(√÷¥‹∞≈∏Æ∑Œ)
    pet->pos.X = tail->position.X;
    pet->pos.Y = tail->position.Y;
    eraseTail();
    gotoxy(pet->pos.X, pet->pos.Y, "¢¡"); // ºˆ¡§ : »Æ¿ŒøÎ, ¿ßƒ° »Æ¿Œ « ø‰

    int arrX = (pet->pos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = pet->pos.Y - GBOARD_ORIGIN_Y;
    gameBoardInfo[arrY][arrX] = 13;
}

void petMove()
{
    if (isFever()) return;

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
        gotoxycol(pet->pos.X, pet->pos.Y, 2, "¢¡");
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
        for (int i = 0; i < 5; i++) printf("°‡");

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
    gotoxycol(pet->pos.X, pet->pos.Y, 2, "¢¡");
    setTextColor(15);

    arrX = (pet->pos.X - GBOARD_ORIGIN_X) / 2;
    arrY = pet->pos.Y - GBOARD_ORIGIN_Y;
    gameBoardInfo[arrY][arrX] = 13;
}

void getItemOfPet()
{
    itemCount--;
    if (item->itemNo == 4) {    //¡¡¿∫æ∆¿Ã≈€(±Ê¿Ã°È, º”µµ°È, FEVER, Ω«µÂ, ø©¿«¡÷ª˝º∫)
        //int itemType = (rand() % 5) + 1;//(rand() % 5) + 1; ø¯∑° ¿Ã∞«µ• ¡ˆ±›¿∫ æ∆¿Ã≈€ 2∞≥∏∏
        int itemType = 4;

        switch (itemType) {
        case 1:        //±Ê¿Ã°È
            deleteBody();
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "±Ê¿Ã ∞®º“!");
            break;
        case 2:        //º”µµ°È
            speedDown();
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "º”µµ ∞®º“!");
            break;
        case 3:
            getHeart();
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "ª˝∏Ì +1"); // ºˆ¡§ : ∏‡∆Æ, ¿Ã¿¸ ∏‡∆Æ∏¶ ¡ˆøˆæﬂ «“ ∞≈ ∞∞¿Ω
            break;
        case 4:
            dragonBallBomb();
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "ø©¿«¡÷ ∆¯≈∫"); // ºˆ¡§ : ∏‡∆Æ
            break;
        case 5:
            //getPet();
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "∆Í »πµÊ"); // ºˆ¡§ : ∏‡∆Æ
            break;
        default:
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "!!!!!!!");
            break;
        }

        createItem();
    }
    else if (item->itemNo == 5) {    //æ»¡¡¿∫æ∆¿Ã≈€ (±Ê¿Ã°Ë, º”µµ°Ë)
        int itemType = (rand() % 2) + 1;

        switch (itemType) {
        case 1:        //±Ê¿Ã°Ë
            addBody();
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "±Ê¿Ã ¡ı∞°!");
            break;
        case 2:        //º”µµ°Ë
            speedUp();
            gotoxy(INFO_X, ITEM_Y, "                  ");
            gotoxy(INFO_X, ITEM_Y, "º”µµ ¡ı∞°!");
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
    if (stage == 3) return;
    int multipleDragonBall = rand() % 3 + 2; // 2~4
    for (int i = 0; i < multipleDragonBall; i++) {
        addDragonBall();
    }
}

void moogiSwitch() {
    if (isFever()) return;

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

    gotoxy(head->position.X, head->position.Y, "°›");
    Moogi* tmp = head->back;
    while(tmp!=NULL)
    {
        gotoxy(tmp->position.X, tmp->position.Y, "°‹");
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

void inPlayKeyInput() // «√∑π¿Ã ¡ﬂ¿« ≈∞ ¿‘∑¬
// ¿Ãπ´±‚ ¡∂¿€ π◊ ¿œΩ√¡ﬂ¡ˆ
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

        Sleep(15); // «√∑π¿Ã º”µµ ¡∂¿˝
    }
}

void shiftUp() // «√∑π¿Ã ¡ﬂ up πÊ«‚≈∞ ¿‘∑¬ Ω√
{
    // √Êµπ »Æ¿Œ, ¿˚¿˝ƒ° ∏¯«— πÊ«‚ ¿¸»Ø¿Œ¡ˆ »Æ¿Œ
    if (direction == 0 || direction == 1) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;

    // πÊ«‚ æ˜µ•¿Ã∆Æ, ¿Ãµøø° µ˚∂Û head π◊ tail∏∏ redraw
    direction = 0;
    drawHead(nextPos);
    eraseTail();
}

void shiftDown() // «√∑π¿Ã ¡ﬂ down πÊ«‚≈∞ ¿‘∑¬ Ω√
{
    if (direction == 0 || direction == 1) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;

    direction = 1;
    drawHead(nextPos);
    eraseTail();
}

void shiftLeft() // «√∑π¿Ã ¡ﬂ left πÊ«‚≈∞ ¿‘∑¬ Ω√
{
    if (direction == 2 || direction == 3) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;
    
    direction = 2;
    drawHead(nextPos);
    eraseTail();
}

void shiftRight() // «√∑π¿Ã ¡ﬂ right πÊ«‚≈∞ ¿‘∑¬ Ω√
{
    if (direction == 2 || direction == 3) return;
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;

    direction = 3;
    drawHead(nextPos);
    eraseTail();
}

void pausePlay() // ¿œΩ√¡§¡ˆ -> ¿ÁΩ√¿€ »§¿∫ ∏Æº¬¿ª ±‚¥Ÿ∏≤
// »≠∏È¿Ã æÓ∂ª∞‘ ±∏º∫µ«¥¬¡ˆ ∏Ù∂Ûº≠
// ¿œ¥‹ æ∆∑°¬  πÊ«‚≈∞ ¥©∏£∏È »®»≠∏È¿∏∑Œ µπæ∆∞°±‚
// ¿ß¬  πÊ«‚≈∞ ¥©∏£∏È ±◊≥… ¿ÃæÓº≠ ∞‘¿” ¡¯«‡
// Ω∫∆‰¿ÃΩ∫ ¥©∏£∏È º±≈√ »Æ¡§¿∏∑Œ «ÿ≥˘¿Ω
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

int detectCollision(int posX, int posY) // √Êµπ ∞®¡ˆ
// ¿Ã «‘ºˆ¿« ¿‘∑¬ ¿Œ¿⁄∑Œ ¿Ãπ´±‚ ∏”∏Æ ¿ßƒ°∏¶ ≥÷æÓ¡÷∏È µ
// √Êµπ ∞®¡ˆ«œ∞Ì ΩÕ¿∏∏È directionø° µ˚∂Û «—ƒ≠ øÚ¡˜¿Œ ¿ßƒ°
// ±◊≥… nextHeadPos() ∞™¿ª ≥÷æÓ¡÷∏È µ
{
    int arrX = (posX - GBOARD_ORIGIN_X) / 2;
    int arrY = (posY - GBOARD_ORIGIN_Y);

    return gameBoardInfo[arrY][arrX];
}

COORD nextHeadPos() // head¿« ¥Ÿ¿Ω ¿ßƒ° π›»Ø
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

void drawHead(COORD headPos) // ¿Ãπ´±‚¿« ¿Ãµø¿ª √‚∑¬(head)
// ¿Ãπ´±‚ ø¨∞·∏ÆΩ∫∆Æ æ’ø° ≥ÎµÂ «œ≥™ √ﬂ∞°«œ∞Ì
// ∏”∏Æ∂˚ ±◊ πŸ∑Œ µ⁄ «— ƒ≠(¿Ã¿¸ ¿ßƒ°ø° √‚∑¬µ» ∏”∏Æ°›∏¶ ¡ˆøÏ±‚ ¿ß«ÿ)¿ª √‚∑¬
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
    
    gotoxy(head->position.X, head->position.Y, "°›");
    gotoxy(head->back->position.X, head->back->position.Y, "°‹");

    getSomething();

    int arrX = (headPos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (headPos.Y - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = 2;
}

void eraseTail() // ¿Ãπ´±‚¿« ¿Ãµø¿ª √‚∑¬(tail)
// ≤ø∏Æ∏¶ «— ƒ≠ æ’¿∏∑Œ ¿ÃµøΩ√≈∞∞Ì ∏∂¡ˆ∏∑ ≥ÎµÂ∏¶ ªË¡¶(drawHeadø°º≠ «œ≥™ √ﬂ∞°«œ¥œ±Ó length¥¬ ∂»∞∞¿Ω)
// ±◊∏Æ∞Ì ¡ˆ≥™∞£ ¿⁄∏Æø° ≥≤¿∫ √‚∑¬¿ª ∞¯πÈπÆ¿⁄∑Œ ¡ˆøÚ
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

Moogi* getNode(Moogi* front, Moogi* back, COORD position) // ¿Ãπ´±‚ ≥ÎµÂ «œ≥™ ª˝º∫
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
        if (heart > 1) { // ∏Òº˚ ≥≤æ“¿ª ∂ß
            waitToRecover();
            return 1;
        }
        else return 0;
    }
    gotoxy(0, 0, "                  ");

    if (detectCollision(nextPos.X, nextPos.Y) != 13)
    {
        drawHead(nextPos);      //æÍ∞° getSomething ∫Œ∏ß, getSomething¿Ã countScore∂˚ getItem ∫Œ∏ß,
        eraseTail();
    }
    
    if (stage == 2) wallMove();
    if (pet != NULL) petMove();

    if (isFever())
    {
        fillFever();
        time_t currentFeverTime;
        time(&currentFeverTime);

        if ((int)difftime(currentFeverTime, ferverStartTime) >= 7)
            removeFever();
    }

    return 1;
}

void getSomething()
{
    deleteItem();
    countScore();
    getItem();
    getFever();
}

void wallMove()
{
    if (isFever()) return;

    Wall* p = wallHead;

    while (p->next != NULL)
    {
        p = p->next;

        COORD pos = { p->position.X, p->position.Y };

        int cantMove = 4;

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
            cantMove--;

            if (!cantMove) break;
        }

        if (!cantMove) continue;

        int arrX = (p->position.X - GBOARD_ORIGIN_X) / 2;
        int arrY = (p->position.Y - GBOARD_ORIGIN_Y);
        gameBoardInfo[arrY][arrX] = 0;

        arrX = (pos.X - GBOARD_ORIGIN_X) / 2;
        arrY = (pos.Y - GBOARD_ORIGIN_Y);
        gameBoardInfo[arrY][arrX] = 1;

        setTextColor(14);
        gotoxy(p->position.X, p->position.Y, "  ");
        gotoxy(pos.X, pos.Y, "¢…");
        setTextColor(15);

        p->position.X = pos.X;
        p->position.Y = pos.Y;
    }
}

/*-------------------------------------------------------------------------------------------*/

void countScore() {
    if (detectCollision(head->position.X, head->position.Y) == 3) {
        if (dragonBallCount > 0) // ∞≥ºˆ∞° ¿Ωºˆ∞° µ«¡ˆ æ ∞‘
            dragonBallCount--;
        currentScore += 10;
        
        if (currentScore > 0 && currentScore > petScore)
        {
            petScore += 10;
            petGauge++; if (petGauge > 5) petGauge = 5;
            gotoxycol(19, 25, 2, "");
            for (int i = 0; i < petGauge; i++) printf("°·");
            setTextColor(15);
            for (int i = petGauge; i < 5; i++) printf("°‡");
        }

        if (currentScore > 0 && currentScore > feverScore)
        {
            feverScore += 20;
            createFever();
        }

        if (stage < 4) // Ω∫≈◊¿Ã¡ˆ 4, 5¥¬ Ω∫¿ßƒ° ±‚¥… off
            moogiSwitch();

        addBody();

        if (stage == 3)
        {
            changeMoogiColor();
            changeMoogiBodyColor();
        }

        if (dragonBallCount == 0) { // ø©¿«¡÷∞° ∏π¿∏∏È √ﬂ∞°∑Œ ª˝º∫«œ¡ˆ æ ¿Ω
            addDragonBall();
        }
        if (stage == 2) addWall();
    }
    else if (detectCollision(head->position.X, head->position.Y) == 12) {
        currentScore += 3;
    }
    gotoxy(79, 9, "");
    setTextColor(15);
    printf("%d", currentScore);
}

void getBestScore() {    //∞‘¿” Ω√¿€ Ω√ «√∑π¿Ã ¿¸ø° π›µÂΩ√ ∫“∑Øæﬂ «—¥Ÿ...(1π¯∏∏)
    FILE* fp_r = fopen("BestScore.txt", "r");
    if (fp_r == NULL) {
        return;
    }

    for (int i = 0; i < 5; i++) {
        fscanf(fp_r, "%d", &stageBestScore[i]);
    }
    fclose(fp_r);        //∆ƒ¿œø°º≠ √÷∞Ì¡°ºˆ æÚæÓø» °Ê ∆ƒ¿œ ∫ÒøÏ∞Ì stageBestScoreπËø≠ø° ¿˙¿Â
}

void setBestScore(int score) {            //∞‘¿”ø¿πˆµ∆¿ª∂ß∏∏ ∫Œ∏£∏È µ… µÌ?
    if (score > stageBestScore[stage - 1]) {
        stageBestScore[stage - 1] = score;
    }
}

void saveBestScore() {    //∞‘¿” ¡æ∑· Ω√ ¥ŸΩ√ √÷∞Ì¡°ºˆ∏¶ ∆ƒ¿œø° ¿˙¿Â«œ¥¬ ø™«“
    FILE* fp_w = fopen("BestScore.txt", "w");
    for (int i = 0; i < 5; i++) {
        fprintf(fp_w, "%d\n", stageBestScore[i]);
    }
    fclose(fp_w);
}

int isGameOver() {        //∏Æ≈œ∞™¿Ã 1¿Ã≥™ 2∏È ∞‘¿”¡æ∑·«ÿæﬂµ
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1 || detectCollision(nextPos.X, nextPos.Y) == 2) {
        if (heart > 1)
        {
            waitToRecover();
            return 0;
        }
        return 1;        //π⁄¿∏∏È 1∏Æ≈œ
    }
    return 0;            //æ∆¥œ∏È 0
}

void getItem() {
    if (isFever()) return;

    if (detectCollision(head->position.X, head->position.Y) == 4 || detectCollision(head->position.X, head->position.Y) == 5)
    {
        itemCount--;
        if (item->itemNo == 4) {    //¡¡¿∫æ∆¿Ã≈€(±Ê¿Ã°È, º”µµ°È, FEVER, Ω«µÂ, ø©¿«¡÷ª˝º∫)
            //int itemType = (rand() % 5) + 1;//(rand() % 5) + 1; ø¯∑° ¿Ã∞«µ• ¡ˆ±›¿∫ æ∆¿Ã≈€ 2∞≥∏∏
            int itemType = 4;

            switch (itemType) {
            case 1:        //±Ê¿Ã°È
                deleteBody();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "±Ê¿Ã ∞®º“!");
                break;
            case 2:        //º”µµ°È
                speedDown();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "º”µµ ∞®º“!");
                break;
            case 3:
                getHeart();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "ª˝∏Ì +1"); // ºˆ¡§ : ∏‡∆Æ, ¿Ã¿¸ ∏‡∆Æ∏¶ ¡ˆøˆæﬂ «“ ∞≈ ∞∞¿Ω
                break;
            case 4:
                dragonBallBomb();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "ø©¿«¡÷ ∆¯≈∫"); // ºˆ¡§ : ∏‡∆Æ
                break;
            case 5:
                getPet();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "∆Í »πµÊ"); // ºˆ¡§ : ∏‡∆Æ
                break;
            default:
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "!!!!!!!");
                break;
            }

            createItem();
        }
        else if (item->itemNo == 5) {    //æ»¡¡¿∫æ∆¿Ã≈€ (±Ê¿Ã°Ë, º”µµ°Ë)
            int itemType = (rand() % 2) + 1;

            switch (itemType) {
            case 1:        //±Ê¿Ã°Ë
                addBody();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "±Ê¿Ã ¡ı∞°!");
                break;
            case 2:        //º”µµ°Ë
                speedUp();
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "º”µµ ¡ı∞°!");
                break;
            default:
                gotoxy(INFO_X, ITEM_Y, "                  ");
                gotoxy(INFO_X, ITEM_Y, "!!!!!!!");
                break;
            }

            createItem();
        }
    }
    //0 æ∆π´∞Õµµæ¯¥¬∞≈, 1 ∫Æ¿Âæ÷π∞, 2 ¿Ãπ´±‚∏ˆ, 3 ø©¿«¡÷, 4 ¡¡¿∫æ∆¿Ã≈€, 5 ≥™ª€æ∆¿Ã≈€,
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
    gotoxy(x, y, "°€");
    setTextColor(15);
}


// »£√‚ : ±∏ΩΩ¿ª ∏‘¿∏∏È Ω««‡
// ¿ÃπÃ ∫Æ ∂«¥¬ æ∆¿Ã≈€¿Ã √‚∑¬µ» ∞˜ø° √‚∑¬«œ∏È X
void addWall() {
    if (dragonBallCount > 1) return; // ∫Æ ª˝º∫ ¡∂∞«(±∏ΩΩ ∞≥ºˆ)

    int wallX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X;
    int wallY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;

    do {
        wallX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (wallX % 2 == 1) wallX++;
        wallY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
    } while (detectCollision(wallX, wallY)); // æ∆π´∞Õµµ æ¯¿∏∏È 0 π›»Øµ

    setTextColor(14);
    gotoxy(wallX, wallY, "¢…");
    setTextColor(15);

    int arrX = (wallX - GBOARD_ORIGIN_X) / 2;
    int arrY = (wallY - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = 1; // ∫Æ 1

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

void changeMoogiColor() {      //¿Ãπ´±‚ ªˆ±Ú ºº∆√, 0ª° 1≥Î 2√  3∆ƒ 4»Ú
    moogiColor = rand() % 5;
}
void changeMoogiBodyColor() {   //¿Ãπ´±‚ ∏ˆ ªˆ±Ú πŸ≤ﬁ(countScoreø°º≠ ∫“∏≤)
    setTextColor_rygbw(moogiColor);
    gotoxy(head->position.X, head->position.Y, "°›");
    Moogi* p = head->back;
    while (p->back != NULL) {
        gotoxy(p->position.X, p->position.Y, "°‹");
        p = p->back;
    }
}

void setDragonBallColor() {     //«— π¯∏∏ »£√‚, √ ±‚»≠øÎµµ
    for (int i = 0; i < 5; i++)
        dragonBallColor[i] = i;     //0ª° 1≥Î 2√  3∆ƒ 4»Ú
}

void setDragonBallPos() {       //«— π¯∏∏ »£√‚, √ ±‚»≠øÎµµ.
    for (int i = 0; i < 5; i++) {
        dragonBallPos[i].X = 0;
        dragonBallPos[i].Y = 0;
    }
}

void showColorDragonBall(int x, int y, int color) {
    setTextColor_rygbw(color);  //0ª° 1≥Î 2√  3∆ƒ 4»Ú
    gotoxy(x, y, "°€");
    setTextColor(15);
}

void deleteDragonBall() {       //ø©¿«¡÷ ∏‘¿∫ »ƒ ∞∞¿∫ ªˆ æ∆¥— ø©¿«¡÷ ¡ˆøÏ¥¬ øÎµµ(addDragonBallø°º≠ ∫“∏≤)
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

void waitToRecover() { // ø©±‚º≠ ≈∞∏¶ ªı∑Œ ¿‘∑¬ πﬁ¿ª¡ˆ, stage∑Œ ∫∏≥ªº≠(moogimove return 1) ¿‘∑¬πﬁ¿ª¡ˆ
    int key, flag = 0;
    gotoxycol(INFO_X, ITEM_Y + 1, 4, "PRESS TO CONTINUE");
    Moogi* p = head->back;
    COORD nextPos;

    for (int i = 0; i < 5; i++) {
        for (int i = 0; ; i++) {
            if (stage == 3) {
                setTextColor_rygbw(moogiColor);
                gotoxy(p->position.X, p->position.Y, "°€");
            }
            else
                gotoxycol(p->position.X, p->position.Y, 15, "°€");
            if (p->back == NULL) break;
            p = p->back;
        }
        Sleep(300);
        p = head->back;
        for (int i = 0; ; i++) {
            if (stage == 3) {
                setTextColor_rygbw(moogiColor);
                gotoxy(p->position.X, p->position.Y, "°‹");
            }
            else
                gotoxycol(p->position.X, p->position.Y, 15, "°‹");
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

            Sleep(15); // «√∑π¿Ã º”µµ ¡∂¿˝
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

void createFever() // FEVER æ∆¿Ã≈€ ª˝º∫
{
    if (isFever()) return;

    int ch;
    for (int i = 0; i < 5; i++)
    {
        if (fever[i] == 2) return;
        if (fever[i] == 0)
        {
            ch = i;
            break;
        }
    }

    int feverX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X;
    int feverY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;

    do {
        feverX = (rand() % GBOARD_WIDTH) + GBOARD_ORIGIN_X; if (feverX % 2 == 1) feverX++;
        feverY = (rand() % GBOARD_HEIGHT) + GBOARD_ORIGIN_Y;
    } while (detectCollision(feverX, feverY));

    int arrX = (feverX - GBOARD_ORIGIN_X) / 2;
    int arrY = feverY - GBOARD_ORIGIN_Y;

    switch (ch)
    {
    case 0:
        gotoxycol(feverX, feverY, 10, "F");
        setTextColor(15);
        gameBoardInfo[arrY][arrX] = 7;
        fever[0] = 2;
        break;
    case 1:
        gotoxycol(feverX, feverY, 11, "E");
        setTextColor(15);
        gameBoardInfo[arrY][arrX] = 8;
        fever[1] = 2;
        break;
    case 2:
        gotoxycol(feverX, feverY, 12, "V");
        setTextColor(15);
        gameBoardInfo[arrY][arrX] = 9;
        fever[2] = 2;
        break;
    case 3:
        gotoxycol(feverX, feverY, 13, "E");
        setTextColor(15);
        gameBoardInfo[arrY][arrX] = 10;
        fever[3] = 2;
        break;
    case 4:
        gotoxycol(feverX, feverY, 14, "R");
        setTextColor(15);
        gameBoardInfo[arrY][arrX] = 11;
        fever[4] = 2;
        break;
    }
}

void getFever() // »πµÊ«œ∏È FEVER πËø≠ √ ±‚»≠, GBOARD æ∆∑° FEVER «ˆ»≤ æ˜µ•¿Ã∆Æ
{
    int getIdx = detectCollision(head->position.X, head->position.Y);

    switch (getIdx)
    {
    case 7:
        fever[0] = 1;
        gotoxycol(56, 25, 10, "F");
        setTextColor(15);
        break;
    case 8:
        fever[1] = 1;
        gotoxycol(59, 25, 11, "E");
        setTextColor(15);
        break;
    case 9:
        fever[2] = 1;
        gotoxycol(62, 25, 12, "V");
        setTextColor(15);
        break;
    case 10:
        fever[3] = 1;
        gotoxycol(65, 25, 13, "E");
        setTextColor(15);
        break;
    case 11:
        fever[4] = 1;
        gotoxycol(68, 25, 14, "R");
        setTextColor(15);
        feverStart = 1;
        break;
    default:
        break;
    }
}

int isFever()
{
    if (fever[4] == 1) return 1;
    else return 0;
}

void fillFever()
{
    if (!feverStart) return;

    feverStart = 0;
    time(&ferverStartTime);

    if (stage == 2)
    {
        Wall* p = wallHead, * pi = p;
        while (p != NULL)
        {
            p = p->next;
            free(pi);
            pi = p;
        }
        wallHead = (Wall*)malloc(sizeof(Wall));
        wallHead->next = NULL;
    }

    for (int y = GBOARD_ORIGIN_Y + 1; y < GBOARD_ORIGIN_Y + GBOARD_HEIGHT; y++)
    {
        for (int x = GBOARD_ORIGIN_X + 2; x < GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 2; x+=2)
        {
            if (x == head->position.X && y == head->position.Y) continue;
            if (detectCollision(x, y) != 2)
            {
                gotoxycol(x, y, 12, "°€");
                
                int arrX = (x - GBOARD_ORIGIN_X) / 2;
                int arrY = y - GBOARD_ORIGIN_Y;
                gameBoardInfo[arrY][arrX] = 12;
            }
        }
    }

    setTextColor(15);
}

void removeFever()
{
    for (int y = GBOARD_ORIGIN_Y + 1; y < GBOARD_ORIGIN_Y + GBOARD_HEIGHT; y++)
    {
        for (int x = GBOARD_ORIGIN_X + 2; x < GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 2; x+=2)
        {
            if (x == head->position.X && y == head->position.Y) continue;
            if (detectCollision(x, y) != 2)
            {
                gotoxycol(x, y, 12, "  ");

                int arrX = (x - GBOARD_ORIGIN_X) / 2;
                int arrY = y - GBOARD_ORIGIN_Y;
                gameBoardInfo[arrY][arrX] = 0;
            }
        }
    }

    for (int i = 0; i < 5; i++) fever[i] = 0;
    gotoxycol(56, 25, 15, "F  E  V  E  R");

    feverStart = 0;

    addDragonBall();
    createItem();
}
