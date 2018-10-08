#include "driverlib.h"
#include "Config_IO.h"
#include <stdio.h>



uint16_t DUTYv = 0;

int cont =0, FlagMedir=0;
uint16_t upCounter = 0;
uint16_t distancia = 0;

int NivelRacao = 0; //Altura � de 60cm (1-> 4a15cm, 2->15a30, 3->30a45, 4-> 45-55, 5-> >55)

void main (void){
   WDT_A_hold(WDT_A_BASE);
    config_BCS(24000);
    config_TimerA ();
    config_RTC ();
    while(true){
        //dis = Mede_Distancia ();
        //fprintf("Distancia: %d", &dis);
        __bis_SR_register(LPM0_bits + GIE);
    }
}
