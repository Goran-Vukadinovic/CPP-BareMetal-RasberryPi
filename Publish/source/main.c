#include "uart.h"
#include "fb.h"
#include "_define.h"
#include "_debug.h"
#include "_logic.h"
#include "_gpio.h"
#include "_utils.h"
#include "_playgame.h"
#include "_rand.h"
#include "_delays.h"
#include "_gameutils.h"

int main()
{
    #ifdef DEVELOP
    uart_init();// set up serial console and linear frame buffer
    #else
    Init_GPIO(CLK, 1);
    Init_GPIO(CLK ,0);
    Init_GPIO(LAT, 1);
    Init_GPIO(LAT, 0);
    Init_GPIO(DAT, 1);
    #endif
    
    fb_init();
    rand_init();

    _print("Init Success\r");
    drawScaleString(200, 360, "Press any button to start the game!", 0xB, 3); 

    UpdateInput();
    while (!IsAnyKeyPressed())
    {
        UpdateInput();
        wait_cycles(1000);
    }
    
    _print("Start button presssed\r\n");

    clearScreen();
    InitResource();
    
    PrepareMainMenu();
    // echo everything back
    while(IsRunning()) {
        UpdateDeltaTime();
        OnUpdate();
    }
    _print("Game exit\r");
    return 0;
}
