#include"init.h"
#include"osc_config.h"
#include"timer.h"
#include"SD_adc.h"
#include"uart.h"
#include"usb.h"

//_CONFIG1(WDTPS_PS32768 & FWPSA_PR128 & WINDIS_OFF & FWDTEN_WDT_DIS & ICS_PGx1 & LPCFG_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF)  
//_CONFIG2(POSCMD_NONE & WDTCLK_LPRC & OSCIOFCN_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & ALTCVREF_CVREF_RA & WDTCMX_LPRC & IESO_OFF)
//_CONFIG3(WPFP_WPFP0 & SOSCSEL_ON & WDTWIN_PS25_0 & BOREN_OFF & WPDIS_WPDIS & WPCFG_WPCFGDIS )
//_CONFIG4(PLLDIV_DIV12);

int main(void)
{
	
	INIT_OSC();
	INIT_TIMER();
	INIT_UART(); 
	INIT_ADC();
	INIT_USB();
	TRISEbits.TRISE3	= 0;
	while(1)
	{
		
		ADC_SD_GetRawResult();
	}
	return 0;
}
