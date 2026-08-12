/*
 * ro_wrap.h
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
 * \brief RO driver wrap definition.
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Renato Augusto Schenkel Meneghin Marchiori
 *
 * \version 0.0.1
 *
 * \date 2026/06/10
 *
 * \defgroup ro_wrap RO Wrap
 * \ingroup tests
 * \{
 */

#ifndef RO_WRAP_H_
#define RO_WRAP_H_

#include <stdint.h>

#include <drivers/ro/ro.h>

int __wrap_ro_init(ro_config_t config);
int __wrap_ro_enable(ro_config_t config);
int __wrap_ro_disable(ro_config_t config);
int __wrap_ro_check_device(ro_config_t config);
int __wrap_ro_set_rtc_time(ro_config_t config, uint32_t time);

int __wrap_ro_write_cmd(ro_config_t config, ro_cmd_t cmd);
int __wrap_ro_read(ro_config_t config, uint8_t *data, uint16_t len);
int __wrap_ro_echo(ro_config_t config);
uint16_t __wrap_ro_calc_checksum(uint8_t *data, uint16_t len);

int __wrap_ro_start_tracking(ro_config_t config);
int __wrap_ro_stop_tracking(ro_config_t config);
int __wrap_ro_start_capture(ro_config_t config);
int __wrap_ro_abort_capture(ro_config_t config);
int __wrap_ro_enable_open_loop(ro_config_t config);
int __wrap_ro_disable_open_loop(ro_config_t config);
int __wrap_ro_clear_event(ro_config_t config);

int16_t __wrap_ro_get_state_pkg(ro_config_t config, uint8_t *status);
int16_t __wrap_ro_get_hk_pkg(ro_config_t config, uint8_t *hk);
int16_t __wrap_ro_get_event_pkg(ro_config_t config, uint8_t *event);
int16_t __wrap_ro_get_navigation_pkg(ro_config_t config, uint8_t *navigation);
int16_t __wrap_ro_get_observation_pkg(ro_config_t config, uint8_t *observation);
int16_t __wrap_ro_get_iq_pkg(ro_config_t config, uint8_t *iq);

int __wrap_ro_get_state(ro_config_t config, ro_state_t *state);
int __wrap_ro_get_hk(ro_config_t config, ro_hk_t *hk);
int __wrap_ro_get_event(ro_config_t config, ro_event_t *event);
int __wrap_ro_get_navigation(ro_config_t config, ro_navigation_t *navigation);
int __wrap_ro_get_observation(ro_config_t config, ro_observation_t *observation);

void __wrap_ro_delay_ms(uint32_t ms);

#endif /* RO_WRAP_H_ */

/** \} End of ro_wrap group */
