#include "registers.h"
extern void DelayMs(unsigned int);
extern void DisableInterrupts(void);
extern void EnableInterrupts(void);

extern void LCD_PutPixel(unsigned int, unsigned int, unsigned int);
extern void LCD_Contrast(int, int);
extern void LCD_Refresh(void);
extern void LCD_Blank(void);
extern void LCD_Init(void);  
extern void GPIO_Setup_LCD(void);

unsigned char LCDmat[1056];
unsigned int ABits[6] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20};   //[CSbit, RSTbit, A0bit, CLKbit, DATbit,B-]
unsigned char APorts[6] = {'B','B','B','B','B','B'};          //[CSport, RSTport, A0port, CLKport, DATport,B-]
unsigned int APBases[6]={0x40005000, 0x40005000, 0x40005000, 0x40005000, 0x40005000, 0x40005000};

int main(void)
{	  
    //unsigned int a=0;
    //unsigned int b=0;
    unsigned int xvalue, yvalue;
    
	
		GPIO_Setup_LCD ();
 	    LCD_Init();
	  LCD_Blank();
	   
	  xvalue = 60;
	  yvalue = 25; 
	  
         
		
	// flash block on LCD twice	 
		 DelayMs(1000);
		 LCD_PutPixel(20,20,1);   //draw block
		 LCD_PutPixel(21,20,1);
		 LCD_PutPixel(20,21,1);
		 LCD_PutPixel(21,21,1);
		 LCD_Refresh();
		 DelayMs(1000);         
		 LCD_Blank();
		 DelayMs(1000);
		 LCD_PutPixel(20,20,1);    //draw block
		 LCD_PutPixel(21,20,1);
		 LCD_PutPixel(20,21,1);
		 LCD_PutPixel(21,21,1);  
     LCD_Refresh();		 
		 DelayMs(1000);         
		 LCD_Blank();
		 



while(1);
}

