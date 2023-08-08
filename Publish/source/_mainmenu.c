#include "_mainmenu.h"
#include "fb.h"

#include "_define.h"
#include "_utils.h"
#include "_gpio.h"

extern struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[1280 * 720 * 4 + 1];
} mainMenuStart;

enum{
    e_MM_DrawBackground,
    e_MM_WaitInput,
    e_MM_DrawSelectMark,
};

static int s_nMainMenuStatus = e_MM_DrawBackground;
static int s_nMainMenuCurItem = 0;

void InitMainMenu()
{
    s_nMainMenuStatus = e_MM_DrawBackground;
    s_nMainMenuCurItem = 0;
}

void EraseSelectMark()
{
    drawImageEx(mainMenuStart.pixel_data, mainMenuStart.width, mainMenuStart.height, 
                    SELECT_MARK_OFFSET_X, SELECT_MARK_OFFSET_Y + SELECT_MARK_DELTA * s_nMainMenuCurItem, 
                    SELECT_MARK_WIDTH, SELECT_MARK_WIDTH,
                    SELECT_MARK_OFFSET_X, SELECT_MARK_OFFSET_Y + SELECT_MARK_DELTA * s_nMainMenuCurItem, 0);
}

void DrawSelectMark()
{
    drawRect2_delta(SELECT_MARK_OFFSET_X, SELECT_MARK_OFFSET_Y + SELECT_MARK_DELTA * s_nMainMenuCurItem, 
                SELECT_MARK_WIDTH, SELECT_MARK_WIDTH, SELECT_MARK_COLOR, SELECT_MARK_BRUSH, 0);
}

void DrawBackground_MM()
{
    myDrawImage(mainMenuStart.pixel_data, mainMenuStart.width, mainMenuStart.height,0,0, 0);
}

int OnMainMenu()
{
    int nRet = ST_MainMenu;
    switch (s_nMainMenuStatus)
    {
    case e_MM_DrawBackground:
        DrawBackground_MM();
        s_nMainMenuStatus = e_MM_DrawSelectMark;
        break;
    case e_MM_DrawSelectMark:
        DrawSelectMark();
        s_nMainMenuStatus = e_MM_WaitInput;
        break;
    case e_MM_WaitInput:
        if(IsKeyPressed(SNES_UP))
        {
            EraseSelectMark();
            s_nMainMenuCurItem--;
            if(s_nMainMenuCurItem < 0) s_nMainMenuCurItem = 1;
            s_nMainMenuStatus = e_MM_DrawSelectMark;
        }
        else if(IsKeyPressed(SNES_DOWN)){
            EraseSelectMark();
            s_nMainMenuCurItem++;
            if(s_nMainMenuCurItem > 1) s_nMainMenuCurItem = 0;
            s_nMainMenuStatus = e_MM_DrawSelectMark;
        }
        else if(IsKeyPressed(SNES_A)){
            if(s_nMainMenuCurItem == 0){
                nRet = ST_PlayGame;
            }
            else{
                nRet = ST_ExitGame;
            }
        }
        break;
    
    default:
        break;
    }
    return nRet;
}
