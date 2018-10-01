#include "registers.h"
#include "types.h"

void DelayMs(unsigned int);
void SetLCDColour(int, int, int);
int interpolate(int, int, int, int, int);

void DelayMs(unsigned int numLoops) {
  volatile unsigned int lp;
  volatile unsigned int i;

  for (lp = 0; lp < numLoops; lp++)
    for (i = 0; i <= 0xFFFF; i++)
      ;
}

int Pow(int value, int exp) {
  for (int i = 0; i < exp; i++) {
    value *= value;
  }
  return value;
}

// Generic function to setup pins on port f for PWM output
void Setup_GPIO_PWM_ForPin(int pinNumber) {
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

  TIMER1_CTL &= ~0x1;        // Turn off TAEN (Bit 0)
  TIMER1_CTL &= ~(0x1 << 6); // Not Inverted (Bit 6)
  TIMER1_CFG &= ~0x7;        // Clear (Bit 0-3)
  TIMER1_CFG |= 0x4;         // 16 Bit Timer

  TIMER1_TAMR |= 0x1 << 3;    // Set TAAMS for PWM (Bit 3)
  TIMER1_TAMR &= ~(0x1 << 2); // Clear TACMR (Bit 2)

  TIMER1_TAMR &= ~0x3; // Clear TAMR
  TIMER1_TAMR |= 0x2;  // Set TAMR to Periodic

  TIMER1_TAILR = 0xFFFF;   // Set Preload
  TIMER1_TAMATCH = 0xFFF0; // Default Match (100% brightness)

  TIMER1_CTL |= 0x1; // Turn on TAEN
}

void SetupTimer1B() {
  int timerBit = 0x1 << 1;
  SYSCTL_RCGCTIMER |= timerBit;
  while ((SYSCTL_PRTIMER & timerBit) != timerBit);

  TIMER1_CTL &= ~(0x1 << 8);  // Turn off TBEN (Bit 8)
  TIMER1_CTL &= ~(0x1 << 14); // Not Inverted (Bit 14)

  TIMER1_CFG &= ~0x7; // Clear (Bit 0-3)
  TIMER1_CFG |= 0x4;  // 16 Bit Timer

  TIMER1_TBMR |= 0x1 << 3;    // Set TBAMS for PWM (Bit 3)
  TIMER1_TBMR &= ~(0x1 << 2); // Clear TBCMR (Bit 2)

  TIMER1_TBMR &= ~0x3; // Clear TBMR
  TIMER1_TBMR |= 0x2;  // Set TBMR to Periodic

  TIMER1_TBILR = 0xFFFF;   // Set Preload
  TIMER1_TBMATCH = 0xFFF0; // Default Match (100% brightness)

  TIMER1_CTL |= 0x1 << 8; // Turn on TBEN (Bit 8)
}

void SetupTimer2A() {
  int timerBit = 0x1 << 2;
  SYSCTL_RCGCTIMER |= timerBit;
  while ((SYSCTL_PRTIMER & timerBit) != timerBit);

  TIMER2_CTL &= ~0x1;        // Turn off TAEN (Bit 0)
  TIMER2_CTL &= ~(0x1 << 6); // Not Inverted (Bit 6)
  TIMER2_CFG &= ~0x7;        // Clear (Bit 0-3)
  TIMER2_CFG |= 0x4;         // 16 Bit Timer

  TIMER2_TAMR |= 0x1 << 3;    // Set TAAMS for PWM (Bit 3)
  TIMER2_TAMR &= ~(0x1 << 2); // Clear TACMR (Bit 2)

  TIMER2_TAMR &= ~0x3; // Clear TAMR
  TIMER2_TAMR |= 0x2;  // Set TAMR to Periodic

  TIMER2_TAILR = 0xFFFF;   // Set Preload
  TIMER2_TAMATCH = 0xFFF0; // Default Match (100% brightness)

  TIMER2_CTL |= 0x1; // Turn on TAEN
}

void SetupRGB_PWM() {
  Setup_GPIO_PWM_ForPin(2);
  SetupTimer1A();

  Setup_GPIO_PWM_ForPin(3);
  SetupTimer1B();

  Setup_GPIO_PWM_ForPin(4);
  SetupTimer2A();
}

void GPIO_Setup_Button() {
  int portAValue = 0x1; // Port A (Bit 0)
  int pinNumber = 6;    // Pin 6
  int pinValue = 0x1 << pinNumber;

  SYSCTL_RCGCGPIO |= portAValue;
  while ((SYSCTL_PRGPIO & portAValue) != portAValue);

  GPIOA_DEN |= pinValue;     // Turn on digital enable
  GPIOA_DIR &= ~pinValue;    // Clear to set as input
  GPIOA_PDR |= pinValue;     // Set Pull Down Resistor
  GPIOA_AFSEL &= ~pinValue;  // Clear to set as regular GPIO
  GPIOA_PCTL &= ~(0xF << (pinNumber * 4));  // Clear to 0 for normal GPIO
}

void GPIO_Setup_ADC() {
  int portEValue = 0x1 << 4; // Port E (Bit 4)
  int pinNumber = 3;    // Pin 3
  int pinValue = 0x1 << pinNumber;

  SYSCTL_RCGCGPIO |= portEValue;
  while ((SYSCTL_PRGPIO & portEValue) != portEValue);

  GPIOE_DEN   &= ~pinValue;  // Turn off Digital enable
  GPIOE_DIR   &= ~pinValue;  // Clear to set as input
  GPIOE_AFSEL |= pinValue;   // Enable alternate function
  GPIOE_AMSEL |= pinValue;   // Enable analogue mode
}

void ADC_Setup_Joystick() {
  SYSCTL_RCGCADC |= 0x1; // Set Bit 0 for ADC 0
  while((SYSCTL_PRADC & 0x1) != 0x1); // Wait for stabalisation

  GPIO_Setup_ADC();

  ADC0_ACTSS &= ~0x1; // Disable Sequencer 0

  ADC0_PC = 0x1; // Set sampling rate
  ADC0_EMUX  = 0; // Clear to set as software start
  ADC_SSMUX0 = 0; // Set to pin AIN0
  ADC_SSCTL0 &= ~0x9; // Clear 0, 3 (No differential Sampling, No Temperature)
  ADC_SSCTL0 |= 0x1 << 2;  // Set End of Sequence (Bit 2)

  ADC0_ACTSS |= 0x1; // Reenable Sequencer 0
}

long int GetJoystickValue() {
  ADC0_PSSI |= 0x1; // Initiate Sequencer 1
  while((ADC0_RIS & 0x1) == 0x0); // Wait for conversion to be completed
  long int value = ADC0_SSFIFO;
  ADC0_ISC &= ~0x1;
  
  value = interpolate(0, 4090, 0, 10, value) * 10;
  
  return value;
}

bool ButtonPushed() {
  if ((GPIOA_DATA & (0x1 << 6)) != 0x1 << 6) {
    return true;
  }
  return false;
}

int interpolate(int minSource, int maxSource, int minTarget, int maxTarget, int valueSource) {
  int rangeSource = (maxSource - minSource);
  int rangeTarget = (maxTarget - minTarget);
  int valueTarget =
      (((valueSource - minSource) * rangeTarget) / rangeSource) + minTarget;
  return valueTarget;
}

void SetLCDColour(int r, int g, int b) {
  TIMER1_TAMATCH = interpolate(0, 100, 0, 0xFFF0, g);
  TIMER1_TBMATCH = interpolate(0, 100, 0, 0xFFF0, b);
  TIMER2_TAMATCH = interpolate(0, 100, 0, 0xFFF0, r);
}
