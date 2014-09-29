/**
 * Copyright (C) 2011, Universidad Simón Bolívar
 *
 * @brief Routines for measuring elapsed time
 * Copying: GNU GENERAL PUBLIC LICENSE Version 2
 * @author Guillermo Palma <gpalma@ldc.usb.ve>
 */

#ifndef ___TIMER_H
#define ___TIMER_H

enum timer_e {REAL, VIRTUAL};

void start_timers(void);

double elapsed_time(enum timer_e type);

#endif /* ___TIMER_H */
