/*****************************************************************************
*
* File: delays.c
* 
* Copyright S. Brennen Ball, 2006-2007
* 
* The author provides no guarantees, warantees, or promises, implied or
*	otherwise.  By using this software you agree to indemnify the author
* 	of any damages incurred by using it.
* 
*****************************************************************************/

//This code depends on a clock frequency of 40 MHz.

#include "delays.h"

void DelayUS(unsigned long microseconds)
{
  int j;
  for (j = 0; j < 40*microseconds; j++) { // number is 1 million
  	// do nothing
    }
}

void DelayMS(unsigned long milliseconds)
{
	unsigned long count;
	
	for(count = 0; count < milliseconds; count++)
		DelayUS(1000);// 1000 us = 1 ms
}

void DelayS(unsigned long seconds)
{
	unsigned long count;
	
	for(count = 0; count < seconds; count++)
	{
		DelayMS(1000);// 1 s = 1000 ms
	}
}
