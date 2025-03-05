#include "cc1101.h"


// Binary configuration for writing to cc1101 registers
#define WRITE_SINGLE 0b00111111
#define WRITE_BURST 0b01111111
#define READ_SINGLE 0b10111111
#define READ_BURST 0b11000000

// Command strobe addresses
#define RESET 0x30
#define SRX 0x34
#define STX 0x35
#define SIDLE 0x36

// FIFO addresses
#define SINGLE_BYTE_TX_FIFO 0x3f
#define BURST_ACCESS_TX_FIFO 0x7f
#define SINGLE_BYTE_RX_FIFO 0xbf
#define BUSRT_ACCESS_RX_FIFO 0xff

// Status register addresses
#define PARTNUM_ADDR 0x30
#define VERSION_ADDR 0x31

// Keeps track of how many CC1101s have been initialized so that 
// each one will have a unique spi interface
static int cc1101_number = 0;

CC1101 *cc1101_init(char sck, char tx, char rx, char cs) {

    CC1101 *cc1101 = malloc(sizeof(CC1101));
    if (cc1101_number == 0) {
        cc1101->spi_bus = spi0;
        cc1101_number = 1;
    } else if (cc1101_number == 1) {
        cc1101->spi_bus = spi1;
        cc1101_number = 2;
    } else {
        return NULL;
    }

    spi_init(cc1101->spi_bus, 6250000);
    
    // SPI interface, 8 data bits per transfer, CPOL
    // polarity and CPHA phase both 0, must be SPI_MSB_FIRST
    spi_set_format(cc1101->spi_bus, 8, 0, 0, SPI_MSB_FIRST);

    // Initialize the other SPI pins
    gpio_set_function(sck, GPIO_FUNC_SPI);
    gpio_set_function(tx, GPIO_FUNC_SPI);
    gpio_set_function(rx, GPIO_FUNC_SPI);

    // Set the CS pin high
    cc1101->cs_pin = cs;
    gpio_init(cc1101->cs_pin);
    gpio_set_dir(cc1101->cs_pin, GPIO_OUT);
    gpio_put(cc1101->cs_pin, 1);

    // Check that everything was set up properly
    if (cc1101 == NULL) {
        printf("Failed to allocate memory for cc1101\n");
        exit(-1);
    }

    if (cc1101->spi_bus == NULL) {
        printf("Failed to set cc1101 spi bus\n");
        exit(-1);
    }

    return cc1101;
}


void write_reg(CC1101 *cc1101, uint8_t reg_addr, uint8_t *data) {

    // Combine register address, write bit, and burst bit to form header byte
    uint8_t header_byte = reg_addr | WRITE_SINGLE;

    // Pull CS low
    gpio_put(cc1101->cs_pin, 0);

    // Write the header byte and the data byte
    spi_write_blocking(cc1101->spi_bus, &header_byte, 1);
    spi_write_blocking(cc1101->spi_bus, data, 1);

    gpio_put(cc1101->cs_pin, 1);
}


void write_burst_reg(CC1101 *cc1101, uint8_t start_reg_addr, uint8_t **data, size_t data_size) {

    uint8_t header_byte;
    header_byte = start_reg_addr | WRITE_BURST;

    gpio_put(cc1101->cs_pin, 0);

    // write single header byte
    spi_write_blocking(cc1101->spi_bus, &header_byte, 1);
    // write consecutive data bytes without any header bytes
    for (size_t i = 0; i < data_size; i++) {
        spi_write_blocking(cc1101->spi_bus, data[i], 1);
    }

    gpio_put(cc1101->cs_pin, 1);
}


void read_reg(CC1101 *cc1101, uint8_t reg_addr, uint8_t *output_buffer) {
    
    uint8_t header_byte = reg_addr | READ_SINGLE;
    gpio_put(cc1101->cs_pin, 0);
    spi_write_blocking(cc1101->spi_bus, &header_byte, 1);
    // 0 (second argument) is data repeatedly output on TX while reading from RX
    spi_read_blocking(cc1101->spi_bus, 1, output_buffer, 1);
    //printf("Read response %s\n", output_buffer);
    gpio_put(cc1101->cs_pin, 1);
}


void read_burst_reg(CC1101 *cc1101, uint8_t start_reg_addr, uint8_t **output_buffer, size_t data_size) {
    
    uint8_t header_byte;
    uint8_t current_reg_addr = start_reg_addr;

    gpio_put(cc1101->cs_pin, 0);

    for(size_t i = 0; i < data_size; i++) {
        // Set the header byte
        header_byte = current_reg_addr | READ_BURST;
        // Write the header byte
        spi_write_blocking(cc1101->spi_bus, &header_byte, 1);
        // Read data
        read_reg(cc1101, header_byte, output_buffer[i]);
        current_reg_addr++;
    }
    gpio_put(cc1101->cs_pin, 1);
}


void print_part_num(CC1101 *cc1101) {
    uint8_t cc1101_part_num[10] = {'a'};
    read_reg(cc1101, PARTNUM_ADDR, cc1101_part_num);
    printf("CC1101 part number is: %s\n", cc1101_part_num);
}

void calculate_frequency()


void set_frequency(CC1101 *cc1101, uint8_t frequency) {
    
}
