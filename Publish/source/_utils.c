#include "_utils.h"
#include "_define.h"

#include "fb.h"

#include "_delays.h"
#include "_debug.h"

enum {
    FONT_WIDTH     = 8,
    FONT_HEIGHT    = 8,
    FONT_BPG       = 8,  // Bytes per glyph
    FONT_BPL       = 1,  // Bytes per line
    FONT_NUMGLYPHS = 224
};
extern unsigned char font[FONT_NUMGLYPHS][FONT_BPG];

unsigned long getTickCount()
{
    //return s_globalCurTick;
    return get_system_timer();
}

void drawRect2_delta(int x1, int y1, int w, int h, unsigned char color, int width, int fill)
{
    drawRect2(x1, y1, x1 + w, y1 + h, color, width, fill);
}

void drawRect2(int x1, int y1, int x2, int y2, unsigned char color, int width, int fill)
{
    int y=y1;
    while (y < y2) {
        int x=x1;
        while (x < x2) {
            if ((x >= x1 && x < x1 + width) 
            || (x <= x2 && x > x2 - width) 
            || (y >= y1  && y < y1 + width)
            || (y <= y2 && y > y2 - width)) drawPixel(x, y, color);
            else if (fill) drawPixel(x, y, (color & 0xf0) >> 4);
            x++;
       }
       y++;
    }
}

void drawRect3(int x1, int y1, int x2, int y2, unsigned char color, int width)
{
    for(int x = x1 ; x <= x2 ; x++)
    {
        for(int i = 0 ; i < width ; i++)
        {
            drawPixel(x, y1 + i, color);
            drawPixel(x, y2 - i, color);
        }
    }
    for(int y = y1 ; y <= y2 ; y++)
    {
        for(int i = 0 ; i < width ; i++)
        {
            drawPixel(x1 + i, y, color);
            drawPixel(x2 - i, y, color);
        }
    }
}

void drawLine2(int x1, int y1, int x2, int y2, unsigned char attr)  
{  
    int dx, dy, p, x, y;

    dx = x2-x1;
    dy = y2-y1;
    x = x1;
    y = y1;
    p = 2*dy-dx;

    if(dx != 0)
    {
        while (x<x2) {
            if (p >= 0) {
                drawPixel(x,y,attr);
                y++;
                p = p+2*dy-2*dx;
            } else {
                drawPixel(x,y,attr);
                p = p+2*dy;
            }
            x++;
        }
    }
    else{
        y = y1;
        while (y<y2) {
            drawPixel(x,y,attr);
            y++;
        }
    }
}

void drawImageEx(unsigned char *image, int imgWidth, int imgHeight, 
                int x, int y, int width, int height, int toX, int toY, int bTransport)
{
    if(x < 0 || x + width > imgWidth || y < 0 || y + height > imgHeight) return;
    for(int i = 0 ; i < width ; i++)
    {
        int xx = x + i;
        for(int j = 0 ;  j < height ; j++)
        {
            int yy = y + j;
            int offset = (xx + yy * imgWidth) * 4;
            int rgba_color = *(int*)&image[offset];
            int argb_color = __builtin_bswap32(rgba_color);
            argb_color = (argb_color << 24) | (argb_color >> 8);
            if (bTransport == 1){
                if (argb_color != 0xFF000000 && argb_color != 0x00000000) myDrawPixel(toX + i, toY + j, argb_color);
            }
            else{
                myDrawPixel(toX + i, toY + j, argb_color);
            }
        }
    }
}

void drawScaleImageEx(unsigned char *image, int imgWidth, int imgHeight, 
            int x, int y, int width, int height, int toX, int toY, int toWidth, int toHeight, int bTransport)
{
    if(x < 0 || x + width > imgWidth || y < 0 || y + height > imgHeight) return;
    if(toWidth == 0 || toHeight == 0) return;
    float fX = 1.f * width / toWidth;
    float fY = 1.f * height / toHeight;
    for(int i = 0 ; i < toWidth ; i++)
    {
        int srcX = x + i * fX;
        int xx = toX + i;
        for(int j = 0 ;  j < toHeight ; j++)
        {
            int srcY = y + j * fY;
            int yy = toY + j;
            int orgOffset = (srcX + srcY * imgWidth) * 4;
            int rgba_color = *(int*)&image[orgOffset];
            int argb_color = __builtin_bswap32(rgba_color);
            argb_color = (argb_color << 24) | (argb_color >> 8);
            if (bTransport == 1){
                if (argb_color != 0xFF000000 && argb_color != 0x00000000) myDrawPixel(xx, yy, argb_color);
            }
            else{
                myDrawPixel(xx, yy, argb_color);
            }
        }
    }
}

void drawScaleChar(unsigned char ch, int x, int y, unsigned char attr, int mul)
{
    unsigned char *glyph = (unsigned char *)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;


    for (int i=0; i<FONT_HEIGHT; i++) 
    {
	    for (int j=0; j<FONT_WIDTH; j++) 
        {
	        unsigned char mask = 1 << j;
	        unsigned char col = (*glyph & mask) ? attr & 0x0f : (attr & 0xf0) >> 4;
            for (int ii=0; ii<mul; ii++) 
                for (int jj=0; jj<mul; jj++)
	                if(*glyph & mask) drawPixel(x+j * mul + ii, y + i * mul + jj, col);
	    }
	    glyph += FONT_BPL;
    }
}
void drawScaleString(int x, int y, char *s, unsigned char attr, int mul)
{
    while (*s) 
    {
        if (*s == '\r') 
        {
            x = 0;
        } 
        else if(*s == '\n') 
        {
            x = 0; y += FONT_HEIGHT * mul;
        } 
        else 
        {
	        drawScaleChar(*s, x, y, attr, mul);
            x += FONT_WIDTH * mul;
        }
        s++;
    }
}

int abs(int x)
{
    if(x < 0) return -x;
    return x;
}


void drawImage(Image* image, POINT ptPos, int bTransport)
{
    drawImageEx(image->pixel_data, image->width, image->height, 
            0, 0, image->width, image->height, 
            ptPos.x, ptPos.y, bTransport);
}

void drawScaleImage(Image* image, POINT ptPos, int width, int height, int bTransport)
{
    drawScaleImageEx(image->pixel_data, image->width, image->height, 
            0, 0, image->width, image->height,
            ptPos.x, ptPos.y, width, height, 
            bTransport);

}
