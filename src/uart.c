#include"init.h"
#include"uart.h"

/********************************************************************************************
* Function Name     : INIT_UART                                         					*
* Description       : This Function will configure and initialize UART						*
* Parameters        : None                                               					*
* Return Value      : void.    																*
********************************************************************************************/
void INIT_UART(void)
{
	__builtin_write_OSCCONL(OSCCON &  ~(1<<6)); //disabling oscillator
	// Configure Input Functions, Assign U1RX To Pin RP0

	// RPINR18bits.U1RXR =10;	// unsed pin RX.

	// Configure Output Functions, Assign U1TX To Pin RP2
	_RP17R  = 3; //TX	
	__builtin_write_OSCCONL(OSCCON &  (1<<6));  // enabling oscillator
	
	U1BRG				= __UBRG;				// set baud speed by prescaling (see formula in data sheet)
												// using BRGH = 0 Fcy = Fosc/2.

	U1MODEbits.UARTEN	= 1;					// Enable UART.
	U1MODEbits.USIDL	= 0;					// Continues module operation in Idle mode
	U1MODEbits.IREN		= 0;					// IrDA encoder and decoder are disabled
	U1MODEbits.RTSMD	= 0;					// UxRTS pin is in Flow Control mode
	U1MODEbits.UEN		= 0b00;					// 0x00 - UxTX and UxRX pins are enabled and used
	U1MODEbits.WAKE		= 0;					// No wake-up is enabled
	U1MODEbits.LPBACK	= 0;					// Loopback mode is disabled
	U1MODEbits.ABAUD	= 0;					// Baud rate measurement is disabled
	U1MODEbits.RXINV	= 0;					// UxRX Idle state is ‘1’
	U1MODEbits.BRGH		= 0;					// Standard Speed mode (16 BRG clock cycles per bit)
	U1MODEbits.PDSEL	= 0;					// 8-bit data, no parity
	U1MODEbits.STSEL	= 0;					// One Stop bit

	U1STAbits.UTXISEL1	= 1;					// Interrupt when a character is transferred to the Transmit Shift Register (TSR), and as a result, transmit buffer becomes empty
	U1STAbits.UTXISEL0	= 0;
	U1STAbits.UTXINV	= 0;
	U1STAbits.UTXBRK	= 0;					// Sync Break transmission is disabled
	U1STAbits.UTXEN		= 1;					// Transmit is enabled, UxTX pin is controlled by UARTx
	U1STAbits.UTXBF		= 0;					// Transmit buffer is not full, at least one more character can be written
	U1STAbits.TRMT		= 0;					// Transmit Shift Register is not empty
	U1STAbits.URXISEL	= 0b00;					// Interrupt is set when any character is received and transferred from the RSR to the receive buffer receive buffer has one or more characters
	U1STAbits.ADDEN		= 0;					// Address Detect mode is disabled
	U1STAbits.RIDLE		= 0;					// Receiver is active
	U1STAbits.PERR		= 0;					// Parity error has not been detected
	U1STAbits.FERR		= 0;					// Framing error has not been detected
	U1STAbits.OERR		= 0;					// Receive buffer has not overflowed
	U1STAbits.URXDA		= 0;					// Receive buffer is empty

	IPC2bits.U1RXIP		= _U1IPL;				// UART interrupt priority level.
	IFS0bits.U1RXIF 	= 0; 					// reset interrupt flag
	IEC0bits.U1TXIE		= 0;					// Interrupt request is not enabled
	
	printf("uart ready\n");
}

/********************************************************************************************
* Function Name     : WriteUART1                                           					*
* Description       : This function writes data into the UxTXREG for that to be Tx'd		*
* Parameters        : None																	*  
* Return Value      : unsigned int                                                 			*
********************************************************************************************/
void WriteUART1(unsigned int data)
{
    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
	    U1TXREG = data & 0xFF;	
}

/********************************************************************************************
* Function Name     : BusyUART1                                           					*
* Description       : This function checks if the UART is busy and flags ready for Tx/Rx.	*
* Parameters        : unsigned int * address of the string buffer to be transmitted			*  
* Return Value      : None                                                 					*
********************************************************************************************/
char BusyUART1(void)
{  
    return(!U1STAbits.TRMT);
}

