#ifndef __INIT_H
	#define __INIT_H

	#include<p24FJ128GC006.h>
	#include<string.h>
	#include<ctype.h>
	#include<stdlib.h>
	#include<stdio.h>
	#include<math.h>

	/********************************Setting*************************************************
	* Fosc(Internal FRC) 			: 8 Mhz													*
	* Fcy = (Fosc/2)*PLL			based on BRGH = 0 										*
	* Fcy = (Fosc/4)*PLL					 BRGH = 1										*
	****************************************************************************************/
	
	#define	__UBRG		25		// U1baud rate
	#define _U1IPL		1		// Interrupt Priority Level.

	#define	__T1_TIME	10000	
#endif
