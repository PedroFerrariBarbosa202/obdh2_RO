/*
 * ttc.c
 * 
 * Copyright The OBDH 2.0 Contributors.
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
 * \brief TTC device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.35
 * 
 * \date 2020/02/01
 * 
 * \addtogroup ttc
 * \{
 */

#include <stdbool.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>
#include <drivers/i2c/i2c.h>

#include "ttcsb.h"

static ttcsb_config_t ttcsb_config = {0};

int ttcsb_init()
{
    static bool ttcsb_is_open = false;

    int err = -2;

    ttcsb_config_t ttc_config = {0};

    if (ttcsb_is_open)
    {
        err = 0;    /* TTC 0 device already initialized */
    }
    else
    {
        //TODO! view configs
        ttcsb_config.port                   = SPI_PORT_0;
        ttcsb_config.cs_pin                 = SPI_CS_0;
        ttcsb_config.port_config.speed_hz   = CONFIG_SPI_PORT_0_SPEED_BPS;
        ttcsb_config.port_config.mode       = SPI_MODE_0;
        ttcsb_config.id                     = SL_TTC2_RADIO_0;

        ttc_config = ttcsb_config;
    }

    if (err == -2)
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, TTC_MODULE_NAME, "Initializing TTC SBAND device ");
        sys_log_print_uint(ttcsb_config.id);
        sys_log_print_msg("...");
        sys_log_new_line();

        if (sl_ttcsb_init(ttc_config) == 0)
        {
            uint8_t hw_ver = 0;

            if (sl_ttc2_read_hardware_version(ttc_config, &hw_ver) == 0)
            {
                uint32_t fw_ver = 0;

                if (sl_ttc2_read_firmware_version(ttc_config, &fw_ver) == 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_INFO, TTC_MODULE_NAME, "SpaceLab TTC SBAND detected! (hw=");
                    sys_log_print_uint(hw_ver);
                    sys_log_print_msg(", fw=");
                    sys_log_print_uint(fw_ver);
                    sys_log_print_msg(")");
                    sys_log_new_line();

                    ttcsb_is_open = true;
                    err = 0;
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error reading the firmware version of the TTC device ");
                    sys_log_print_uint(ttc_config.id);
                    sys_log_print_msg("!");
                    sys_log_new_line();

                    err = -1;
                }
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error reading the hardware version of the TTC device ");
                sys_log_print_uint(ttc_config.id);
                sys_log_print_msg("!");
                sys_log_new_line();

                err = -1;
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error initializing the TTC device ");
            sys_log_print_uint(ttc_config.id);
            sys_log_print_msg("!");
            sys_log_new_line();

            err = -1;
        }
    }

    return err;
}

int ttc_set_param(ttc_param_id_t param, uint32_t val)
{
    int err = -1;

    err = sl_ttc2_write_reg(ttc_config, param, val);  
    return err;
}

int ttc_get_param(ttc_param_id_t param, uint32_t *val)
{
    int err = -1;

    err = sl_ttc2_read_reg(ttc_config, param, val); 
    return err;
}

int ttc_get_data(ttc_data_t *data)
{
    int err = -1;

    ttcsb_config_t ttc_config = {0};

    ttc_config = ttcsb_config;
    err = 0;

    if (err == 0)
    {
        if (sl_ttcsb_check_device(ttc_config) == 0) // needs read_reg
        {
            if (sl_ttc2_read_hk_data(ttc_config, data) != 0) // needs read_reg
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error reading the data from the TTC device ");
                sys_log_print_uint(ttc_config.id);
                sys_log_print_msg("!");
                sys_log_new_line();

                err = -1;
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TTC_MODULE_NAME, "Error reading the data from the TTC device ");
            sys_log_print_uint(ttc_config.id);
            sys_log_print_msg("! No device detected!");
            sys_log_new_line();
        }
    }

    return err;
}

/** \} End of ttc group */
