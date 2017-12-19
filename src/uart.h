#ifndef __UART_H
	#define __UART_H

	void INIT_UART(void);
	void WriteUART1(unsigned int data);
	char BusyUART1(void);
	unsigned int ReadUART1(void);
	char DataRdyUART1(void);
	void CloseUART1(void);
	void putsUART1(unsigned int* buffer);
	unsigned int getsUART1(unsigned int length,unsigned int *buffer, unsigned int uart_data_wait);

#endif
