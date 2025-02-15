#include <reg51.h>  // Include the header file for 8051 microcontroller

#define smot P1    // Define smot as Port 1 for controlling the motor

// Function prototypes
void delay(unsigned int d);   // Delay function prototype
void step_clock();            // Stepper motor clockwise function prototype
void step_anticlock();        // Stepper motor anticlockwise function prototype

// Define bits for controlling two red and two green LEDs
sbit red1 = P2^0;   // Red LED 1 on Port 2, bit 0
sbit red2 = P2^1;   // Red LED 2 on Port 2, bit 1
sbit green1 = P2^2; // Green LED 1 on Port 2, bit 2
sbit green2 = P2^3; // Green LED 2 on Port 2, bit 3

// Interrupt Service Routine (ISR) for sensor 1
void sensor1() interrupt 1
{
    green1 = green2 = 0;  // Turn off the green LEDs
    red1 = red2 = 1;      // Turn on the red LEDs
    step_clock();          // Rotate the motor clockwise
}

// Interrupt Service Routine (ISR) for sensor 2
void sensor2() interrupt 2
{
    red1 = red2 = 0;      // Turn off the red LEDs
    green1 = green2 = 1;  // Turn on the green LEDs
    step_anticlock();     // Rotate the motor anticlockwise
}

// Main function
void main()
{
    smot = 0x00;  // Initialize the motor control pins to 0 (off state)
    EA = 1;       // Enable global interrupts
    EX0 = 1;      // Enable external interrupt 0 (for sensor 1)
    EX1 = 1;      // Enable external interrupt 1 (for sensor 2)

    while(1);  // Infinite loop (keep the program running)
}

// Function to rotate the stepper motor in a clockwise direction
void step_clock()
{
    smot = 0x09;  // Set the motor pins for clockwise step
    delay(1000);  // Delay to allow the motor to step
    smot = 0x0C;  // Change motor pins for next clockwise step
    delay(1000);  // Delay to allow the motor to step
}

// Function to rotate the stepper motor in an anticlockwise direction
void step_anticlock()
{
    smot = 0x0C;  // Set the motor pins for anticlockwise step
    delay(1000);  // Delay to allow the motor to step
    smot = 0x09;  // Change motor pins for next anticlockwise step
    delay(1000);  // Delay to allow the motor to step
}

// Delay function to introduce a small delay
void delay(unsigned int d)
{
    unsigned char i;
    for(; d > 0; d--)  // Outer loop for delay duration
    {
        for(i = 255; i > 0; i--);  // Inner loop to waste time (provide delay)
        for(i = 255; i > 0; i--);  // Inner loop to waste time (provide delay)
    }
}
