#ifndef _DEFINE_H_
#define _DEFINE_H_

typedef unsigned int BOOL;
#define TRUE    (1)
#define FALSE   (0)

enum E_MainStatus
{
    ST_MainMenu,
    ST_PlayGame,
    ST_ExitGame,
};

#define SCREEN_WIDTH        1280
#define SCREEN_HEIGHT       720

#define SELECT_MARK_WIDTH       30
#define SELECT_MARK_OFFSET_X    350
#define SELECT_MARK_OFFSET_Y    365
#define SELECT_MARK_BRUSH       10
#define SELECT_MARK_DELTA       95
#define SELECT_MARK_COLOR       0xB

#define MAX_OBSTACLE_CNT          32
#define LEVEL_COUNT             1
#define OBSTACLE_TYPE_COUNT     1
#define MAX_CELL_WIDTH          (64)
#define MAX_CELL_HEIGHT         (64)
#define CELL_WIDTH              (32)
#define CELL_HEIGHT             (32)


typedef struct _POINT {
    int x;
    int y;
} POINT;

typedef struct _Image {
    unsigned char *pixel_data;
    int width;
    int height;
} Image;

typedef struct _Object {
    Image image;

    int nWidth;
    int nHeight;

    POINT ptCell;
    POINT ptPos;
    POINT ptPrevPos;
    int bRedraw;
} Object;


typedef struct _LevelInfo{
    Image background;
    int nLimitSecond;
    int nLives;
    int nObstacles;
    int nCellWidth;
    int nCellHeight;
    char aryCellMap[MAX_CELL_WIDTH][MAX_CELL_HEIGHT];
    POINT ptStartCell;
    POINT ptTargetCell;    
    int nRoadCellCnt;
} LevelInfo;


#endif