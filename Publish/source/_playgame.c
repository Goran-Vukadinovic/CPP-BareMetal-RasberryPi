#include "_playgame.h"
#include "fb.h"
#include "_rand.h"
#include "_define.h"
#include "_utils.h"
#include "_gpio.h"
#include "_delays.h"
#include "_debug.h"
#include "_sprintf.h"
#include "_gameutils.h"



extern struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[1280 * 720 * 4 + 1];
} levelOneBackground;

extern struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[150 * 150 * 4 + 1];
} donkeykong;

static LevelInfo s_aryLevelInfo[LEVEL_COUNT];

static int s_nPlayGameStatus = 0;

static PlayerInfo s_player;
static int s_nCurLevel = 0;
static unsigned long s_nKeyBlockedTick = 0;
static Obstacle s_aryObstacles[MAX_OBSTACLE_CNT];
static int s_nObstacleCnt = 0;

static int s_nCurGameMenuItem = 0;

static int s_TmpRoadMask[22][40] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0}
};


void InitResource()
{
    LevelInfo* pLevel = &s_aryLevelInfo[0];
    pLevel->background.pixel_data = levelOneBackground.pixel_data;;
    pLevel->background.width = levelOneBackground.width;
    pLevel->background.height = levelOneBackground.height;
    pLevel->nLimitSecond = 30;
    pLevel->nLives = 4;
    pLevel->nObstacles = 20;
    POINT ptScreen = {SCREEN_WIDTH, SCREEN_HEIGHT};
    POINT cell = PosToCell(ptScreen);
    pLevel->nCellWidth = cell.x + 1;
    pLevel->nCellHeight = cell.y + 1;
    for(int i = 0 ; i < MAX_CELL_WIDTH ; i++){
        for(int j = 0 ;  j < MAX_CELL_HEIGHT ; j++){
            pLevel->aryCellMap[i][j] = 0;
        }
    }
    pLevel->nRoadCellCnt = 0;
    for(int i = 0 ; i < 40 ; i++){
        for(int j = 0 ;  j < 22 ; j++){
            pLevel->aryCellMap[i][j] = s_TmpRoadMask[j][i];
            if(pLevel->aryCellMap[i][j]) pLevel->nRoadCellCnt++;
        }
    }

    pLevel->ptStartCell.x = 0;
    pLevel->ptStartCell.y = 9;
    pLevel->ptTargetCell.x = 38;
    pLevel->ptTargetCell.y = 11;
}

LevelInfo* GetCurLevelInfo()
{
    return &s_aryLevelInfo[s_nCurLevel];
}

void GenerateObstacles(int nCnt)
{
    LevelInfo* pLevel = GetCurLevelInfo();
    for(int i = 0 ; i < nCnt ; i++)
    {
        Obstacle* pObstacle = &s_aryObstacles[i];
        pObstacle->object.image.pixel_data = 0;
        pObstacle->object.image.width = CELL_WIDTH;
        pObstacle->object.image.height = CELL_HEIGHT;
        POINT cell = GetRandomRoadCell(pLevel);
        pObstacle->object.ptPos = CellToPos(cell);
        pObstacle->object.ptCell = cell;
        pObstacle->object.ptPrevPos = pObstacle->object.ptPos;
        pObstacle->object.bRedraw = FALSE;

        pObstacle->object.nWidth = CELL_WIDTH;
        pObstacle->object.nHeight = CELL_HEIGHT;

        pObstacle->nSpeed = rand(3, 10) * CELL_WIDTH;
        pObstacle->nDirect = rand(0, 3);
        pObstacle->nRange = rand(2* CELL_WIDTH, 3 * CELL_WIDTH);
        pObstacle->ptOrgPos = pObstacle->object.ptPos;
    }
    s_nObstacleCnt = nCnt;
}


int FindObjectByPos(POINT pos, Obstacle** pResult)
{
    int nCnt = 0;
    for(int i = 0 ; i < s_nObstacleCnt ; i++)
    {
        int dx = pos.x - s_aryObstacles[i].object.ptPos.x;
        int dy = pos.y - s_aryObstacles[i].object.ptPos.y;
        int len = dx * dx + dy * dy;
        if(len < CELL_WIDTH * CELL_HEIGHT / 4)
        {
            pResult[nCnt] = &s_aryObstacles[i];
            nCnt++;
        }
    }
    return nCnt;
}

