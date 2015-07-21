#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="includes">
#include "System.h"
#include "Queue.h"
#include "packetizer.h"
#include "SPI.h"
#include "UART.h"
#include "ledCommon.h"
// </editor-fold>

void parse_packet(uint8 *buffer, uint8 size);
void initialize_pins();
void init_SPI(SPI_Config spi_config, uint8 *spi_tx_buf);
void init_UART(UART_Config uart_config, uint8 *uart_rx_buffer);
void init_packet(Packetizer_Config packet_config, UART_Config uart_config);

void copy_strip(PIXEL *pixels, PIXEL *colors);
void set_all(PIXEL *pixels, PIXEL *color);
void set_strip(PIXEL *pixels, PIXEL *color);
void set_dual_all(PIXEL *pixels, PIXEL *color1, PIXEL *color2);
void set_dual_strip(PIXEL *pixels, PIXEL *color1, PIXEL *color2);


#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

