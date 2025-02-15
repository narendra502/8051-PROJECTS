#include <reg51.h>

// Pin configuration
sbit rs = P1^0;  // Register Select pin
sbit rw = P1^1;  // Read/Write pin
sbit en = P1^2;
sbit led1=P3^0;
sbit led2=P3^1;
sbit buz=P3^2;
sbit sw1=P3^3;
sbit hi=P2^6;
sbit sw2=P3^4;
sbit motp=P3^6;
sbit motn=P3^7;

// Enable pin

// Function prototypes
void lcdcmd(unsigned char);
void lcddat(unsigned char);
void lcdstr(char *);
void delay();
void init(void);

// Main function
void main() {
	unsigned char seg[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
	unsigned char x;
	unsigned int i;
	unsigned int j;
    init() ;
	P0=0X00;
	// Initialize the LCD
while(1){
	if(sw1==0){
		hi=1;
		
    lcdcmd(0x80);  // Move the cursor to the beginning of the first line
    lcdstr("!Please Go In"); 
		delay();
		lcdcmd(0xc0);
lcdstr("Forward Path");
delay();
lcdcmd(0x1c);
// Display "HELLO" on the first line
led1=1;
		buz=1;
		motp=1;
		for(j=0;j<30000;j++);
		for(x=0;x<10;x++)
		{
			P0=seg[x];
			for(i=0;i<60000;i++);
		}
		
	}
	else {
		motp=0;
		led1=0;
		buz=0;
		P0=0X00;
		lcdcmd(0x01);
	}
	if(sw2==0){
		buz=1;
		led2=1;
		lcdcmd(0x80);
		lcdstr("hey please stop");
		lcdcmd(0x1c);
		motn=1;
		
	}
	else {
		led2=0;
		motn=0;
		buz=0;
		lcdcmd(0x01);
	}
	
		
     // Display "I'm GOOD" on the second line

    
}}

// Function to send command to LCD
void lcdcmd(unsigned char val) {
    P2 = val;  // Place command on data bus (P2)
    rs = 0;    // Command mode (RS=0)
    rw = 0;    // Write mode (RW=0)
    en = 1;    // Enable high
    delay();
    en = 0;    // Enable low
}

// Function to send data to LCD
void lcddat(unsigned char val) {
    P2 = val;  // Place data on data bus (P2)
    rs = 1;    // Data mode (RS=1)
    rw = 0;    // Write mode (RW=0)
    en = 1;    // Enable high
    delay();
    en = 0;    // Enable low
}

// Function to provide delay
void delay()
{
	unsigned int i ;
	for(i=0;i<12000;i++);
}


// Function to send a string to LCD
void lcdstr(char *str) {
    while(*str) {
        lcddat(*str++);  // Send each character to the LCD
    }
}

// Function to initialize LCD
void init(void) {
    lcdcmd(0x38);  // 8-bit mode, 2-line display, 5x7 dots
    lcdcmd(0x0C);  // Display ON, cursor OFF
    lcdcmd(0x01);  // Clear display screen
    lcdcmd(0x06);  // Increment cursor (shift cursor to right)
    delay();
}
