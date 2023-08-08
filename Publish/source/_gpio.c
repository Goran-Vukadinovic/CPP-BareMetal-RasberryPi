#include "gpio.h"
#include "uart.h"

#include "_gpio.h"
#include "_utils.h"

static unsigned *gpio = (unsigned*)GPIO_BASE;
static int s_bBtnPressed[16]; // a integer array to store all of the buttons and keep track of what button has been pressed


// Waits for a time interval, passed as a parameter 
// (directly from slides: https://d2l.ucalgary.ca/d2l/le/content/500758/viewContent/5668893/View)
void Wait(int time)
{
    unsigned *clo = (unsigned*)CLO_REG;
    unsigned c = *clo + time; // in micro seconds
    while (c > *clo);
}


// Initializes a GPIO line
void Init_GPIO(int pin, int value)
{
    if (pin == LAT && value == 1) { // For latch INPUT line
       INP_GPIO(LAT);
    }
    else if(pin == LAT && value == 0){ // For latch OUTPUT line
        OUT_GPIO(LAT);
    }
    else if (pin == CLK && value == 1){ // For clock INPUT line
        INP_GPIO(CLK);
    }
    else if (pin == CLK && value == 0){ // For clock OUTPUT line
        OUT_GPIO(CLK);
    }
    else if (pin == DAT && value == 1){ // For data INPUT Line
        INP_GPIO(DAT);
    }
}

// Write to GPIO Latch line
// NOTE: LATCH TRACKS WHAT BUTTONS ON THE CONTROLLER ARE PRESSED
void Write_Latch(int value){
    if (value) {
            *GPSET0 = 1 << LAT; // Set GPSET0 to 1 (required to write on the latch)
        } else {
            *GPCLR0 = 1 << LAT; // Set GPCLR0 to 0 (required to say that a controller button was NOT pressed)
        }
}


// Write to GPIO Clock line
   // NOTE: THE CLOCK TRACKS INTERVALS FOR WHAT BUTTON

void Write_Clock(int value){
    if (value) {
            *GPSET0 = 1 << CLK; // Set GPSET0 to 1
        } else {
            *GPCLR0 = 1 << CLK; // Set GPCLR0 to 0
        }
}


int Read_Data() {
    int pin = DAT; // pin#10 is the pin of the SNES controller (which is equal to DATA line)
    int value = 1 << pin; // aligning the pin 
    int result = (*GPLEV0 & value); // masking everything else
    if (result == 0) {
        return 0; 
    } else {
        return 1; 
    }
}

#ifdef DEVELOP
char GetPressedKey()
{
    unsigned int* pUART0_DR = ((unsigned int*)(MMIO_BASE+0x00201000));
    unsigned int* pUART0_FR = ((unsigned int*)(MMIO_BASE+0x00201018));
    
    char r;
    if(*pUART0_FR&0x10) r= 0;
    else r=(char)(*pUART0_DR);
    return r;
}
#endif

// Reading the SNES controller inputs (pseudocode followed from D2L slides: https://d2l.ucalgary.ca/d2l/le/content/500758/viewContent/5668893/View)
int* Read_SNES()
{
    int i = 0;
#ifdef DEVELOP
    for (i = 0; i < 16; i++) {
        s_bBtnPressed[i] = 1;
    }
    char key = GetPressedKey();
    switch (key)
    {
    case 'b':
        s_bBtnPressed[SNES_B - 1] = 0;
        break;
    case 'y':
        s_bBtnPressed[SNES_Y - 1] = 0;
        break;
    case 'c':
        s_bBtnPressed[SNES_SELECT - 1] = 0;
        break;
    case 'z':
        s_bBtnPressed[SNES_START - 1] = 0;
        break;
    case '8':
        s_bBtnPressed[SNES_UP - 1] = 0;
        break;
    case '5':
        s_bBtnPressed[SNES_DOWN - 1] = 0;
        break;
    case '4':
        s_bBtnPressed[SNES_LEFT - 1] = 0;
        break;
    case '6':
        s_bBtnPressed[SNES_RIGHT - 1] = 0;
        break;
    case 'a':
        s_bBtnPressed[SNES_A - 1] = 0;
        break;
    case 'x':
        s_bBtnPressed[SNES_X - 1] = 0;
        break;
    case 'v':
        s_bBtnPressed[SNES_TLEFT - 1] = 0;
        break;
    case 'n':
        s_bBtnPressed[SNES_TRIGHT - 1] = 0;
        break;
    
    default:
        break;
    }
#else
    Write_Latch(1); 
    Write_Clock(1); 
    Wait(12); // signal to SNES to sample buttons
    Write_Latch(0); 
    for (i = 0; i < 16; i++) { // for all possible controller options
        Wait(6); // wait(6 micro seconds)
        Write_Clock(0); // falling edge of the clock
        Wait(6); // wait(6 micro seconds)
        s_bBtnPressed[i] = Read_Data(); // add to the array that will be returned
        Write_Clock(1); // rising edge; new cycle
    }
#endif
    return s_bBtnPressed; // returning the array of buttons (1= not pressed, 0= pressed)
}

void UpdateInput()
{
    Read_SNES();
}

int* GetKeyStatus()
{
    return s_bBtnPressed;
}

int IsKeyPressed(int key)
{
    return s_bBtnPressed[key - 1] == 0;
}

int IsAnyKeyPressed()
{
    int bPressed = 0;
    for (int i = 0; i < 16; i++){            
        if(s_bBtnPressed[i] == 0){
            bPressed = 1;
            break;
        }
    }
    return bPressed;
}