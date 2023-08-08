#include "_gameutils.h"

#include "fb.h"

#include "_define.h"
#include "_delays.h"
#include "_debug.h"
#include "_utils.h"
#include "_rand.h"

static float s_fCycleDelta = 0;

// simply put, this function clears the screen 
void clearScreen()
{ 
    for (int i = 0; i < 1280; i++){
        for (int j=0; j<720; j++){
            drawPixel(i, j, 0);
        }
    }
}

void UpdateDeltaTime()
{
    static unsigned long prevTick = 0;
    unsigned long curTick = getTickCount();
    if(!prevTick) prevTick = curTick;
    unsigned long delta = curTick - prevTick;
    s_fCycleDelta = 1.f * delta / 1000 / 1000;
    prevTick = curTick;
}

float deltaSecond()
{
    return s_fCycleDelta;
} 


POINT CellToPos(POINT ptCell)
{
    POINT ptPos;
    ptPos.x = ptCell.x * CELL_WIDTH + CELL_WIDTH / 2;
    ptPos.y = ptCell.y * CELL_HEIGHT + CELL_HEIGHT / 2;
    return ptPos;
}

POINT PosToCell(POINT ptPos)
{
    POINT ptCell;
    ptCell.x = (ptPos.x - CELL_WIDTH / 2) / CELL_WIDTH;
    ptCell.y = (ptPos.y - CELL_HEIGHT / 2) / CELL_HEIGHT;
    return ptCell;
}

POINT GetRandomRoadCell(LevelInfo * pLevel)
{
    POINT pt;
    pt.x = 0;
    pt.y = 0;
    int nRand = rand(0, pLevel->nRoadCellCnt);
    int nCnt = 0;
    for(int i = 0 ; i < MAX_CELL_WIDTH ; i++){
        for(int j = 0 ;  j < MAX_CELL_HEIGHT ; j++){
            if(pLevel->aryCellMap[i][j] > 0)
            {
                 nCnt++;
                 if(nCnt >= nRand)
                {
                    pt.x = i;
                    pt.y = j;
                    return pt;
                }
            }
        }
    }
    return pt;
}

void UpdateObjectPos(Object *pObject)
{
    pObject->ptPos = CellToPos(pObject->ptCell);
}

void UpdateObjectCell(Object *pObject)
{
    pObject->ptCell = PosToCell(pObject->ptPos);
}

void DrawObject(Object *pObject)
{
    drawScaleImage(&pObject->image, pObject->ptPos, pObject->nWidth, pObject->nHeight, 1);
}

void DrawTempObject(Object *pObject)
{
    int x = pObject->ptPos.x;
    int y = pObject->ptPos.y;
    drawCircle(x + pObject->nWidth / 2, y + pObject->nHeight / 2, pObject->nWidth / 2 - 2, 0xB0, 1);
}


void EraseObject(Object *pObject, Image* imgBackground, int bPrev)
{
    if(bPrev)
    {
        DrawBackground(imgBackground, pObject->ptPrevPos.x, pObject->ptPrevPos.y, pObject->nWidth, pObject->nHeight);    
    }
    else
    {
        DrawBackground(imgBackground, pObject->ptPos.x, pObject->ptPos.y, pObject->nWidth, pObject->nHeight);    
    }  
    
}

void DrawBackground(Image* image, int x, int y, int width, int height)
{
    drawImageEx(image->pixel_data, image->width, image->height,
                x, y, width, height,
                x, y, 0);
}