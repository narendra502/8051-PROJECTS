#include<reg52.h>
#include<string.h>

// Macro definitions
#define COL1 0
#define COL2 1
#define COL3 2
#define MAX_ROW 3

// LCD
#define LCD P3 /*Attached Lcd on Port*/
sbit rs = P1^0; /*Configure RS Pin*/
sbit rw = P1^1; /*Configure R/W pin*/
sbit e  = P1^2; /*Configure Enable pin*/

// KEYPAD
#define KEYPAD P2
sbit COL_1 =  P2^0;
sbit COL_2 =  P2^1;
sbit COL_3 =  P2^2;
sbit ROW_1 =  P2^4;
sbit ROW_2 =  P2^5;
sbit ROW_3 =  P2^6;
sbit ROW_4 =  P2^7;
sbit buz = P0^0; /* Buzzer */

// Stepper Motor
#define STEPPER_MOTOR P1

// LEDs
sbit greenLED = P2^3; /* Green LED for door open */
sbit redLED = P1^3; /* Red LED for access denied */

// Function declarations
void LcdCommand(const char cCommand);
void LcdData(const char cData);
void DisplayMessage(const char *pszMessage);
void LcdInit(void);
void Delay(unsigned int);
void DoResetRow(short siRowNumber);
char Check_Column(void);
char ReadColStatus(void);
void SteeperMotorForward(void);
void SteeperMotorBackward(void);

// Main function
int main()
{

    char ucData = 0;
    unsigned char cLen = 0;
    int iLenPassword = 0;
    char acBuffer[8] = {0};
		
    const char *pcPassword = "12345"; // Password
		greenLED = 0;
    redLED=0;
	buz=1;
    KEYPAD = 0x0F; // Make Row O/p & Col I/p
    LCD    = 0x00; // Make LCD O/p
    rs = rw = e = 0; // O/p
    STEPPER_MOTOR = 0x00; // Make o/p
    
    iLenPassword = strlen(pcPassword); // Calculate length of password
    LcdInit(); // Initialize the LCD
    
    LcdCommand(0x80); /* Address of DDRAM */
    DisplayMessage("Enter Password:");
    LcdCommand(0xc0); /* Address of DDRAM */
    
    while(1)
    {
			greenLED = 0;
        if(cLen < iLenPassword)
        {
					buz=1;
            ucData = ReadColStatus(); /* Read column Status */
            LcdData(ucData); /* Display entered character on LCD */
            acBuffer[cLen] = ucData; /* Store entered value in buffer */
            cLen++;
        }
        else
        {
            if(!strncmp(pcPassword, acBuffer, iLenPassword))
            {
							buz=1;
                LcdCommand(0x01); /* Clear the LCD */
                LcdCommand(0x80); /* Address of DDRAM */
                DisplayMessage("Door is Opening..");
               
                greenLED = 1; /* Turn ON green LED */
                SteeperMotorForward(); /* Move stepper motor forward */
              
                
                LcdCommand(0x01); /* Clear the LCD */
                LcdCommand(0x85); /* Address of DDRAM */
                DisplayMessage("Welcome");
                Delay(700);
                
                LcdCommand(0x01); /* Clear the LCD */
                LcdCommand(0x80); /* Address of DDRAM */
                DisplayMessage("Door is Closing..");
                SteeperMotorBackward(); /* Move stepper motor backward */
            
                cLen = 0;
                LcdCommand(0x01); /* Clear the LCD */
                LcdCommand(0x80); /* Address of DDRAM */
                DisplayMessage("Enter Password:");
                LcdCommand(0xc0); /* Address of DDRAM */
								redLED=0;
								redLED=0;
            }
            else
            {
							
              greenLED = 0;/* Turn ON red LED */
						
                LcdCommand(0x01); /* Clear the LCD */
                LcdCommand(0x80); /* Address of DDRAM */
                DisplayMessage("Access Denied..");
                Delay(100);
                
                LcdCommand(0x01); /* Clear the LCD */
                LcdCommand(0x80); /* Address of DDRAM */
                DisplayMessage("Wrong Password");
                Delay(300);
            
                redLED = 1; /* Turn OFF red LED */
            
                LcdCommand(0x01); /* Clear the LCD */
                LcdCommand(0x80); /* Address of DDRAM */
                DisplayMessage("Enter Password:");
                LcdCommand(0xc0); /* Address of DDRAM */
                         cLen = 0;
            }
					
        }
    }
    return 0;
}

