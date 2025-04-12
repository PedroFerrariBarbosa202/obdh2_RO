/*
 * sched_tc.c
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
 * \brief Scheduled TC Execution Task definition.
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2025/04/12
 * 
 * \addtogroup sched_tc
 * \{
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <projdefs.h>
#include <structs/satellite.h>
#include <structs/obdh_data.h>
#include <devices/payload/payload.h>
#include <conops/conops.h>
#include <conops/util/cmd_queue.h>
#include <system/sys_log/sys_log.h>
#include <system/system.h>
#include <config/config.h>

#include "sched_tc.h"
#include "process_tc.h"
#include "startup.h"

TaskHandle_t xTaskSchedTCHandle;

static struct conops_cmd_queue tc_queue;

static void tc_queue_lock(void *lock)
{
    SemaphoreHandle_t sem = *(SemaphoreHandle_t*)lock;

    if (xSemaphoreTake(sem, pdMS_TO_TICKS(TC_QUEUE_MUTEX_TIMEOUT_MS)) == pdFALSE)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_SCHED_TC_NAME, "Failed to acquire TC Queue lock!");
        sys_log_new_line();
    }
}

static void tc_queue_unlock(void *lock)
{
    SemaphoreHandle_t sem = *(SemaphoreHandle_t*)lock;

    (void)xSemaphoreGive(sem);
}

void reset_sched_tc_queue(void)
{
    cmd_queue_reset(&tc_queue);
}

void vTaskSchedTC(void* p)
{
    (void)p;

    SemaphoreHandle_t xSemQueuelock = NULL;
    int err = 0;

    (void)xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_SCHED_TC_STARTUP_TIMEOUT_MS));

    // mem_mng_load_tc_queue_from_fram(&tc_queue); /* Load Sched TCs from FRAM */

    xSemQueuelock = xSemaphoreCreateMutex();

    if (xSemQueuelock != NULL)
    {
        cmd_queue_init(&tc_queue, &xSemQueuelock, tc_queue_lock, tc_queue_unlock);
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_SCHED_TC_NAME, "Failed to create TC Queue lock! Using lockless impl...");
        sys_log_new_line();

        cmd_queue_init(&tc_queue, NULL, NULL, NULL);
    }

    TickType_t last_cycle = xTaskGetTickCount();

    while (1)
    {
        struct conops_cmd tc = {0};

        uint32_t now = system_get_time(); 

        while (cmd_queue_dequeue(&tc_queue, now, &tc) == 1)
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_SCHED_TC_NAME, "Executing TC scheduled for: ");
            sys_log_print_uint(tc.timestamp);
            sys_log_new_line();

            //err = execute_tc(tc.payload, sizeof(tc.payload), true);
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_SCHED_TC_PERIOD_MS));
    }
}

/** \} End of sched_tc group */
