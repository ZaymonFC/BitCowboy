#include "registers.h"
#include "types.h"

extern void DelayMs(unsigned int);
extern void DisableInterrupts(void);
extern void EnableInterrupts(void);

extern void LCD_PutPixel(unsigned int, unsigned int, unsigned int);
extern void LCD_Contrast(int, int);
extern void LCD_Refresh(void);
extern void LCD_Blank(void);
extern void LCD_Init(void);
extern void GPIO_Setup_LCD(void);
extern void LCD_CLEAR_MAT(void);
extern void GPIO_Setup_Button(void);

extern void SetupRGB_PWM(void);
extern void SetLCDColour(int r, int g, int b);

extern void ADC_Setup_Joystick(void);
extern long int GetJoystickValue(void);
extern bool ButtonPushed(void);
extern int interpolate(int, int, int, int, int);


unsigned char LCDmat[1056];
unsigned int ABits[6] = {
    0x01, 0x02, 0x04,
    0x08, 0x10, 0x20}; // [CSbit, RSTbit, A0bit, CLKbit, DATbit,B-]
unsigned char APorts[6] = {
    'B', 'B', 'B',
    'B', 'B', 'B'}; // [CSport, RSTport, A0port, CLKport, DATport,B-]
unsigned int APBases[6] = {0x40005000, 0x40005000, 0x40005000,
                           0x40005000, 0x40005000, 0x40005000};

// Type Definitions and #Defines
typedef struct Map {
  int width;
  int height;
  char tiles[128];
} Map;

char tiles[16][256] = {
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, 
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0 }, 
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,1,0,1,0,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, 
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0 }, 
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0 }, 
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,1,0,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,1,1,1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,1,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, 
};

char sprite[3][108] = {
    {0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0}
};

char alphabet[26][35] = {
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, 
    { 0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1 }, 
    { 1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1 }, 
    { 1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1 }, 
    { 1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0 }, 
    { 1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1 }, 
    { 1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0 }, 
    { 1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,0,0,0,1,1,1,1,1,1 }, 
    { 1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1 }, 
    { 1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,1 }, 
    { 0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1 }, 
    { 1,0,0,0,1,1,0,0,1,0,1,0,1,1,0,1,1,1,0,0,1,1,1,0,0,1,0,0,1,0,1,0,0,0,1 }, 
    { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1 }, 
    { 1,1,0,1,1,1,1,1,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1 }, 
    { 1,0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,0,0,1,1,1,0,0,0,1 }, 
    { 0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0 }, 
    { 1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0 }, 
    { 1,1,1,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,1,1,1,1,0,0,0,0,1 }, 
    { 1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,1,0,0,1,0,0,1,0,1,0,0,0,1 }, 
    { 1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1 }, 
    { 1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0 }, 
    { 1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1 }, 
    { 1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0 }, 
    { 1,0,1,0,1,1,0,1,0,1,1,0,1,0,1,0,1,1,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0 }, 
    { 1,0,0,0,1,0,1,0,1,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,1,0,1,0,1,0,0,0,1 }, 
    { 1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0 }, 
};

const int SCREEN_TILES_WIDTH = 8;
const int SCREEN_TILES_HEIGHT = 4;
const int SPRITE_WIDTH = 9;
const int SPRITE_HEIGHT = 12;

bool DEATH = true;
int DEATH_TILE = 16;
typedef enum { idle, l, r } Orientation;

/*
 * ─── FORWARD DECLARATIONS
 * ───────────────────────────────────────────────────────
 */
void DrawMap(Position *viewPort, struct Map *map, const int MAP_TILES);
void DrawTile(int tileNumber, int x, int y);
void DrawPlayer(Position *playerPosition, Position *viewPort, Orientation);
void CalculateViewPortCoordinates(Position *viewPort, Position *playerPosition);
void CalculateCollisionDirection(Position *, Map *, CollisionInfo *);
void DrawText(int, int, char*, int);
void EndGame(void);

// Test Functions
void Test_PWM_LCD_LEDCOLOUR(void);
void Test_GPIO_Setup_Button(void);
void Test_ADC_Joystick(void);