// Function to write command on LCD
void LcdCommand(const char cCommand)
{
    rs = 0;
    rw = 0;
    e = 1;
    LCD = cCommand;
    Delay(1);
    e = 0;
}

// Function to write data on LCD
void LcdData(const char cData)
{
    rs = 1;
    rw = 0;
    e = 1;
    LCD = cData;
    Delay(1);
    e = 0;
}

// Function to display a message on LCD
void DisplayMessage(const char *pszMessage)
{
    while(*pszMessage != '\0')   // Till null character
    {
        LcdData(*pszMessage);
        pszMessage++;
    }
}

// Function to provide delay
void Delay(unsigned int i)
{
    int j, k;
    for(j = 0; j < i; j++)
        for(k = 0; k < 1275; k++);
}

// Initialize the LCD
void LcdInit(void)
{
    LcdCommand(0x01);
    Delay(2);
    LcdCommand(0x38);
    Delay(2);
    LcdCommand(0x06);
    Delay(2);
    LcdCommand(0x0c);
    Delay(2);
}

// Read the status of the column
char ReadColStatus(void)
{
    char ucData = 'p';
    while('p' == ucData)
    {
        ucData = Check_Column();
    }
    return ucData;
}

// Check the status of the columns
char Check_Column(void)
{
    short siColNumber = 0;
    const unsigned char ucaKeyPad[4][3] =
    {
        {'1','2','3'},
        {'4','5','6'},
        {'7','8','9'},
        {'*','0','#'}
    };
    
    for(siColNumber = 0; siColNumber <= MAX_ROW; siColNumber++)
    {
        DoResetRow(siColNumber); /* Reset the row */
        if(COL_1 == 0)
        {
            Delay(30);
            return ucaKeyPad[siColNumber][COL1];
        }
        if(COL_2 == 0)
        {
            Delay(30);
            return ucaKeyPad[siColNumber][COL2];
        }
        if(COL_3 == 0)
        {
            Delay(30);
            return ucaKeyPad[siColNumber][COL3];
        }
    }
    return 'p';
}

// Reset the row
void DoResetRow(short siRowNumber)
{
    ROW_1 = ROW_2 = ROW_3 = ROW_4 = 1; /* Make all rows high */
    switch(siRowNumber)
    {
    case 0:
        ROW_1 = 0;
        break;
    case 1:
        ROW_2 = 0;
        break;
    case 2:
        ROW_3 = 0;
        break;
    case 3:
        ROW_4 = 0;
        break;
    }
}

// Function to move stepper motor forward
void SteeperMotorForward(void)
{
    short int siLoop = 0;
    while(siLoop < 9)
    {
        STEPPER_MOTOR &= 0x0f;
        STEPPER_MOTOR |= 0xc0;
        Delay(10);
        STEPPER_MOTOR &= 0x0f;
        STEPPER_MOTOR |= 0x60;
        Delay(10);
        STEPPER_MOTOR &= 0x0f;
        STEPPER_MOTOR |= 0x30;
        Delay(10);
        STEPPER_MOTOR &= 0x0f;
        STEPPER_MOTOR |= 0x90;
        Delay(10);
        siLoop++;
    }
}

// Function to move stepper motor backward
void SteeperMotorBackward(void)
{
    short int siLoop = 0;
    while(siLoop < 9)
    {
        STEPPER_MOTOR &= 0x0f;
        STEPPER_MOTOR |= 0x30;
        Delay(10);
        STEPPER_MOTOR &= 0x0f;
        STEPPER_MOTOR |= 0x60;
        Delay(10);
        STEPPER_MOTOR &= 0x0f;
        STEPPER_MOTOR |= 0xc0;
        Delay(10);
        STEPPER_MOTOR &= 0x0f;
        STEPPER_MOTOR |= 0x90;
        Delay(10);
        siLoop++;
    }
}
