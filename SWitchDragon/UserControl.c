#pragma warning(disable: 4996)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>

#include "UserControl.h"

int inPlayKeyInput() // 플레이 중의 키 입력
// 이무기 조작 및 일시중지
{
    int i, key, isDone = 0;

    for (i = 0; i < 20; i++)
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
                isDone = pausePlay();
                break;
            default:
                break;
            }
        }

        Sleep(speed); // 플레이 속도 조절
    }

    return isDone;
}

void shiftUp() // 플레이 중 up 방향키 입력 시
{
    // 충돌 확인, 적절치 못한 방향 전환인지 확인
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;
    if (direction == 0 || direction == 1) return;

    // 방향 업데이트, 이동에 따라 head 및 tail만 redraw
    direction = 0;
    drawHead(nextPos);
    eraseTail();
}

void shiftDown() // 플레이 중 down 방향키 입력 시
{
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;
    if (direction == 0 || direction == 1) return;

    direction = 1;
    drawHead(nextPos);
    eraseTail();
}

void shiftLeft() // 플레이 중 left 방향키 입력 시
{
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;
    if (direction == 2 || direction == 3) return;

    direction = 2;
    drawHead(nextPos);
    eraseTail();
}

void shiftRight() // 플레이 중 right 방향키 입력 시
{
    COORD nextPos = nextHeadPos();
    if (detectCollision(nextPos.X, nextPos.Y) == 1)
        return;
    if (direction == 2 || direction == 3) return;

    direction = 3;
    drawHead(nextPos);
    eraseTail();
}

int pausePlay() // 일시정지 -> 재시작 혹은 리셋을 기다림
// 화면이 어떻게 구성되는지 몰라서
// 일단 아래쪽 방향키 누르면 홈화면으로 돌아가기
// 위쪽 방향키 누르면 그냥 이어서 게임 진행
// 스페이스 누르면 선택 확정으로 해놨음
{
    int isDone = 0;

    while (1)
    {
        if (_kbhit() != 0)
        {
            int key = _getch();

            if (key == SPACE) break;
            else if (key == DOWN) isDone = 1;
            else if (key == UP) isDone = 0;
        }
        Sleep(speed);
    }

    return isDone;
}

void goToStartScreen()
{
    // 필요한 맵 그리는 함수 호출

    // 전역 변수 초기화
    direction = 0;
    stage = -1;
    speed = 20;
    shield = 0;
    for (int i = 0; i < 5; i++) fever[i] = 0;

    // 아래는 head, tail 초기화
    Moogi* p = head, * pi = p;
    while (p != NULL)
    {
        pi = p;
        p = p->right;
        free(pi);
    }
    COORD pos = { 20,20 };
    head = getNode(NULL, NULL, pos);
    pos.Y++;
    tail = getNode(head, NULL, pos);
    head->right = tail;
}

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
        curPos.X--;
        break;
    case 3:
        curPos.X++;
        break;
    default:
        break;
    }

    return curPos;
}

void drawHead(COORD headPos) // 이무기의 이동을 출력(head)
// 이무기 연결리스트 앞에 노드 하나 추가하고
// 머리랑 그 바로 뒤 한 칸(이전 위치에 출력된 머리◐를 지우기 위해)을 출력
{
    Moogi* node = getNode(NULL, head, headPos);
    head = node;

    int arrX = (headPos.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (headPos.Y - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = 2;

    SetCurrentCursorPos(head->position.X, head->position.Y);
    printf("◐");
    SetCurrentCursorPos(head->right->position.X, head->right->position.Y);
    printf("●");
}

void eraseTail() // 이무기의 이동을 출력(tail)
// 꼬리를 한 칸 앞으로 이동시키고 마지막 노드를 삭제(drawHead에서 하나 추가하니까 length는 똑같음)
// 그리고 지나간 자리에 남은 출력을 공백문자로 지움
{
    int arrX = (tail->position.X - GBOARD_ORIGIN_X) / 2;
    int arrY = (tail->position.Y - GBOARD_ORIGIN_Y);
    gameBoardInfo[arrY][arrX] = 0;

    SetCurrentCursorPos(tail->position.X, tail->position.Y);
    printf("  ");

    Moogi* tmp = tail;
    tail = tail->left;
    tail->right = NULL;
    free(tmp);
}

Moogi* getNode(Moogi* left, Moogi* right, COORD position) // 이무기 노드 하나 생성
{
    Moogi* node = (Moogi*)malloc(sizeof(Moogi));
    node->left = left;
    node->right = right;
    node->position = position;

    return node;
}
