#ifndef _GAME_UTILS_H_
#define _GAME_UTILS_H_
#include "_define.h"


#define DONKEY_WIDTH          (CELL_WIDTH * 1.5)
#define DONKEY_HEIGHT         (CELL_HEIGHT * 1.5)

#define DONKEY_OFSET_X         (CELL_HEIGHT * 0.2)
#define DONKEY_OFSET_Y         (CELL_HEIGHT * 0.5)

enum E_ObjectType{
    e_OT_Obstacle,
    e_OT_Ship,
    e_OT_Rope
};

enum PlayerStatus{
    e_PS_Normal,
    e_PS_Jump,
    e_PS_OnVessel
};


typedef struct _PlayerInfo{
    Object object;

    int nStatus;// normal, jump
    int nMoveDelay;

    long nRemainTime;
    int nLives;
    int nScore;

    int nBoostKind;
    int nBoostTime;
    int nBoostStart;

    int bReborn;
    unsigned long nRebornTick;
    int nBlinkCnt;

} PlayerInfo;

typedef struct _Obstacle {
    Object object;
    int nDirect;
    int nRange;
    int nSpeed;
    POINT ptOrgPos;
} Obstacle;



void clearScreen();
void UpdateDeltaTime();
float deltaSecond();

POINT CellToPos(POINT ptCell);
POINT PosToCell(POINT ptPos);
POINT GetRandomRoadCell(LevelInfo * pLevel);

void UpdateObjectPos(Object *pObject);
void UpdateObjectCell(Object *pObject);
void DrawObject(Object *pObject);
void DrawTempObject(Object *pObject);
void EraseObject(Object *pObject, Image* imgBackground, int bPrev);
void DrawBackground(Image* image, int x, int y, int width, int height);
#endif