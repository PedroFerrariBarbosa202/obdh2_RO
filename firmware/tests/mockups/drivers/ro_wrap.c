/*
 * ro_wrap.c
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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * \brief RO driver wrap implementation.
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Renato Augusto Schenkel Meneghin Marchiori
 *
 * \version 0.0.1
 *
 * \date 2026/06/10
 *
 * \addtogroup ro_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "ro_wrap.h"

#define RO_CHECK_CONFIG(config)                       \
    do                                                 \
    {                                                  \
        check_expected(config.interface);              \
        check_expected(config.en_pin);                 \
                                                       \
        if (config.interface == RO_IF_UART)             \
        {                                              \
            check_expected(config.uart_port);          \
        }                                              \
        else if (config.interface == RO_IF_I2C)        \
        {                                              \
            check_expected(config.i2c_port);           \
            check_expected(config.i2c_bitrate);        \
        }                                              \
    } while (false)

#define RO_CHECK_DATA(config, data, len)               \
    do                                                 \
    {                                                  \
        RO_CHECK_CONFIG(config);                       \
        check_expected_ptr(data);                      \
        check_expected(len);                           \
    } while (false)

#define RO_MOCK_PACKAGE(config, data)                  \
    do                                                 \
    {                                                  \
        RO_CHECK_CONFIG(config);                       \
                                                       \
        if ((data) != NULL)                            \
        {                                              \
            (data) = mock_ptr_type(uint8_t *);         \
        }                                              \
    } while (false)

int __wrap_ro_init(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_enable(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_disable(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_check_device(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_echo(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_start_tracking(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_stop_tracking(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_start_capture(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_abort_capture(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_enable_open_loop(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_disable_open_loop(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_clear_event(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_i2c_init(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_gpio_init(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_gpio_set(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_gpio_clear(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_uart_init(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_uart_rx_available(ro_config_t config)
{
    RO_CHECK_CONFIG(config);
    return mock_type(int);
}

int __wrap_ro_set_rtc_time(ro_config_t config, uint32_t time)
{
    RO_CHECK_CONFIG(config);
    check_expected(time);

    return mock_type(int);
}

int __wrap_ro_write_cmd(ro_config_t config, ro_cmd_t cmd)
{
    RO_CHECK_CONFIG(config);
    check_expected(cmd.id);
    check_expected(cmd.param);

    return mock_type(int);
}

int __wrap_ro_read(ro_config_t config, uint8_t *data, uint16_t len)
{
    RO_CHECK_DATA(config, data, len);
    return mock_type(int);
}

uint16_t __wrap_ro_calc_checksum(uint8_t *data, uint16_t len)
{
    check_expected_ptr(data);
    check_expected(len);

    return mock_type(uint16_t);
}

int16_t __wrap_ro_get_state_pkg(ro_config_t config, uint8_t *status)
{
    RO_MOCK_PACKAGE(config, status);
    return mock_type(int16_t);
}

int16_t __wrap_ro_get_hk_pkg(ro_config_t config, uint8_t *hk)
{
    RO_MOCK_PACKAGE(config, hk);
    return mock_type(int16_t);
}

int16_t __wrap_ro_get_event_pkg(ro_config_t config, uint8_t *event)
{
    RO_MOCK_PACKAGE(config, event);
    return mock_type(int16_t);
}

int16_t __wrap_ro_get_navigation_pkg(ro_config_t config, uint8_t *navigation)
{
    RO_MOCK_PACKAGE(config, navigation);
    return mock_type(int16_t);
}

int16_t __wrap_ro_get_observation_pkg(ro_config_t config, uint8_t *observation)
{
    RO_MOCK_PACKAGE(config, observation);
    return mock_type(int16_t);
}

int16_t __wrap_ro_get_iq_pkg(ro_config_t config, uint8_t *iq)
{
    RO_MOCK_PACKAGE(config, iq);
    return mock_type(int16_t);
}

int __wrap_ro_get_state(ro_config_t config, ro_state_t *state)
{
    RO_CHECK_CONFIG(config);

    if (state != NULL)
    {
        state->current_time = mock_type(uint32_t);
        state->operational_state = mock_type(uint8_t);
        state->tracking_state = mock_type(uint8_t);
        state->event_available = mock_type(uint8_t);
        state->reserved = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_ro_get_hk(ro_config_t config, ro_hk_t *hk)
{
    uint16_t i = 0U;

    RO_CHECK_CONFIG(config);

    if (hk != NULL)
    {
        hk->current_time = mock_type(uint32_t);
        hk->fpga_temperature = mock_type(uint16_t);
        hk->rf_temperature = mock_type(uint16_t);
        hk->voltage = mock_type(uint16_t);
        hk->current = mock_type(uint16_t);
        hk->operational_state = mock_type(uint8_t);

        for (i = 0U; i < sizeof(hk->reserved); i++)
        {
            hk->reserved[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_ro_get_event(ro_config_t config, ro_event_t *event)
{
    RO_CHECK_CONFIG(config);

    if (event != NULL)
    {
        event->event_id = mock_type(uint32_t);
        event->timestamp_start = mock_type(uint32_t);
        event->timestamp_end = mock_type(uint32_t);
        event->prn = mock_type(uint8_t);
        event->duration_ms = mock_type(uint32_t);
        event->num_samples = mock_type(uint32_t);
        event->event_status = mock_type(uint8_t);
    }

    return mock_type(int);
}

int __wrap_ro_get_navigation(ro_config_t config, ro_navigation_t *navigation)
{
    RO_CHECK_CONFIG(config);

    if (navigation != NULL)
    {
        navigation->sat_pos_x = mock_type(int32_t);
        navigation->sat_pos_y = mock_type(int32_t);
        navigation->sat_pos_z = mock_type(int32_t);
        navigation->sat_vel_x = mock_type(int32_t);
        navigation->sat_vel_y = mock_type(int32_t);
        navigation->sat_vel_z = mock_type(int32_t);
        navigation->gnss_time = mock_type(uint32_t);
    }

    return mock_type(int);
}

int __wrap_ro_get_observation(ro_config_t config, ro_observation_t *observation)
{
    uint16_t i = 0U;

    RO_CHECK_CONFIG(config);

    if (observation != NULL)
    {
        observation->observation_id = mock_type(uint32_t);
        observation->gnss_time = mock_type(uint32_t);
        observation->prn = mock_type(uint8_t);
        observation->carrier_phase = mock_type(int32_t);
        observation->code_phase = mock_type(int32_t);
        observation->doppler = mock_type(int32_t);
        observation->snr = mock_type(uint16_t);
        observation->tracking_state = mock_type(uint8_t);

        for (i = 0U; i < sizeof(observation->reserved); i++)
        {
            observation->reserved[i] = mock_type(uint8_t);
        }
    }

    return mock_type(int);
}

int __wrap_ro_i2c_write(ro_config_t config, uint8_t *data, uint16_t len)
{
    RO_CHECK_DATA(config, data, len);
    return mock_type(int);
}

int __wrap_ro_i2c_read(ro_config_t config, uint8_t *data, uint16_t len)
{
    RO_CHECK_DATA(config, data, len);
    return mock_type(int);
}

int __wrap_ro_uart_write(ro_config_t config, uint8_t *data, uint16_t len)
{
    RO_CHECK_DATA(config, data, len);
    return mock_type(int);
}

int __wrap_ro_uart_read(ro_config_t config, uint8_t *data, uint16_t len)
{
    RO_CHECK_DATA(config, data, len);
    return mock_type(int);
}

void __wrap_ro_delay_ms(uint32_t ms)
{
    check_expected(ms);
}

/** \} End of ro_wrap group */