void InitGame(int nLevel, BOOL bRestart)
{
    if(!bRestart) s_nPlayGameStatus = e_PG_Start;
    else s_nPlayGameStatus = e_PG_ReStart;

    s_nCurLevel = nLevel;
    LevelInfo* pCurLevel = GetCurLevelInfo();
    s_player.object.image.pixel_data = donkeykong.pixel_data;
    s_player.object.image.width = donkeykong.width;
    s_player.object.image.height = donkeykong.height;
    s_player.object.ptCell = pCurLevel->ptStartCell;
    s_player.object.ptPos = CellToPos(s_player.object.ptCell);
    s_player.object.ptPrevPos = s_player.object.ptPos;
    s_player.object.bRedraw = FALSE;
    s_player.object.nWidth = DONKEY_WIDTH;
    s_player.object.nHeight = DONKEY_HEIGHT;

    s_player.nStatus = e_PS_Normal;
    s_player.nMoveDelay = 0;

    s_player.nRemainTime = pCurLevel->nLimitSecond * 1000000;
    s_player.nScore = 0;
    s_player.nLives = pCurLevel->nLives;

    s_player.nBoostKind = 0;
    s_player.nBoostStart = 0;
    s_player.nBoostTime = 0;

    s_player.bReborn = TRUE;
    s_player.nRebornTick = 0;
    s_player.nBlinkCnt = 0;

    GenerateObstacles(pCurLevel->nObstacles);
}

void PrepareGamePlay()
{    
    InitGame(0, FALSE);
}

void ErasePlayer(int bPrev)
{
    LevelInfo* pLevelInfo = GetCurLevelInfo();
    int x = s_player.object.ptPos.x - DONKEY_OFSET_X;
    int y = s_player.object.ptPos.y - DONKEY_OFSET_Y;
    if(bPrev)
    {
        x = s_player.object.ptPrevPos.x - DONKEY_OFSET_X;
        y = s_player.object.ptPrevPos.y - DONKEY_OFSET_Y;
    }
    DrawBackground(&pLevelInfo->background, x, y, DONKEY_WIDTH, DONKEY_HEIGHT);
}

void DrawPlayer(BOOL bForce)
{
    unsigned long curTick = getTickCount();
    POINT ptPos;
    ptPos.x = s_player.object.ptPos.x - DONKEY_OFSET_X;
    ptPos.y = s_player.object.ptPos.y - DONKEY_OFSET_Y;

    BOOL bDraw = TRUE;
    if(s_player.bReborn)
    {
        if(!s_player.nRebornTick) s_player.nRebornTick = curTick;
        if(s_player.nBlinkCnt % 3 == 1) bDraw = FALSE;
        s_player.nBlinkCnt++;
        if(s_player.nBlinkCnt > 3) s_player.nBlinkCnt = 0;
        if(curTick - s_player.nRebornTick > 2 * 1000 * 1000)
        {
            s_player.bReborn = FALSE;
            s_player.nBlinkCnt = 0;
            s_player.nRebornTick = 0;
            bDraw = TRUE;
            _print("reborn finish\n");
        }
        ErasePlayer(TRUE);
        if(bDraw) drawScaleImage(&s_player.object.image, ptPos, DONKEY_WIDTH, DONKEY_HEIGHT, 1);
    }
    else
    {
        if(bForce || s_player.object.bRedraw)
        {
            ErasePlayer(TRUE);
            drawScaleImage(&s_player.object.image, ptPos, DONKEY_WIDTH, DONKEY_HEIGHT, 1);
        }
        else
        {
            drawScaleImage(&s_player.object.image, ptPos, DONKEY_WIDTH, DONKEY_HEIGHT, 1);
        }
        
    }
    s_player.object.bRedraw = FALSE;
    s_player.object.ptPrevPos = s_player.object.ptPos;
}

