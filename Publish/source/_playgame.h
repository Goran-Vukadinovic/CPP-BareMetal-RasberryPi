#ifndef _PLAY_GAME_H_
#define _PLAY_GAME_H_

#include "_define.h"

enum{
    e_PG_Start,
    e_PG_ReStart,
    e_PG_WaitPlaying,
    e_PG_Playing,
    e_PG_DrawMenu,
    e_PG_DrawSelectMark,
    e_PG_WaitMenuInput,
    e_PG_ShowWin,    
    e_PG_ShowFail,
    e_PG_WaitAction
};

#define GAME_MENU_SEL_MARK_X        420
#define GAME_MENU_SEL_MARK_Y        300
#define GAME_MENU_SEL_MARK_DELTA    45



int OnPlayGame();
void InitResource();
void PrepareGamePlay();

#endif