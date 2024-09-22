#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

// Function Declarations
void GPIO_PORTF_setup(void);   // Sets up GPIO Port F pins and interrupts
void PWMConfig(void);          // Configures the PWM generator
void GPIOFHandler(void);       // Interrupt handler for button presses
void dual_switch(void);        // Logic to change duty cycle based on button press
void SysTickHandler(void) {}   // Placeholder for SysTick, not used in this code

// Constants
#define frequency 100000      // 100 kHz frequency
#define time_period (16000000/frequency)  // Time period based on 16 MHz system clock
#define del_duty 15           // Duty cycle change for each button press



// Main function to initialize GPIO, PWM, and set up initial duty cycle
void main(void)
{
    GPIO_PORTF_setup();                   // Setup GPIO for buttons and LEDs
    PWMConfig();                          // Setup PWM
    duty = 50;                            // Start with 50% duty cycle
    PWM1_3_CMPA_R = (time_period * duty) / 100; // Set initial 50% duty cycle

    while(1) {
        // Infinite loop, everything handled in interrupt handler
    }
}

// Interrupt handler for GPIO Port F (triggered by button presses)
void GPIOFHandler(void)
{
    dual_switch();                        // Handle button press and adjust duty cycle

    int wait;
    for(wait = 0; wait < 1600 * 1000 / 4; wait++) {}  // Debounce delay of 0.25 seconds

    GPIO_PORTF_ICR_R = 0x11;              // Clear interrupt flags for PF4 and PF0
    GPIO_PORTF_IM_R |= 0x11;              // Re-enable interrupts for PF4 and PF0
}

// Logic for changing duty cycle based on button presses
void dual_switch(void)
{
    GPIO_PORTF_IM_R &= ~0x11;             // Disable interrupts during duty cycle adjustment

    // Check if SW2 (PF0) is pressed (duty cycle increase)
    if(GPIO_PORTF_RIS_R & 0x10)
    {
        if (duty < 90) {
            duty += del_duty;             // Increase duty cycle by 15%
        }
        if (duty >= 90) {
            duty = 90;                    // Cap duty cycle at 90%
        }
    }
    // Check if SW1 (PF4) is pressed (duty cycle decrease)
    if (GPIO_PORTF_RIS_R & 0x01)
    {
        if (duty > 5) {
            duty -= del_duty;             // Decrease duty cycle by 15%
        }
        if (duty <= 5) {
            duty = 5;                     // Cap duty cycle at 5%
        }
    }
    // If both switches are pressed, maintain current duty cycle
    if (GPIO_PORTF_RIS_R & 0x11) {
        duty = duty;                      // Do nothing, maintain the same duty cycle
    }

    PWM1_3_CMPA_R = (time_period * duty) / 100;  // Update PWM duty cycle
}
