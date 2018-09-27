#include "registers.h"
void DelayMs(unsigned int);

void DelayMs(unsigned int numLoops)
{
	volatile unsigned int lp;
	volatile unsigned int i;

	for(lp = 0; lp < numLoops; lp++)
		for (i = 0; i <= 0xFFFF; i++) ;
}

int Pow(int value, int exp) {
	for (int i = 0; i < exp; i++) {
		value *= value;
	}
	return value;
}

void SetupPWM_ForPin(int pin){ 
	int portBit = 5;
	int portValue = Pow(2, portBit);
	
	SYSCTL_RCGCGPIO |= portValue;
	while ((SYSCTL_PRGPIO & portValue) != 1);
	
	int pinValue = 2 << pin;
	
	GPIOF_DEN |= pinValue;
	GPIOF_DIR |= pinValue;
	GPIOF_AFSEL |= pinValue;
	GPIOF_PCTL &= ~(0xF << pin);
	GPIOF_PCTL |= (0x7 << pin);
}

void SetupRGB_PWM()
{
	// Setup PWM - PORT F
	SetupPWM_ForPin(2);
	// SetupPWM_ForPin(3);
	// SetupPWM_ForPin(4);
	
	// Setup Timer 1 A
	int timerBit = 2;
	RCGCTIMER |= timerBit;
	while ((PRTIMER & timerBit) != 1);

	TIMER1_CTL &= ~0x1; // Turn off TAEN
	TIMER1_CTL &= ~ (2 << 6);
	TIMER1_CFG &= ~0x7; // Clear
	TIMER1_CFG |= 0x4;  // 16 Bit Timer

	TIMER1_TAMR |= 2 << 3; // Set TAAMS for PWM
	TIMER1_TAMR &= ~(2 << 2); // Clear TACMR
	TIMER1_TAMR |= 0x2; // Set TAMR

	TIMER1_TAILR = 0xFFFF;
	TIMER1_TAMATCH = 0x0001;

	TIMER1_CTL |= 0x1; // Turn on TAEN

	// Setup Timer 1 B
	
	// Setup Timer 2 A
	
}

void SetLCDColour(int r, int g, int b) {
	return;
}