/********************************************************************************************
* Function Name     : ReadUART1                                           					*
* Description       : This function returns the contents of UxRXREG buffer					*
* Parameters        : None																	*  
* Return Value      : unsigned int                                                 			*
********************************************************************************************/ 
unsigned int ReadUART1(void)
{
    if(U1MODEbits.PDSEL == 3)
        return (U1RXREG);
    else
		return (U1RXREG & 0xFF );
	
}

/********************************************************************************************
* Function Name     : DataRdyUART1                                           				*
* Description       : This function checks URXDA and flags if the UART is ready				*
* Parameters        : None																	*  
* Return Value      : unsigned int                                                 			*
********************************************************************************************/ 
char DataRdyUART1(void)
{
    return(U1STAbits.URXDA);
}

/********************************************************************************************
* Function Name     : CloseUART1                                           					*
* Description       : This function stop and closes the UART functionality					*
* Parameters        : None																	*  
* Return Value      : None                                                 					*
********************************************************************************************/ 
void CloseUART1(void)
{  
    U1MODEbits.UARTEN = 0;
	
    IEC0bits.U1RXIE = 0;
    IEC0bits.U1TXIE = 0;
	
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;
}

/********************************************************************************************
* Function Name     : putsUART1                                           					*
* Description       : This function puts the data string to be transmitted in to the		*
*                     transmit buffer (till NULL character).Operation is done by filling up	*
*					  UxTXREG.																*
* Parameters        : unsigned int * address of the string buffer to be transmitted			*  
* Return Value      : None                                                 					*
********************************************************************************************/
void putsUART1(unsigned int *buffer)
{
    char * temp_ptr = (char *) buffer;

    /* transmit till NULL character is encountered */

    if(U1MODEbits.PDSEL == 3)        /* check if TX is 8bits or 9bits */
    {
        while(*buffer != '\0') 
        {
            while(U1STAbits.UTXBF); /* wait if the buffer is full */
            U1TXREG = *buffer++;    /* transfer data word to TX reg */

		}

    }
    else
    {
        while(*temp_ptr != '\0')
        {
            while(U1STAbits.UTXBF);  /* wait if the buffer is full */
            U1TXREG = *temp_ptr++;   /* transfer data byte to TX reg */
        }
    }

}

/********************************************************************************************
* Function Name     : getsUART1                                   				            *
* Description       : This function gets a string of data of specified length  if available	*
*					  in the UxRXREG buffer into the buffer specified.                      *
* Parameters        : unsigned int length the length expected         				        *
*                     unsigned int *buffer  the received data to be recorded to this array  *
*                     unsigned int uart_data_wait timeout value               				*
* Return Value      : unsigned int number of data bytes yet to be received    				*
********************************************************************************************/
unsigned int getsUART1(unsigned int length,unsigned int *buffer, unsigned int uart_data_wait)
{
    int wait = 0;
    char *temp_ptr = (char *) buffer;

    while(length)                         /* read till length is 0 */
    {
	
        while(!DataRdyUART1())
        {
            if(wait < uart_data_wait)
                wait++ ;                  /*wait for more data */
            else
                return(length);           /*Time out- Return words/bytes to be read */
				
        }
        wait=0;
        if(U1MODEbits.PDSEL == 3)         /* check if TX/RX is 8bits or 9bits */
            *buffer++ = U1RXREG;          /* data word from HW buffer to SW buffer */
	else
            *temp_ptr++ = U1RXREG & 0xFF; /* data byte from HW buffer to SW buffer */

        length--;
    }

    return(length);                       /* number of data yet to be received i.e.,0 */
}

// UART1 transmit ISR
void __attribute__((__interrupt__,no_auto_psv)) _U1TXInterrupt(void) 
{
	IFS0bits.U1TXIF = 0;
}

// UART1 receive ISR
void __attribute__((__interrupt__,no_auto_psv)) _U1RXInterrupt(void) 
{
	IFS0bits.U1RXIF = 0;	
}
