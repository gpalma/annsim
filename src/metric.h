/**
 * Copyright (C) 2012, 2013 Universidad Simón Bolívar
 *
 * Copying: GNU GENERAL PUBLIC LICENSE Version 2
 * @author Guillermo Palma <gpalma@ldc.usb.ve>
 */

#ifndef ___METRIC_H
#define ___METRIC_H

void init_metric_data(const struct graph *g);

double dist_tax(const struct graph *g, long term1, long term2);

double sim_dtax(const struct graph *g, long x, long y);

double dist_tax_lca(const struct graph *g, long x, long y, long *lcap);

double dist_ps(const struct graph *g, long x, long y);

double sim_dps(const struct graph *g, long x, long y);

double dist_ps_lca(const struct graph *g, long x, long y, long *lcap);

void free_metric(void);

const long *get_nodes_depth(void);

#endif /* ___METRIC_H */
