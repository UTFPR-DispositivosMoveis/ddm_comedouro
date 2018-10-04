#include <driverlib.h>


int echo_pulse_duration, distance_cm;      // time in us

int main()
{

    // Disable watchdog timer
    WDT_A_hold(WDT_A_BASE);

    // Make P1.0 a digital output for the LED and P1.6
    // a digital output for the sensor trigger pulses
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN2);

    // Set up Timer_A1: SMCLK clock, input divider=1,
    // "continuous" mode. It counts from 0 to 65535,
    // incrementing once per clock cycle (i.e. every 1us).

    Timer_A_clear(TIMER_A0_BASE);
    Timer_A_
    TA1CTL = TASSEL_2 + ID_0 + MC_2;

    // Now just monitor distance sensor
    while(1)
    {

        // Send a 20us trigger pulse
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN3);
        __delay_cycles(11);           // 20us delay
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3);

        // Measure duration of echo pulse
        while (!GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2));   // Wait for start of echo pulse
        TA1R = 0;                     // Reset timer at start of pulse
        while (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2));    // Wait for end of echo pulse
        echo_pulse_duration = TA1R;   // Current timer value is pulse length

        distance_cm = 0.017 * echo_pulse_duration; // Convert from us to cm


    }
}
