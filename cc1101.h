#ifndef _cc1101_t_H_
#define _cc1101_t_H_

#include <inttypes.h>
#include <stdlib.h>

#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

// Holds information for a specific cc1101
typedef struct cc1101 {
    spi_inst_t *spi_if; // Which SPI interface
    // int sck_pin; // Clock pin
    // int tx_pin; // tx (PICO) pin
    // int rx_pin; // rx (CIPO) pin
    int cs_pin; // Chip select pin
} cc1101_t;

// Clock pin, tx (PICO) pin, rx (CIPO) pin, chip select pin
// Must be run before cc1101 can be used
cc1101_t *cc1101_init(int sck, int tx, int rx, int cs) {


// Write a single byte to a single register address.
void write_reg(cc1101_t *cc1101, uint8_t reg_addr, uint8_t *data);

// Write multiple bytes of data, each byte to a consecutive register.
// The register written to will increment after each byte has been written.
void write_burst_reg(cc1101_t *cc1101, uint8_t start_reg_addr, uint8_t **data, size_t data_size);

// Read a single byte from a single register address.
void read_reg(cc1101_t *cc1101, uint8_t reg_addr, uint8_t *output_buffer);

// Read multilple bytes of data, each byte from a consecutive register.
// The register read from will increment after each byte has been read.
 void read_burst_reg(cc1101_t *cc1101, uint8_t start_reg_addr, uint8_t **output_buffer, size_t data_size);

// Get the part number for a particular cc1101  module.
void print_part_num(cc1101_t *cc1101);

#endif
