#include "registers.h"
void Delay(unsigned int);

void Delay(unsigned int numLoops)
{
volatile unsigned int lp;
volatile unsigned int i;

for(lp=0; lp<numLoops; lp++)
	for (i=0; i<=0xFFFF; i++) ;
}




