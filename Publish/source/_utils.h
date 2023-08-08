#ifndef _UTILS_H_
#define _UTILS_H_

#include "_define.h"
unsigned long getTickCount();

void drawScaleString(int x, int y, char *s, unsigned char attr, int mul);

void drawRect2(int x1, int y1, int x2, int y2, unsigned char color, int width, int fill);
void drawRect2_delta(int x1, int y1, int w, int h, unsigned char color, int width, int fill);
void drawLine2(int x1, int y1, int x2, int y2, unsigned char attr)  ;

void drawImageEx(unsigned char *image, int imgWidth, int imgHeight, int x, int y, int width, int height, 
                int toX, int toY, int bTransport);
void drawScaleImageEx(unsigned char *image, int imgWidth, int imgHeight, int x, int y, int width, int height, 
                int toX, int toY, int toWidth, int toHeight, int bTransport);

void drawImage(Image* image, POINT ptPos, int bTransport);
void drawScaleImage(Image* image, POINT ptTo, int toWidth, int toHeight, int bTransport);

int abs(int x);

#endif