int main(void) {
  Map map = {
    32,
    4,
    {1, 9, 8, 9, 9, 8, 8, 3, 9, 3, 9, 9, 8, 8, 8, 9, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 11, 2, 1, 3, 3, 3, 3, 3, 11, 9, 9, 9, 3, 3, 9, 9, 8, 3, 3, 3, 3, 12, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 10, 2, 11, 3, 3, 3, 1, 10, 11, 1, 1, 1, 3, 3, 3, 3, 12, 3, 3, 13, 11, 6, 5, 7, 9, 5, 3, 3, 3, 3, 3, 3, 10, 2, 14, 7, 5, 7, 13, 2, 2, 15, 2, 2, 14, 7, 7, 13, 11, 3, 10, 2, 11, 6, 8, 9, 8, 8, 7, 8, 5, 13, 2, 2, 2 }
  };

  Map collisionMap = {
    32,
    4,
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 0, 0, 2, 2, 0, 2, 2, 0, 2, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 2, 2, 2, 0, 0, 0, 0, 0, 2, 0, 2, 2, 2, 2, 2 }
  };

  const int MAP_TILES = map.height * map.width;

  // --- Setup and testing --------------------------------------------------
  SetupRGB_PWM();
  // Test_PWM_LCD_LEDCOLOUR();

  GPIO_Setup_Button();
  // Test_GPIO_Setup_Button();

  ADC_Setup_Joystick();
  // Test_ADC_Joystick();

  SetLCDColour(10, 10, 10);

  GPIO_Setup_LCD();

  LCD_Init();
  LCD_Blank();

  // --- `Game` Loop --------------------------------------------------------
  // Stock Variables
  Position playerPosition = { 80, 15 };
  Position viewPort = { 0, 0 };
  Orientation orientation = idle;
  CollisionInfo collisionInformation = { false, false, false };

  int playerSpeed = 2;
  int jumpSpeed = 6;
  int gravity = 6;

  int xVelocity = 0;
  int yVelocity = 0;
  bool inAir = true;

  SetLCDColour(0, 100, 30);
  
  // Colour Values
//  int lRed = 20;
//  int lGreen = 40;
//  int lBlue = 80;
//  
//  int rRed = 100;
//  int rGreen = 50;
//  int rBlue = 20;

  while (1) {
    // Pre-Update
    yVelocity += 1;
    if (yVelocity >= gravity) yVelocity = gravity;

    // Handle Inputs
    if (GetJoystickValue() < 40) {
      xVelocity = -playerSpeed;
      orientation = r;
    } else if (GetJoystickValue() > 60) {
      xVelocity = playerSpeed;
      orientation = l;
    } else {
      xVelocity = 0;
      orientation = idle;
    }

    if (ButtonPushed() && !inAir) {
      yVelocity = -jumpSpeed;
      inAir = true;
    }
    
    playerPosition.y += yVelocity;
    
    // Collisions
    CalculateCollisionDirection(&playerPosition, &collisionMap, &collisionInformation);
    if(DEATH) {
      EndGame();
    }

    if (collisionInformation.right) {
      xVelocity = 0;
      orientation = idle;
      playerPosition.x -= playerSpeed;
    }
    if (collisionInformation.left) {
      xVelocity = 0;
      orientation = idle;
      playerPosition.x += playerSpeed;
    } 
    if (collisionInformation.bottom) {
      yVelocity = 0;
      // Place the player on the ground
      playerPosition.y -= playerPosition.y % 16 - 5;
      inAir = false;
    }
    
    // Update Coords
    playerPosition.x += xVelocity;


    // --- Render -------------------------------------------------------------
    LCD_CLEAR_MAT();

    // Elements
    CalculateViewPortCoordinates(&viewPort, &playerPosition);
    DrawMap(&viewPort, &map, MAP_TILES);
    DrawPlayer(&playerPosition, &viewPort, orientation);
    DrawText(2, 2, "tivagame", 8);
    
    // Refresh
    LCD_Refresh();

    // --- Colour Management --------------------------------------------------
//    int mapWidth = map.width * 16;
//    int red = interpolate(0, mapWidth, lRed, rRed, playerPosition.x);
//    int green = interpolate(0, mapWidth, lGreen, rGreen, playerPosition.x);
//    int blue = interpolate(0, mapWidth, lBlue, rBlue, playerPosition.x);
//    SetLCDColour(red, green, blue);
  }
}

void EndGame() {
  LCD_CLEAR_MAT();

  for (int i = 0; i < SCREEN_TILES_HEIGHT; i++) {
    for (int j = 0; j < SCREEN_TILES_WIDTH; j++) {
      int x = j * 16;
      int y = i * 16;
      if (i > 0 && i < 3) {
        if (j > 1 && j < 6) {
          continue;
        }
      }
      DrawTile(DEATH_TILE, x, y);
      LCD_Refresh();
      DelayMs(2);
    }
  }
  DrawText(41, 29, "gameover", 8);
  LCD_Refresh();
  DelayMs(50);
}

void CalculateViewPortCoordinates(Position *viewPort, Position *playerPosition) {
  viewPort->x = playerPosition->x - 61;
  // viewPort->y = playerPosition->y - 32;
  viewPort->y = 0;
}

