#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

volatile int i = 0;  // Used to measure the switch press time
int duty;            // PWM duty cycle variable

// Function Declarations
void GPIO_PORTF_setup(void);   // Sets up GPIO Port F pins and interrupts
void PWM_setup(void);          // Sets up the PWM module
void Portf_interrupt_handler(void);  // Interrupt handler for switch presses



// Main function
void main(void)
{
    GPIO_PORTF_setup();                     // Initialize GPIO Port F
    PWM_setup();                            // Initialize PWM
    duty = 50;                              // Set initial duty cycle to 50%
    PWM1_3_CMPA_R = (160 * duty) / 100;     // Set the duty cycle

    while (1)
    {
        // Main loop, nothing to do as interrupts handle the logic
    }
}

// Interrupt handler for Port F, triggered by switch press
void Portf_interrupt_handler(void)
{
    i = 0;

    // Measure how long the switch is pressed by counting in a loop
    while ((GPIO_PORTF_DATA_R & 0x01) == 0) {
        i++;
    }

    // Handle switch 1 (PF4)
    if (GPIO_PORTF_MIS_R & 0x10)
    {
        // Implement behavior for switch 1 if needed
    }
    // Handle switch 2 (PF0)
    else if (GPIO_PORTF_MIS_R & 0x01)
    {
        // Long press on switch 2, decrease duty cycle by 5%
        if (i >= 900000)
        {
            if (duty > 5)
            {
                duty -= 5;
            }
            else
            {
                duty = 5;
            }
        }
        // Short press on switch 2, increase duty cycle by 5%
        else if (i >= 0 && i <= 900000)
        {
            if (duty < 95)
            {
                duty += 5;
            }
            else
            {
                duty = 100;
            }
        }
    }

    PWM1_3_CMPA_R = (160 * duty) / 100;     // Update PWM duty cycle

    // Simple delay for debounce
    int j;
    for (j = 0; j < 1600 * 100 / 4; j++) {}

    GPIO_PORTF_ICR_R = 0x11;                // Clear interrupt flags for PF4 and PF0
}
