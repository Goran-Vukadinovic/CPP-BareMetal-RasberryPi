#include "_define.h"
#include "_logic.h"
#include "fb.h"

#include "_utils.h"
#include "_gpio.h"
#include "_mainmenu.h"
#include "_playgame.h"
#include "_gameutils.h"
#include "_debug.h"

static BOOL s_bIsRuning = FALSE;

static enum E_MainStatus  s_eMainStatus = ST_MainMenu;

BOOL IsRunning()
{
    return s_bIsRuning;
}
void PrepareMainMenu()
{
    s_bIsRuning = TRUE;
    s_eMainStatus = ST_MainMenu;
    InitMainMenu();
}

void OnUpdate()
{
    UpdateInput();
    enum E_MainStatus next = s_eMainStatus;
    switch (s_eMainStatus)
    {
    case ST_MainMenu:
        next = OnMainMenu();
        if(next == ST_PlayGame)
        {
            PrepareGamePlay();
            s_eMainStatus = ST_PlayGame;
        }
        else 
        {
            s_eMainStatus = next;
        }
        break;
    case ST_PlayGame:
        next = OnPlayGame();        
        if(next == ST_MainMenu)
        {
            PrepareMainMenu();
            s_eMainStatus = ST_MainMenu;
        }
        else 
        {
            s_eMainStatus = next;
        }
        break;
    case ST_ExitGame:
        clearScreen();
        drawScaleString(500, 360, "Bye bye!", 0xB, 4); 
        s_bIsRuning = FALSE;
        //system("sudo poweroff");
        break;
    default:
        break;
    }
}