void DrawPlayer(Position *playerPosition, Position *viewPort, Orientation orientation) {
  int screenX = playerPosition->x - viewPort->x;
  int screenY = playerPosition->y - viewPort->y;

  for (int x = 0; x < SPRITE_WIDTH; x++) {
    for (int y = 0; y < SPRITE_HEIGHT; y++) {
      int pixelValue = sprite[orientation][x + (y * SPRITE_WIDTH)];
      if (pixelValue)
        LCD_PutPixel(screenX + x, screenY + y, pixelValue);
    }
  }
}

void DrawMap(Position *viewPort, struct Map *map, const int MAP_TILES) {
  // 'Intelligently' determine the range of tiles to draw and the offset for
  // each tile. Calculate the tile coords of the top left tile on the screen
  int xOffset = viewPort->x;
  int yOffset = viewPort->y;

  int tileX = xOffset / 16;
  int tileY = yOffset / 16;

  // Calculate the per tile offset
  int tileOffsetX = xOffset % 16;
  int tileOffsetY = yOffset % 16;
  
  for (int row = 0; row < SCREEN_TILES_HEIGHT + 1; row++) {
    for (int col = 0; col < SCREEN_TILES_WIDTH + 1; col++) {
      int x = col * 16 - tileOffsetX;
      int y = row * 16 - tileOffsetY;

      // Calculate tile type based on tile position
      int tile = (col + tileX) + ((row + tileY) * map->width);

      // Sanity Check the Tile (Allow viewport over map boundary)
      if (tile < 0 || tile >= MAP_TILES) {
        continue;
      }
      if (tileX + col < 0 || tileX + col >= map->width) {
        continue;
      }
      if (tileY + row < 0 || tileY + row >= map->height) {
        continue;
      }

      // Draw the tile at screen offset x, y
      // LCD_PutPixel(tile, 10, 1);
      DrawTile(map->tiles[tile], x, y);
    }
  }
}

void DrawTile(int tileNumber, int xPos, int yPos) {
  if (tileNumber == 3)
    return;
  for (int i = 0; i < 256; i++) {
    int x = i % 16 + 1 + xPos;
    int y = (i / 16) + 1 + yPos;
    LCD_PutPixel(x, y, tiles[tileNumber - 1][i]);
  }
}

void DrawLetter(int x, int y, int letter) {
  for (int r = 0; r < 7; r++) {
    for (int c = 0; c < 5; c++) {
      int tile = c + r * 5;
      LCD_PutPixel(x + c, y + r, alphabet[letter][tile]);
    }
  }  
}

void DrawText(int x, int y, char * text, int length) {
  int textX = x;
  for (int i = 0; i < length; i++) {
    int letter = (int)text[i] - 0x60;
    DrawLetter(textX, y, letter);
    textX += 6;
  }
}

bool CollisionTile(int x, int y, Map * collisionMap) {
  int tile = x + (y * collisionMap->width);
  
  if (tile < 0 || tile >= (collisionMap->width * collisionMap->height)) return false;
  if (collisionMap->tiles[tile] == 2) return true;
  return false;
}

void CalculateCollisionDirection(Position* playerPosition, Map* collisionMap, CollisionInfo * CI) {
  // Clear Collision Information
  CI->right = false; CI->left = false; CI->bottom = false;

  int tileX = playerPosition->x / 16;
  int tileY = playerPosition->y / 16;
  
  int playerLeft = playerPosition->x;
  int playerRight = playerPosition->x + SPRITE_WIDTH;
  int playerBottom = playerPosition->y + SPRITE_HEIGHT;

  int tileLeftX = tileX * 16 + 16;
  int tileRightX = tileX * 16 + 16;
  int tileBottomY = tileY * 16 + 16;
  
  if (playerBottom > tileBottomY && CollisionTile(tileX, tileY + 1, collisionMap))
    CI->bottom = true;
  
  // Might need to check tile to the side and bottom diagonal if player is over
  // the lower tile boundary
  if (playerBottom > tileBottomY + 2) {
    // Bottom Left Diagonal
    if (playerLeft < tileLeftX && CollisionTile(tileX, tileY + 1, collisionMap))
      CI->left = true;
    
    // Bottom Right Diagonal
    if (playerRight > tileRightX + 1 && CollisionTile(tileX + 1, tileY + 1, collisionMap))
      CI->right = true;
  }

  // Tile to the left
  if (playerLeft < tileLeftX && CollisionTile(tileX , tileY, collisionMap))
    CI->left = true;

  // Tile to the right
  if (playerRight > tileRightX && CollisionTile(tileX + 1, tileY, collisionMap))
    CI->right = true;
}

/*
 * ──────────────────────────────────────────────────────────────────── I ──────────
 *   :::::: T E S T   F U N C T I O N S : :  :   :    :     :        : :
 * ──────────────────────────────────────────────────────────────────────────────
 */
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
