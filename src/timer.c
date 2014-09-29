/**
 * Copyright (C) 2011, Universidad Simón Bolívar
 *
 * @brief Routines for measuring elapsed time
 * Copying: GNU GENERAL PUBLIC LICENSE Version 2
 * @author Guillermo Palma <gpalma@ldc.usb.ve>
 */

#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "timer.h"

/*********************************
 **  Static Variables
 *********************************/

static struct rusage res;
static struct timeval tp;
static double virtual_time, real_time;

/*********************************
 **  Timers
 *********************************/

void start_timers(void)
{
  getrusage(RUSAGE_SELF, &res);
  virtual_time = (double) res.ru_utime.tv_sec +
      (double) res.ru_stime.tv_sec +
      (double) res.ru_utime.tv_usec / 1000000.0 +
      (double) res.ru_stime.tv_usec / 1000000.0;
  gettimeofday(&tp, NULL);
  real_time = (double) tp.tv_sec + (double) tp.tv_usec / 1000000.0;
}

double elapsed_time(enum timer_e type)
{
  if (type == REAL) {
    gettimeofday(&tp, NULL);
    return( (double) tp.tv_sec +
            (double) tp.tv_usec / 1000000.0 - real_time );
  } else {
    getrusage(RUSAGE_SELF, &res);
    return( (double) res.ru_utime.tv_sec +
            (double) res.ru_stime.tv_sec +
            (double) res.ru_utime.tv_usec / 1000000.0 +
            (double) res.ru_stime.tv_usec / 1000000.0 - virtual_time );
  }
}
