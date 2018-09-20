
#include "driverlib.h"
#include "HD44780.h"

HD44780 theHD44780;


void main(void)
{
	WDT_A_hold(WDT_A_BASE);


	HD44780_init(&theHD44780, GPIO_PIN2, GPIO_PIN3, GPIO_PIN4, GPIO_PIN5, GPIO_PIN6, GPIO_PIN7);

	HD44780_begin(&theHD44780, 16, 2);

	while(1){
	    HD44780_setCursor(&theHD44780, 1, 2);
	    HD44780_print_string(&theHD44780, "wat");
	}


}

