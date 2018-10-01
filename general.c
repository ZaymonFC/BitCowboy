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

void SetupTimer1A() {
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

	TIMER1_TAILR = 0xFFFF; // Set Preload
	TIMER1_TAMATCH = 0xFFF0; // Default Match (100% brightness)

	TIMER1_CTL |= 0x1; // Turn on TAEN
}

void SetupTimer1B() {
	int timerBit = 0x1 << 1;
	SYSCTL_RCGCTIMER |= timerBit;
	while ((SYSCTL_PRTIMER & timerBit) != timerBit);

	TIMER1_CTL &= ~(0x1 << 8); // Turn off TBEN (Bit 8)
	TIMER1_CTL &= ~(0x1 << 14); // Not Inverted (Bit 14)

	TIMER1_CFG &= ~0x7; // Clear (Bit 0-3)
	TIMER1_CFG |= 0x4;  // 16 Bit Timer

	TIMER1_TBMR |= 0x1 << 3; // Set TBAMS for PWM (Bit 3)
	TIMER1_TBMR &= ~(0x1 << 2); // Clear TBCMR (Bit 2)
	
	TIMER1_TBMR &= ~0x3; // Clear TBMR
	TIMER1_TBMR |= 0x2; // Set TBMR to Periodic

	TIMER1_TBILR = 0xFFFF; // Set Preload
	TIMER1_TBMATCH = 0xFFF0; // Default Match (100% brightness)

	TIMER1_CTL |= 0x1 << 8; // Turn on TBEN (Bit 8)
}

void SetupTimer2A() {
	int timerBit = 0x1 << 2;
	SYSCTL_RCGCTIMER |= timerBit;
	while ((SYSCTL_PRTIMER & timerBit) != timerBit);

	TIMER2_CTL &= ~0x1; // Turn off TAEN (Bit 0)
	TIMER2_CTL &= ~(0x1 << 6); // Not Inverted (Bit 6)
	TIMER2_CFG &= ~0x7; // Clear (Bit 0-3)
	TIMER2_CFG |= 0x4;  // 16 Bit Timer

	TIMER2_TAMR |= 0x1 << 3; // Set TAAMS for PWM (Bit 3)
	TIMER2_TAMR &= ~(0x1 << 2); // Clear TACMR (Bit 2)
	
	TIMER2_TAMR &= ~0x3; // Clear TAMR
	TIMER2_TAMR |= 0x2; // Set TAMR to Periodic

	TIMER2_TAILR = 0xFFFF; // Set Preload
	TIMER2_TAMATCH = 0xFFF0; // Default Match (100% brightness)

	TIMER2_CTL |= 0x1; // Turn on TAEN
}

void SetupRGB_PWM()
{
	Setup_GPIO_PWM_ForPin(2);
	SetupTimer1A();

	Setup_GPIO_PWM_ForPin(3);
	SetupTimer1B();

	Setup_GPIO_PWM_ForPin(4);
	SetupTimer2A();	
}

void GPIO_Setup_Button(){
	int portAValue = 0x1; // Port A
	int pinNumber = 6; // Pin 6
	int pinValue = 0x1 << 6;
	
	SYSCTL_RCGCGPIO |= portAValue;
	while ((SYSCTL_PRGPIO & portAValue) != portAValue);
	
	GPIOA_DEN |= pinValue; // Turn on digital enable
	GPIOA_DIR &= ~pinValue; // Clear to set as input
	// GPIOA_PUR |= pinValue;  // Set pull up resistor
	GPIOA_PDR |= pinValue;
	GPIOA_AFSEL &= ~pinValue; // Clear to set as regular GPIO
	GPIOA_PCTL &= ~(0xF << (pinNumber * 4)); // Clear to 0 for normal GPIO
}

int interpolate(int minSource, int maxSource, int minTarget, int maxTarget, int valueSource) {
	int rangeSource = (maxSource - minSource);
	int rangeTarget = (maxTarget - minTarget);
	int valueTarget = (((valueSource - minSource) * rangeTarget) / rangeSource) + minTarget;
	return valueTarget;
}

void SetLCDColour(int r, int g, int b) {
	TIMER1_TAMATCH = interpolate(0, 100, 0, 0xFFF0, g);
	TIMER1_TBMATCH = interpolate(0, 100, 0, 0xFFF0, b);
	TIMER2_TAMATCH = interpolate(0, 100, 0, 0xFFF0, r);
}


