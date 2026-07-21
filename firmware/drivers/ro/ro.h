


#ifndef RO_H_
#define RO_H_

#include <stdint.h>
#include <stdbool.h>

#include <drivers/i2c/i2c.h>
#include <drivers/gpio/gpio.h>
#include <drivers/uart/uart.h>

#define RO_MODULE_NAME          "RO"

#define RO_SLAVE_ADDRESS        0x15

#define RO_CMD_GET_STATE        0x30
#define RO_CMD_GET_HK           0x31
#define RO_CMD_GET_EVENT        0x32
#define RO_CMD_GET_OBS          0x33
#define RO_CMD_GET_IQ           0x34

#define RO_FRAME_STATE          0x11
#define RO_FRAME_HK             0x22
#define RO_FRAME_EVENT          0x33
#define RO_FRAME_OBS            0x44
#define RO_FRAME_IQ             0x55

typedef enum
{
    RO_IF_UART=0,
    RO_IF_I2C

} ro_if_t;

typedef struct
{
    ro_if_t interface;

    i2c_port_t i2c_port;

    uint32_t i2c_bitrate;

    uart_port_t uart_port;

    gpio_pin_t en_pin;

} ro_config_t;

typedef struct
{
    uint8_t id;

    uint32_t param;

} ro_cmd_t;

typedef struct
{
    uint32_t current_time;

    uint8_t tracking_state;

    uint8_t event_available;

} ro_state_t;

typedef struct
{
    uint32_t current_time;

    uint16_t fpga_temp;

    uint16_t rf_temp;

    uint16_t voltage;

    uint16_t current;

    uint8_t state;

    uint8_t reserved[8];

} ro_hk_t;