void DrawAllObstacles(int bForce)
{
    LevelInfo* pLevel = GetCurLevelInfo();
    for(int i = 0 ; i < s_nObstacleCnt ; i++)
    {
        Obstacle* pObstacle = &s_aryObstacles[i];
        if(bForce || pObstacle->object.bRedraw){
            EraseObject(&pObstacle->object, &pLevel->background, 1);
            DrawTempObject(&pObstacle->object);
        }
        else
        {
            DrawTempObject(&pObstacle->object);
        }
        
        pObstacle->object.bRedraw = FALSE;
    }
}
void DrawTargetPos()
{
    LevelInfo* pCurLevel = GetCurLevelInfo();
    POINT ptTarget = CellToPos(pCurLevel->ptTargetCell);
    drawRect(ptTarget.x, ptTarget.y, ptTarget.x + CELL_WIDTH, ptTarget.y + CELL_HEIGHT, 0xC1, 1);
}
void DrawBackground_PG()
{
    LevelInfo* pCurLevel = GetCurLevelInfo();
    POINT ptZero = {0, 0};
    drawImage(&pCurLevel->background, ptZero, 0);    
    
    /*
    for(int i = 0 ; i < pCurLevel->nCellWidth ; i++)
    {
        int x = CellToCoordX(i);
        drawLine2(x, 0, x, SCREEN_HEIGHT, 0);
    }
    for(int i = 0 ; i < pCurLevel->nCellHeight ; i++)
    {
        int y = CellToCoordY(i);
        drawLine2(0, y, SCREEN_WIDTH, y, 0);
    }
    */
}

BOOL IsBlackCell(int x, int y)
{
    LevelInfo* pCurLevel = GetCurLevelInfo();
    return !pCurLevel->aryCellMap[x][y];
}

void DrawPlayerInfo()
{
    unsigned long s_Tick = 0;
    unsigned long curTick = getTickCount();
    LevelInfo* pCurLevel = GetCurLevelInfo();

    if(!s_Tick) s_Tick = curTick;
    if(s_Tick == curTick || curTick - s_Tick > 1000000){        
        s_Tick = curTick;
        DrawBackground(&pCurLevel->background, 1000, 50, 150, 20);

        char szBuf[128];
        sprintf(szBuf, "Score:%d", s_player.nScore);
        drawScaleString(1000, 50, szBuf, 0x1, 2);

        DrawBackground(&pCurLevel->background, 1000, 75, 150, 20);
        sprintf(szBuf, "Lives:%d", s_player.nLives);
        drawScaleString(1000, 75, szBuf, 0x1, 2);

        DrawBackground(&pCurLevel->background, 1000, 100, 200, 20);
        int nRemain = s_player.nRemainTime / 1000000;
        if(nRemain < 0) nRemain = 0;
        sprintf(szBuf, "Left time:%d", nRemain);
        drawScaleString(1000, 100, szBuf, 0x1, 2);    
    }
}

void UpdateAllObstacles()
{
    //LevelInfo* pLevelInfo = GetCurLevelInfo();
    for(int i = 0 ; i < s_nObstacleCnt ; i++)
    {
        Obstacle* pObstacle = &s_aryObstacles[i];
        switch (pObstacle->nDirect)
        {
        case 0:
            break;
        case 1:
            //EraseObject(&pObstacle->object, &pLevelInfo->background);
            pObstacle->object.ptPrevPos = pObstacle->object.ptPos;
            if(abs(pObstacle->object.ptPos.x - pObstacle->ptOrgPos.x) > pObstacle->nRange)
            {
                pObstacle->object.ptPos.x = pObstacle->ptOrgPos.x + pObstacle->nRange * pObstacle->nSpeed / abs(pObstacle->nSpeed);
                pObstacle->nSpeed = -pObstacle->nSpeed;
            }
            else{
                pObstacle->object.ptPos.x += pObstacle->nSpeed * deltaSecond();
            }
            pObstacle->object.bRedraw = TRUE;
            break;
        case 2:
            //EraseObject(&pObstacle->object, &pLevelInfo->background);
            pObstacle->object.ptPrevPos = pObstacle->object.ptPos;
            if(abs(pObstacle->object.ptPos.y - pObstacle->ptOrgPos.y) > pObstacle->nRange){
                pObstacle->object.ptPos.y = pObstacle->ptOrgPos.y + pObstacle->nRange * pObstacle->nSpeed / abs(pObstacle->nSpeed);
                pObstacle->nSpeed = -pObstacle->nSpeed;                
            }
            else{
                pObstacle->object.ptPos.y += pObstacle->nSpeed * deltaSecond();            
            }
            pObstacle->object.bRedraw = TRUE;
            break;
        default:
            break;
        }
        UpdateObjectCell(&pObstacle->object);
    }
}

