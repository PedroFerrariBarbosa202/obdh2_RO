/*
 * ro.c
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
 * \brief RO driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Bruno Benedetti <brunobenedetti45@gmail.com>
 * \author Renato Augusto Schenkel Meneghin Marchiori
 * 
 * \version 0.0.1
 * 
 * \date 2019/10/27
 * 
 * \addtogroup ro
 * \{
 */

#include <math.h>
#include <string.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "ro.h"

/*============================================================================*/
/* Initialization                                                             */
/*============================================================================*/

/// @brief 
/// @param config 
/// @return 
int ro_init(ro_config_t config);
{
    int err = -1;

    if (ro_gpio_init(config) == 0)
    {
        if (ro_enable(config) == 0)
        {
            /* Used to make sure it can reply to requests after enabling */
            ro_delay_ms(650U);

            switch(config.interface)
            {
                case RO_IF_UART:
                    if (ro_uart_init(config) == 0)
                    {
                        err = ro_check_device(config);
                    }
                    else
                    {
                    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "Error initializing the UART port!");
                        sys_log_new_line();
                    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    }

                    break;
                case RO_IF_I2C:
                    if (ro_i2c_init(config) == 0)
                    {
                        err = ro_check_device(config);
                    }
                    else
                    {
                    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "Error initializing the I2C port!");
                        sys_log_new_line();
                    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    }

                    break;
                default:
                #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                    sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "Unexpected interface!");
                    sys_log_new_line();
                #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    break;
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "Error enabling the module!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "Error initializing the GPIO pins!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

/// @brief 
/// @param config 
/// @return 
int ro_enable(ro_config_t config)
{
    return ro_gpio_set(config);
}

/// @brief 
/// @param config 
/// @return 
int ro_disable(ro_config_t config)
{
    return ro_gpio_clear(config);
}

int ro_check_device(ro_config_t config)
{
    int err = 0;

    uint8_t status[RO_FRAME_STATE_LEN] = {0};

    if (ro_get_state_pkg(config, status) != RO_FRAME_STATE_LEN)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "Error checking the device!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;
}

int ro_set_rtc_time(ro_config_t config, uint32_t time)
{
    ro_cmd_t rtc_cmd;

    rtc_cmd.id = RO_CMD_RTC_SET;
    rtc_cmd.param = time;

    return ro_write_cmd(config, rtc_cmd);
}


/*============================================================================*/
/* Communication                                                                                                                                 */
/*============================================================================*/

int ro_write_cmd(ro_config_t config, ro_cmd_t cmd)
{
    int err = 0;

    uint8_t cmd_str[6] = {0};
    uint16_t cmd_str_len = 1;

    cmd_str[0] = cmd.id;

    switch(cmd.id)
    {
        /* Commands with parameters */
        case RO_CMD_RTC_SET:
            cmd_str[1] = (uint8_t)((cmd.param >> 0) & 0xFFU);
            cmd_str[2] = (uint8_t)((cmd.param >> 8) & 0xFFU);
            cmd_str[3] = (uint8_t)((cmd.param >> 16) & 0xFFU);
            cmd_str[4] = (uint8_t)((cmd.param >> 24) & 0xFFU);
            cmd_str_len += 4;
            break;
        /* Instrument control */
        case RO_CMD_RESET:
        case RO_CMD_ECHO:
        case RO_CMD_START_TRACKING:
        case RO_CMD_STOP_TRACKING:
        case RO_CMD_START_CAPTURE:
        case RO_CMD_ABORT_CAPTURE:

            break;
        
        /* Telemetry */
        case RO_CMD_GET_STATE:
        case RO_CMD_GET_HK:
        case RO_CMD_GET_EVENT:
        case RO_CMD_GET_NAVIGATION:
        case RO_CMD_GET_OBSERVABLES:
        case RO_CMD_GET_IQ:
            break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "Unexpected command!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }

    if (ro_write(config, cmd_str, cmd_str_len) != 0)
    {
        err = -1;
    }

    return err;
}

int ro_read(ro_config_t config, uint8_t *data, uint16_t len)
{
    int err = -1;

    switch(config.interface)
    {
        case RO_IF_UART:
            err = ro_uart_read(config, data, len);
            break;
        case RO_IF_I2C:
            err = ro_i2c_read(config, data, len);
            break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "Unexpected interface!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }

    return err;
}

int ro_echo(ro_config_t config)
{
    int err = 0;

    ro_cmd_t echo_cmd;

    echo_cmd.id = RO_CMD_ECHO;
    echo_cmd.param = 0;

    if (ro_write_cmd(config, echo_cmd) == 0)
    {
        uint8_t echo_ans[5] = {0};

        if (ro_read(config, echo_ans, RO_FRAME_ACK_LEN) == 0)
        {
            uint8_t echo[4] = {'E', 'C', 'H', 'O'}; /* Expected response */

            if (memcmp(echo_ans, echo, 4) != 0)
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "Error reading the \"ECHO\" command response! Wrong answer!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                err = -1;
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "Error reading the \"ECHO\" command response!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "Error writing the \"ECHO\" command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;
}

uint16_t ro_calc_checksum(uint8_t *data, uint16_t len);

/*============================================================================*/
/* Instrument Control                                                         */
/*============================================================================*/

int ro_start_tracking(ro_config_t config);

int ro_stop_tracking(ro_config_t config);

int ro_start_capture(ro_config_t config);

int ro_abort_capture(ro_config_t config);

int ro_enable_open_loop(ro_config_t config);

int ro_disable_open_loop(ro_config_t config);

int ro_clear_event(ro_config_t config);

/*============================================================================*/
/* Data Acquisition                                                           */
/*============================================================================*/

int16_t ro_get_state_pkg(ro_config_t config, uint8_t *status);

int16_t ro_get_hk_pkg(ro_config_t config, uint8_t *hk);

int16_t ro_get_event_pkg(ro_config_t config, uint8_t *event);

int16_t ro_get_navigation_pkg(ro_config_t config, uint8_t *navigation);

int16_t ro_get_observation_pkg(ro_config_t config, uint8_t *observation);

int16_t ro_get_iq_pkg(ro_config_t config, uint8_t *iq);



/*============================================================================*/
/* Data Parsing                                                               */
/*============================================================================*/

int ro_get_state(ro_config_t config,
                 ro_state_t *state);

int ro_get_hk(ro_config_t config,
              ro_hk_t *hk);

int ro_get_event(ro_config_t config,
                 ro_event_t *event);

int ro_get_navigation(ro_config_t config,
                      ro_navigation_t *navigation);

int ro_get_observation(ro_config_t config,
                       ro_observation_t *observation);


/*============================================================================*/
/* I2C Driver                                                                 */
/*============================================================================*/

int ro_i2c_init(ro_config_t config);

int ro_i2c_write(ro_config_t config,
                 uint8_t *data,
                 uint16_t len);

int ro_i2c_read(ro_config_t config,
                uint8_t *data,
                uint16_t len);

/*============================================================================*/
/* GPIO Driver                                                                */
/*============================================================================*/

int ro_gpio_init(ro_config_t config);

int ro_gpio_set(ro_config_t config);

int ro_gpio_clear(ro_config_t config);

/*============================================================================*/
/* UART Driver                                                                */
/*============================================================================*/

int ro_uart_init(ro_config_t config);

int ro_uart_write(ro_config_t config,
                  uint8_t *data,
                  uint16_t len);

int ro_uart_read(ro_config_t config,
                 uint8_t *data,
                 uint16_t len);

int ro_uart_rx_available(ro_config_t config);

/*============================================================================*/
/* Delay                                                                      */
/*============================================================================*/

void ro_delay_ms(uint32_t ms);