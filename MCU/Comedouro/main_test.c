#include    "driverlib.h"
#include    "IO/Config_IO.h"
#include    "IO/Motor/Motor.h"
#include    "IO/SR04/SR04.h"
#include    <stdio.h>


//int NivelRacao = 0; //Altura � de 60cm (1-> 4a15cm, 2->15a30, 3->30a45, 4-> 45-55, 5-> >55)


unsigned int dist;
void main (void){
    WDT_A_hold(WDT_A_BASE);
    config_BCS(24000);
    config_UART();

    while(1){
        __bis_SR_register(LPM0_bits + GIE);
    }
}