void UpdatePlayer()
{    
    LevelInfo* pCurLevel = GetCurLevelInfo();
    POINT ptCell = s_player.object.ptCell;
    if(ptCell.x == pCurLevel->ptTargetCell.x && ptCell.y == pCurLevel->ptTargetCell.y)
    {
        s_nPlayGameStatus = e_PG_ShowWin;
        return;
    }
    if(s_player.nRemainTime <= 0)
    {
        s_nPlayGameStatus = e_PG_ShowFail;
        return;
    }
    if(IsKeyPressed(SNES_START)){
        s_nPlayGameStatus = e_PG_DrawMenu;
        return;                
    }
    
    if(s_player.nStatus == e_PS_Jump)
    {

    }
    else
    {
        if(!s_player.bReborn)
        {
            Obstacle* aryCollision[MAX_OBSTACLE_CNT];
            int nCollision = FindObjectByPos(s_player.object.ptPos, aryCollision);
            if(nCollision > 0){
                s_player.nLives--;
                if(s_player.nLives <= 0)
                {
                    s_nPlayGameStatus = e_PG_ShowFail;
                    return;
                }
                else
                {
                    s_player.bReborn = TRUE;
                    s_player.nBlinkCnt = 0;
                }
            }
        }
        
        if(getTickCount() > s_nKeyBlockedTick)
        {
            if(IsKeyPressed(SNES_UP)){
                UpdateObjectCell(&s_player.object);
                if(!IsBlackCell(s_player.object.ptCell.x, s_player.object.ptCell.y - 1))
                {
                    //ErasePlayer();
                    s_player.object.ptPrevPos = s_player.object.ptPos;
                    s_player.object.ptCell.y--;
                    UpdateObjectPos(&s_player.object);                    
                    s_player.object.bRedraw = TRUE;
                }
            }
            else if(IsKeyPressed(SNES_DOWN)){
                UpdateObjectCell(&s_player.object);
                if(!IsBlackCell(s_player.object.ptCell.x, s_player.object.ptCell.y + 1))
                {
                    //ErasePlayer();
                    s_player.object.ptPrevPos = s_player.object.ptPos;
                    s_player.object.ptCell.y++;
                    UpdateObjectPos(&s_player.object);                    
                    s_player.object.bRedraw = TRUE;
                }
            }
            else if(IsKeyPressed(SNES_LEFT)){
                UpdateObjectCell(&s_player.object);
                if(!IsBlackCell(s_player.object.ptCell.x - 1, s_player.object.ptCell.y))
                {
                    //ErasePlayer();
                    s_player.object.ptPrevPos = s_player.object.ptPos;
                    s_player.object.ptCell.x--;
                    UpdateObjectPos(&s_player.object);                    
                    s_player.object.bRedraw = TRUE;
                }
            }
            else if(IsKeyPressed(SNES_RIGHT)){
                UpdateObjectCell(&s_player.object);
                if(!IsBlackCell(s_player.object.ptCell.x + 1, s_player.object.ptCell.y))
                {
                    //ErasePlayer();
                    s_player.object.ptPrevPos = s_player.object.ptPos;
                    s_player.object.ptCell.x++;
                    UpdateObjectPos(&s_player.object);                    
                    s_player.object.bRedraw = TRUE;
                }
            }
            else if(IsKeyPressed(SNES_B)){
                s_player.nStatus = e_PS_Jump;
            }
        }
    }
    
}

