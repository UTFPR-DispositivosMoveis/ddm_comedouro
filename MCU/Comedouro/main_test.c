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
//    config_RTC();
    config_IO();


    while(1){
/*        set_MotorAngle(90);
        __delay_cycles(24000000);
        set_MotorAngle(0);
        __delay_cycles(24000000);
*/        dist = get_Distance();
    }


    //__no_operation();
}
