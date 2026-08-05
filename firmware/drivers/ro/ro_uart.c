/*
 * ro_uart.c
 * 
 * Copyright (C) 2021, SpaceLab.
 * 
 * This file is part of OBDH 2.0.
 * 
 * OBDH 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OBDH 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief RO UART routines implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \modified by Renato Augusto Schenkel Meneghin Marchiori
 * \version 0.0.1
 * 
 * \date 2026/08/03
 * 
 * \defgroup ro_uart UART 
 * \ingroup ro
 * \{
 */
#include <drivers/uart/uart.h>  

#include "ro.h"

int ro_uart_init(ro_config_t config)
{
    uart_config_t uart_conf = {0};
    int err = -1;
    
    uart_conf.baudrate = RO_UART_BAUDRATE;
    uart_conf.data_bits = 8U;
    uart_conf.parity = UART_NO_PARITY;
    uart_conf.stop_bits = UART_ONE_STOP_BIT;

    if (uart_init(config.uart_port, uart_conf) == 0) 
    {
        err = uart_rx_enable(config.uart_port);
    }
    
    return err;
}

int ro_uart_write(ro_config_t config, uint8_t *data, uint16_t len) 
{ 
    int dummy = uart_flush(config.uart_port); 
    
    return uart_write(config.uart_port, data, len); 
}

int ro_uart_read(ro_config_t config, uint8_t *data, uint16_t len) 
{ 
    return uart_read(config.uart_port, data, len); 
}

int ro_uart_rx_available(ro_config_t config) 
{ 
    return (int)uart_read_available(config.uart_port); 
}
/** \} End of ro group */
