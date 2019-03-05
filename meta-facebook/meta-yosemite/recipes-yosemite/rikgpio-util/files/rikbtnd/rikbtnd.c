/*
 * sensord
 *
 * Copyright 2015-present Facebook. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/file.h>

#include <openbmc/gpio.h>



// Отсюда
// https://stackoverflow.com/a/36095407
static long long get_nanos(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long long)ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
}

static long long last_nanos = 0;
static long long id_last_time = 0;

// Generic Event Handler for GPIO changes
static void gpio_event_handle(gpio_poll_st *gp)
{
  // char cmd[128] = {0};
  long long tt;

  if (gp->gs.gs_gpio == gpio_num("GPIOD5")) 
  { // 
    tt = get_nanos();
    if((tt - id_last_time) > 600)
    {
      if(gpio_get(gpio_num("GPIOD5")) == GPIO_VALUE_LOW)
        syslog(LOG_INFO, "ID button pressed");
    }
    id_last_time = tt;
  }
  // else if (gp->gs.gs_gpio == gpio_num("GPIOL0")) { // IRQ_UV_DETECT_N
  //   log_gpio_change(gp, 20*1000);
  // }



  long long nanos = get_nanos();
  syslog(LOG_CRIT, "delta %10lld value %d: %s - %s\n", nanos - last_nanos, gp->value, gp->name, gp->desc);
  last_nanos = nanos;
}



// GPIO table to be monitored when MB is ON
static gpio_poll_st g_gpios[] = {
  // {{gpio, fd}, edge, gpioValue, call-back function, GPIO description}
  {{0, 0}, GPIO_EDGE_FALLING, 0, gpio_event_handle, "GPIOD5", "FP_ID_BTN" },
  {{0, 0}, GPIO_EDGE_FALLING, 0, gpio_event_handle, "GPIOR7", "FP_PWR_BTN_MUX_N"},
  {{0, 0}, GPIO_EDGE_FALLING, 0, gpio_event_handle, "GPIOR6", "FM_BMC_PWR_BTN_N"},
};

static int g_count = sizeof(g_gpios) / sizeof(gpio_poll_st);



int
main(int argc, char **argv) {
  int rc, pid_file;

  openlog("rikbtnd", LOG_CONS, LOG_DAEMON);

  pid_file = open("/var/run/rikbtnd.pid", O_CREAT | O_RDWR, 0666);
  rc = flock(pid_file, LOCK_EX | LOCK_NB);
  if(rc) {
    if(EWOULDBLOCK == errno) {
      syslog(LOG_ERR, "Another rikbtnd instance is running...\n");
      exit(-1);
    }
  } else {

    daemon(0, 1);
    syslog(LOG_INFO, "rikbtnd: daemon started. ver 0.2");

    gpio_poll_open(g_gpios, g_count);
    gpio_poll(g_gpios, g_count, -1);
    gpio_poll_close(g_gpios, g_count);
  }

  return 0;
}
