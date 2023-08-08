#ifndef _GPIO_H
#define _GPIO_H

// SNES controller pins
#define LAT 9
#define CLK 11
#define DAT 10

// Directly from https://d2l.ucalgary.ca/d2l/le/content/500758/viewContent/5668893/View(CPSC 359 RPi 2 SNES Slides)
#define GPIO_BASE 0xFE200000
#define CLO_REG 0x7E003004 // WAIT register

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y) (From RPi 1 GPIO Slides)

#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |= (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

// SNES buttons
#define SNES_B 1
#define SNES_Y 2
#define SNES_SELECT 3
#define SNES_START 4
#define SNES_UP 5
#define SNES_DOWN 6
#define SNES_LEFT 7
#define SNES_RIGHT 8
#define SNES_A 9
#define SNES_X 10
#define SNES_TLEFT 11
#define SNES_TRIGHT 12

void Init_GPIO(int pin, int value);
void UpdateInput();
int* GetKeyStatus();
int IsKeyPressed(int key);
int* Read_SNES();
int IsAnyKeyPressed();
#endif