#include"init.h"
#include"SD_adc.h"

#define offsetval		15252
#define NO_OF_SAMPLES	128
#define gain			(0.050476) // (0.050476*1.278344)
signed int adc_temp1 = 0;
unsigned long int calval1 = 0;
float adcbufval_13 = 0;
float resultadc = 0;

static signed long int offset;
static double SD_gain;
signed long int CalibratedSD = 0;
unsigned int loop = 0;

void INIT_ADC(void)
{
	 unsigned int i;
	 signed long int maxValue;
	 #define EXPECTED_MAX_VALUE (double) 32767 // 16 bit adc
 
	SD1CON1 			= 0;
	SD1CON2 			= 0;
	SD1CON3 			= 0; // Reset Adc.
	
	SD1CON1bits.PWRLVL 	= 1; // High power mode
	SD1CON1bits.SDREFP 	= 0; // Positive Voltage Reference is SVDD
	SD1CON1bits.SDREFN 	= 0; // Negative Voltage Reference is SVSS
	SD1CON1bits.SDOFFCAL= 1; // Internal Offset Measurement 1 - Enable, 0 - Disable
	SD1CON1bits.DITHER 	= 1; // 1 - Low Dither, because using SVDD as reference
	SD1CON1bits.SDGAIN 	= 0b000; // Gain is 0x101 - 32,0x100 - 16, 0x011 - 8, 0x010 - 4, 0x001 - 2, 0x000 - 1.
	SD1CON1bits.SDRST 	= 0; // Release from Reset
	
	SD1CON2bits.CHOPSEL = 3; // Chopping should be enabled always, 3 - en, 0 - dis
	SD1CON2bits.SDINT 	= 3; // Interrupt on every data clk - 3, on every 5 data clk - 2
	SD1CON2bits.SDWM 	= 1; // SDxRESH/SDxRESL updated on every Interrupt
	SD1CON2bits.RNDRES 	= 2; // Round result to 16-bit
	
	SD1CON3bits.SDDIV 	= 2; // Input Clock Divider is 4MHz (SD ADC clock is 8MHz)
	SD1CON3bits.SDOSR 	= 0; // Oversampling Ratio (OSR) is 1024
	SD1CON3bits.SDCS 	= 1; // Clock Source is 8MHz FRC
	SD1CON3bits.SDCH 	= 0; // 0 - Ch0+ , 1 - Ch1+, 2 - CH1SE.
	SD1CON1bits.SDON 	= 1; // Enable the ADC module now that it is configured

	// Offset calculation.
	for(i = 0; i<6; i++) // (value must be >= 5)
	{
	    IFS6bits.SDA1IF = 0; //Clear interrupt flag
	    while(IFS6bits.SDA1IF == 0);//Wait until hardware says we have a result ready.
	    IFS6bits.SDA1IF = 0; //Clear interrupt flag
	}
	while(IFS6bits.SDA1IF == 0); //Wait until hardware says we have a result ready.
	IFS6bits.SDA1IF = 0; //Clear interrupt flag
	offset = (signed short int)SD1RESH; // cast to accomodate the sign bit extend
	// SD_gain Calculation
	SD1CON1bits.SDON = 0; // Disable ADC
	SD1CON1bits.SDOFFCAL= 0;
	SD1CON3bits.SDCH = 3;
	SD1CON1bits.SDON = 1; // Enable ADC
	
	for(i = 0; i<6; i++) // (value must be >= 5)
	{
	    IFS6bits.SDA1IF = 0; //Clear interrupt flag
	    while(IFS6bits.SDA1IF == 0);//Wait until hardware says we have a result ready.
		IFS6bits.SDA1IF = 0; //Clear interrupt flag
	}
	while(IFS6bits.SDA1IF == 0); //Wait until hardware says we have a result ready.
	IFS6bits.SDA1IF = 0; //Clear interrupt flag
	maxValue = (signed short int)SD1RESH; // cast to accomodate the sign bit extend
	SD_gain = EXPECTED_MAX_VALUE/((double)((signed long int)maxValue-offset));

	SD1CON1bits.SDON 	= 0; // Disable ADC
	SD1CON1bits.SDOFFCAL= 0;
	SD1CON3bits.SDCH 	= 0;
	SD1CON1bits.SDON 	= 1; // Enable ADC
}

double ADC_SD_GetRawResult(void)
{
	IFS6bits.SDA1IF = 0; //Clear interrupt flag
	while(IFS6bits.SDA1IF == 0); //Wait until hardware says we have a result ready.
	SD1CON2bits.SDRESRDY = 0;
	adc_temp1	= (SD1RESH - offsetval);
	calval1    += pow(abs(adc_temp1),2);
	if(++loop >(NO_OF_SAMPLES-1))
	{
		adcbufval_13 = sqrt(calval1/NO_OF_SAMPLES);
		calval1 = 0, loop = 0;
		resultadc = (float)(adcbufval_13*gain);
//		if(resultadc > 0.040)
			printf(" %4.3f\n",resultadc);
	}
	return 0;
}
