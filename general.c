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

// Generic function to setup pins on port f for PWM output
void Setup_GPIO_PWM_ForPin(int pinNumber){ 
	int portFValue = 0x20; // Port F
	
	SYSCTL_RCGCGPIO |= portFValue;
	while ((SYSCTL_PRGPIO & portFValue) != portFValue);
	
	int pinValue = 0x1 << pinNumber;
	
	GPIOF_DEN |= pinValue;
	GPIOF_DIR |= pinValue;
	GPIOF_AFSEL |= pinValue;
	
	GPIOF_PCTL &= ~(0xF << (pinNumber * 4));
	GPIOF_PCTL |= (0x7 << (pinNumber * 4));
}

void SetupRGB_PWM()
{
	// Setup F2, F3, F4
	Setup_GPIO_PWM_ForPin(2);
	Setup_GPIO_PWM_ForPin(3);
	Setup_GPIO_PWM_ForPin(4);
	
  // Setup Timer 1 A
	int timerBit = 0x1 << 1;
	SYSCTL_RCGCTIMER |= timerBit;
	while ((SYSCTL_PRTIMER & timerBit) != timerBit);

	TIMER1_CTL &= ~0x1; // Turn off TAEN (Bit 0)
	TIMER1_CTL &= ~(0x1 << 6); // Not Inverted (Bit 6)
	TIMER1_CFG &= ~0x7; // Clear (Bit 0-3)
	TIMER1_CFG |= 0x4;  // 16 Bit Timer

	TIMER1_TAMR |= 0x1 << 3; // Set TAAMS for PWM (Bit 3)
	TIMER1_TAMR &= ~(0x1 << 2); // Clear TACMR (Bit 2)
	
	TIMER1_TAMR &= ~0x3; // Clear TAMR
	TIMER1_TAMR |= 0x2; // Set TAMR to Periodic

	TIMER1_TAILR = 0xFFFF;
	TIMER1_TAMATCH = 0xFFF0;

	TIMER1_CTL |= 0x1; // Turn on TAEN
	
	// Setup Timer 1 B
	
	// Setup Timer 2 A

	
}

int interpolate(int minSource, int maxSource, int minTarget, int maxTarget, int valueSource) {
	int rangeSource = (maxSource - minSource);
	int rangeTarget = (maxTarget - minTarget);
	int valueTarget = (((valueSource - minSource) * rangeTarget) / rangeSource) + minTarget;
	return valueTarget;
}

void SetLCDColour(int r, int g, int b) {
	int redMatch = 0xFFFF * r;
	TIMER1_TAMATCH = interpolate(0, 100, 0, 0xFFF0, g);
	int blueMatch = 0xFFFF * b;
}


