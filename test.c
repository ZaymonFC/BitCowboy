#include "registers.h"
#include "types.h"

/*
 * -------------------------------------------------------------------- I ----------
 *   :::::: T E S T   F U N C T I O N S : :  :   :    :     :        : :
 * ------------------------------------------------------------------------------
 */
 
extern void DelayMs(unsigned int);

extern void LCD_PutPixel(unsigned int, unsigned int, unsigned int);
extern void LCD_Contrast(int, int);
extern void LCD_Refresh(void);
extern void LCD_Blank(void);
extern void LCD_Init(void);
extern void LCD_CLEAR_MAT(void);

extern void SetLCDColour(int r, int g, int b);
extern long int GetJoystickValue(void);
extern bool ButtonPushed(void);
 
void Test_PWM_LCD_LEDCOLOUR() {
  SetLCDColour(100, 100, 100);
  DelayMs(5);
  SetLCDColour(0, 100, 0);
  DelayMs(5);
  SetLCDColour(100, 0, 0);
  DelayMs(5);
  SetLCDColour(0, 0, 100);
  DelayMs(5);
  SetLCDColour(100, 0, 100);
  DelayMs(5);
  SetLCDColour(100, 100, 0);
  DelayMs(5);
  SetLCDColour(75, 0, 30);
  DelayMs(5);
  SetLCDColour(0, 100, 50);
  DelayMs(5);
}

void Test_GPIO_Setup_Button() {
  int pressCount = 0;
  // Green while button Pressed
  while (pressCount < 2) {
    if (ButtonPushed()) {
      SetLCDColour(100, 100, 100);
      pressCount += 1;
      DelayMs(6);
    } else {
      SetLCDColour(100, 0, 0);
    }
  }
}

void Test_ADC_Joystick() {
  while((GPIOA_DATA & (0x1 << 6)) == (0x1 << 6)) {
    long int joystickValue = GetJoystickValue();
    int red = joystickValue;
    SetLCDColour(red, red, red);
  }
}
