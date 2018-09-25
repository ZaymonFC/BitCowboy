
void LCD_PutPixel(unsigned int, unsigned int, unsigned int);
void LCD_Contrast(int, int);
void LCD_Refresh(void);
void LCD_Blank(void);
void LCD_Char_Out(unsigned char);
void LCD_Dat_Out(unsigned char);
void LCD_Cmd_Out(unsigned char);
void LCD_Init(void);  
extern void GPIO_Setup_LCD(void);
extern unsigned int getportbases(void);

extern void DelayMs(unsigned int);
extern unsigned int ABits[6];
extern unsigned char APorts[6];
extern unsigned char LCDmat[1056];
extern unsigned int APBases[6];

//#define R(x) (*((volatile unsigned long *)(x)))
//R(module + ADCEMUX) &= ~(HEX0 << (seq_offset * 0x04));

#define GP_DAT 0x3FC
#define R(x) (*((volatile unsigned long *)(x)))

    
void LCD_PutPixel(unsigned int x, unsigned int y, unsigned int col)
{   //R5 = x = 1-128, R6 = y= 1-64  R7= colour
		if (y > 64 || x > 128) return;
    unsigned int divb, rem, matpos, val;
    x = x-1;
    if (y<33)
    {   y = y+32;  }
    else
    {   y = y-32; }
    y = y-1;
    
    divb = y / 8;
    rem = y%8;  // remainder of y/8   // y - byt*8
    matpos = divb*128 + x;
    val = (1<<rem);
    if (col==1)
    {  LCDmat[matpos] = LCDmat[matpos]|val;}
    else
    {  LCDmat[matpos] = LCDmat[matpos]&~val;}
 }

void LCD_Contrast(int ratio, int contr)
{   //R4 = res_ratio (<=7)  R5= contrast (<=63)
    if (ratio>7)
    {  ratio = 7;  }
    if (contr >63)
    {  contr = 63; }
    ratio |= 0x20;
    LCD_Cmd_Out(ratio);
    LCD_Cmd_Out(0x81);
    LCD_Cmd_Out(contr);
}

void LCD_Refresh(void)
{  	//y=R6, x=R5
    unsigned int x,y, comd,ofset;
    unsigned char val;
    for (y=0; y<8; y++)
    {   comd = y | 0xB0;
        LCD_Cmd_Out(comd);
        LCD_Cmd_Out(0);
        LCD_Cmd_Out(0x10);
	
        for (x=0; x<128; x++)
        {   ofset = y*128+x;
            val = LCDmat[ofset];
            LCD_Dat_Out(val);
        }
    }
}

void LCD_Blank(void)
{   for (int i=0; i<1024; i++)
    {  LCDmat[i]=0;  }
    LCD_Refresh();
}
        
void LCD_Char_Out(unsigned char cmd)
{   //APBases,ABits = [CSbit, RSTbit, A0bit, CLKbit, DATbit]
    unsigned int	cnt;
     unsigned char bbb;
    cmd &= 0xFF;
    R(APBases[0] + GP_DAT) &= ~ABits[0];   
        
    for (cnt=0; cnt<8; cnt++) 
    {   bbb = cmd & 0x80;
        if (bbb==0)
        {  R(APBases[4] + GP_DAT) &= ~ABits[4];  }
        else
        {  R(APBases[4] + GP_DAT) |= ABits[4];  }
        
        R(APBases[3] + GP_DAT) |= ABits[3];     //pulse clock
        R(APBases[3] + GP_DAT) &= ~ABits[3];      
 
        cmd = cmd<<1;   
    }
    R(APBases[0] + GP_DAT) |= ABits[0]; 
}


void LCD_Dat_Out(unsigned char cmd)
{   //APBases,ABits = [CSbit, RSTbit, A0bit, CLKbit, DATbit]
    cmd &= 0xFF;
    R(APBases[2] + GP_DAT) |= ABits[2];    //A0write(0)
    LCD_Char_Out(cmd);
}


void LCD_Cmd_Out(unsigned char cmd)
{   //APBases,ABits = [CSbit, RSTbit, A0bit, CLKbit, DATbit]
	  cmd &= 0xFF;
    R(APBases[2] + GP_DAT) &= ~ABits[2];    //A0write(0)
    LCD_Char_Out(cmd);
}


void LCD_Init(void)  
{  	//APBases,ABits = [CSbit, RSTbit, A0bit, CLKbit, DATbit]
	   unsigned char aaa;
	   R(APBases[0] + GP_DAT) &= ~ABits[0];
	   R(APBases[1] + GP_DAT) &= ~ABits[1];  
	   DelayMs(70);
     R(APBases[1] + GP_DAT) |= ABits[1];  
     DelayMs(70);
	   
     LCD_Cmd_Out(0xA2);   //1/9th bias
     LCD_Cmd_Out(0xA0);
     LCD_Cmd_Out(0xC0);
     LCD_Cmd_Out(0xC8);
     aaa = 0x20 | 0x3;
     LCD_Cmd_Out(aaa);
     aaa = 0x28 | 0x4;
     LCD_Cmd_Out(aaa);
     DelayMs(70);
     aaa = 0x28 | 0x6;
     LCD_Cmd_Out(aaa);
     DelayMs(70);
     aaa = 0x28 | 0x7;
     LCD_Cmd_Out(aaa);
     DelayMs(70);
     LCD_Cmd_Out(0x26);
     LCD_Cmd_Out(0x81);
     LCD_Cmd_Out(31);
     LCD_Cmd_Out(0x40);
     LCD_Cmd_Out(0xAF);
     R(APBases[0] + GP_DAT) |= ABits[0];
     LCD_Contrast(5,50);  // inputs: ratio, contrast
     LCD_Cmd_Out(0xF0);
}
