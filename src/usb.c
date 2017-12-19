#include"init.h"


void INIT_USB(void)
{
	/* Initializing USB to Host Mode */
	U1CON			 		= 0;		// 
	U1CONbits.HOSTEN		= 1;		// USB host capability is enabled; pull-downs on D+ and D- are activated in hardware
	U1CONbits.SOFEN			= 0;		// Start-of-Frame token is disabled

	U1OTGCONbits.DMPULUP	= 0;
	U1OTGCONbits.DMPULUP	= 0;
	U1OTGCONbits.DMPULDWN	= 1;
	U1OTGCONbits.DPPULDWN	= 1;		// Enable D+ and D-
	
	U1IEbits.ATTACHIE		= 1;		// Peripheral Attach Interrupt bit is enabled	
}
