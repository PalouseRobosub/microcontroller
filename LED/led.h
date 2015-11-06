#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "System.h"
#include "Queue.h"
#include "packetizer.h"
#include "SPI.h"
#include "UART.h"

#define NUMLEDS 16
#define STRIPSIZE 8

typedef struct Pixel
{
	uint8 green, red, blue;
} PIXEL;

void parse_packet(uint8 *buffer, uint8 size);
void initialize_pins();

void init_SPI(SPI_Config spi_config, uint8 *spi_tx_buf);
void init_UART(UART_Config uart_config, uint8 *uart_rx_buffer);
void init_packet(Packetizer_Config packet_config, UART_Config uart_config);

#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