int OnPlayGame()
{
    static unsigned long s_PrevTick = 0;
    int nRet = ST_PlayGame;

    unsigned long curTick = getTickCount();
    if(s_nPlayGameStatus != e_PG_Playing) s_PrevTick = 0;
    LevelInfo* pCurLevel = GetCurLevelInfo();

    switch (s_nPlayGameStatus)
    {
    case e_PG_Start:
        DrawBackground_PG();
        DrawAllObstacles(TRUE);
        DrawPlayer(TRUE);
        DrawPlayerInfo();
        DrawTargetPos();
        
        s_nPlayGameStatus = e_PG_WaitPlaying;
        drawScaleString(300, 300, "Press any button to start!", 0x4, 3); 
        break;
    case e_PG_ReStart:
        DrawBackground_PG();
        DrawAllObstacles(TRUE);
        DrawPlayer(TRUE);
        DrawPlayerInfo();
        DrawTargetPos();
        
        s_nPlayGameStatus = e_PG_Playing;
        break;
    case e_PG_WaitPlaying:
        if(IsAnyKeyPressed())
        {
            s_nPlayGameStatus = e_PG_Playing;
            DrawBackground(&pCurLevel->background, 300 - 2, 300 - 2, 600, 30);
        } 
        break;
    case e_PG_Playing:
        UpdatePlayer();
        UpdateAllObstacles();
        DrawAllObstacles(FALSE);
        DrawPlayer(FALSE);        
        DrawTargetPos();

        if(s_PrevTick == 0) s_PrevTick = curTick;
        long delta = curTick - s_PrevTick;
        s_player.nRemainTime -= delta;
        if(s_player.nRemainTime < 0) s_player.nRemainTime = 0;
        s_PrevTick = curTick;
        DrawPlayerInfo();

        break;
    case e_PG_DrawMenu:
            drawRect2_delta(400, 200, 500, 250, 0x3B, 10, 1);
            drawScaleString(500, 300, "Restart Game", 0xA, 3);
            drawScaleString(550, 350, "Quit", 0xA, 3);
            s_nPlayGameStatus = e_PG_DrawSelectMark;
        break;
    case e_PG_DrawSelectMark:
        drawRect2_delta(GAME_MENU_SEL_MARK_X, GAME_MENU_SEL_MARK_Y + GAME_MENU_SEL_MARK_DELTA * s_nCurGameMenuItem, 
                SELECT_MARK_WIDTH, SELECT_MARK_WIDTH, SELECT_MARK_COLOR, SELECT_MARK_BRUSH, 0);
        s_nPlayGameStatus = e_PG_WaitMenuInput;
        break;
    case e_PG_WaitMenuInput:
        if(IsKeyPressed(SNES_UP))
        {
            drawRect(GAME_MENU_SEL_MARK_X - 5, GAME_MENU_SEL_MARK_Y - 5 + GAME_MENU_SEL_MARK_DELTA * s_nCurGameMenuItem, 
                    GAME_MENU_SEL_MARK_X + SELECT_MARK_WIDTH + 5, GAME_MENU_SEL_MARK_Y + 5 + GAME_MENU_SEL_MARK_DELTA * s_nCurGameMenuItem + SELECT_MARK_WIDTH, 0x33, 1);
            s_nCurGameMenuItem--;
            if(s_nCurGameMenuItem < 0) s_nCurGameMenuItem = 1;
            s_nPlayGameStatus = e_PG_DrawSelectMark;
        }
        else if(IsKeyPressed(SNES_DOWN)){
            drawRect(GAME_MENU_SEL_MARK_X - 5, GAME_MENU_SEL_MARK_Y - 5 + GAME_MENU_SEL_MARK_DELTA * s_nCurGameMenuItem, 
                    GAME_MENU_SEL_MARK_X + SELECT_MARK_WIDTH + 5, GAME_MENU_SEL_MARK_Y + 5 + GAME_MENU_SEL_MARK_DELTA * s_nCurGameMenuItem + SELECT_MARK_WIDTH, 0x33, 1);
            s_nCurGameMenuItem++;
            if(s_nCurGameMenuItem > 1) s_nCurGameMenuItem = 0;
            s_nPlayGameStatus = e_PG_DrawSelectMark;
        }
        else if(IsKeyPressed(SNES_A)){
            if(s_nCurGameMenuItem == 0){
                InitGame(s_nCurLevel, TRUE);
            }
            else{
                nRet = ST_MainMenu;
            }
        }
        else if(IsKeyPressed(SNES_START)){
            DrawBackground(&pCurLevel->background, 400, 200, 500, 250);
            s_nPlayGameStatus = e_PG_Playing; 
        }
        break;
    case e_PG_ShowWin:
        drawScaleString(400, 350, "You are win!", 0x1, 6);
        s_nPlayGameStatus = e_PG_WaitAction; 
        break;
    case e_PG_ShowFail:
        drawScaleString(400, 350, "You are lost!", 0x4, 6);
        s_nPlayGameStatus = e_PG_WaitAction; 
        break;
    case e_PG_WaitAction:
        if(IsAnyKeyPressed())
        {
            nRet = ST_MainMenu;
        }
        break;
    default:
        break;
    }
    
    return nRet;
}
