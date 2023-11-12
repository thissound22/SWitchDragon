#ifndef __CURSOR_H__
#define __CURSOR_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>

#include "Constants.h"
#include "Moogi.h"
#include "Globals.h"
#include "Cursor.h"
#include "DrawMap.h"
#include "UserControl.h"
#include "EtcFunction.h"

// change console size
void console();

void setCurrentCursorPos(int x, int y);
COORD getCurrentCursorPos();
void removeCursor();

#endif
