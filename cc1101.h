#ifndef _CC1101_H_
#define _CC1101_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

/*
 * Struct to hold the SPI interface a CC1101 module is using,
 * as well as which GPIO pin it is using for chip select.
 * It shouldn't be necessary to use anything inside of this
 * struct; only a struct itself will have to be passed to functions.
 */
typedef struct {
    spi_inst_t *spi_bus;
    char cs_pin;
} CC1101;


/*
 * Initialize the CC1101 for operation.
 * sck: GPIO pin number of the clock line for the SPI interface
 * tx: transmit (COPI) pin number
 * rx: receive (COPI) pin number
 * cs: chip select pin number
 * num: the number of 
 * 
 * As of right now, the maximum number of CC1101 modules that can be
 * initialized is 2.
 */
CC1101 *cc1101_init(char sck, char tx, char rx, char cs);


/*
 * Write a single byte to a single register address.
 */
void write_reg(CC1101 *cc1101, uint8_t reg_addr, uint8_t *data);


/*
 * Write multiple bytes of data, each byte to a consecutive register.
 * The register written to will increment after each byte has been written.
 */
void write_burst_reg(CC1101 *cc1101, uint8_t start_reg_addr, uint8_t **data, size_t data_size);


/*
 * Read a single byte from a single register address.
 */
void read_reg(CC1101 *cc1101, uint8_t reg_addr, uint8_t *output_buffer);


/*
 * Read multilple bytes of data, each byte from a consecutive register.
 * The register read from will increment after each byte has been read.
 */
 void read_burst_reg(CC1101 *cc1101, uint8_t start_reg_addr, uint8_t **output_buffer, size_t data_size);


/*
 * Get the part number for a particular CC1101 module.
 */
void print_part_num(CC1101 *cc1101);

#endif
