#include"init.h"
#include"timer.h"

unsigned char TimerDelay_flag_10ms = 0;
unsigned char ledcount = 0;
/********************************************************************************************
* Function Name     : INIT_TIMER                                         					*
* Description       : This function initializes the Timer1 and raise interrupt with time 	*
* 					  calculated in T1_TIME.  												*
* Parameters        : None                                               					*
* Return Value      : void.    																*
********************************************************************************************/
void  INIT_TIMER(void)
{
	T1CON    		= 0;				//  Ensure Timer 1 is in reset state
	IFS0bits.T1IF  	= 0;				//  Reset Timer 1 interrupt flag
	IPC0bits.T1IP  	= 1;				//  Set Timer1 interrupt priority level to 1
	IEC0bits.T1IE  	= 1;				//  Enable Interrrupt w.r.t Timer 1
	T1CONbits.TCKPS	= 1;				// Set Timer Input Clock Prescale Select bits (1:8)
	T1CONbits.TSIDL = 0;				// Continue Timer operation in Idle mode
	T1CONbits.TCS  	= 0;				//  Enable Internal clock (FOSC/4)
	PR1    			= __T1_TIME;		//  set Timer 1 period register for 10ms
	T1CONbits.TON  	= 1;				//  Enable Timer 1
}

/* Timer Interrupt */
void __attribute__((__interrupt__,no_auto_psv)) _T1Interrupt(void)
{
	IFS0bits.T1IF = 0;					// reset Timer 1 interrupt flag
	TimerDelay_flag_10ms = 1;
	if(++ledcount > 100)
	{
		ledcount = 0;
		LATEbits.LATE3 = !LATEbits.LATE3;
	}
}

/********************************************************************************************
* Function Name     : TimerDelay_10ms                                      					*
* Description       : This function creates a multiple of 10 msec with entered delay	 	*
* Parameters        : unsigned int delay                                  					*
* Return Value      : void.    																*
********************************************************************************************/
void TimerDelay_10ms(unsigned long int delay)
{
	TimerDelay_flag_10ms = 0;
	while(delay)
	{
		if(TimerDelay_flag_10ms == 1)
		{
			TimerDelay_flag_10ms = 0;
			delay--;
		}
	}
}
