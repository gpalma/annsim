/**
 * Copyright (C) 2012, 2013, Universidad Simón Bolívar
 *
 * Copying: GNU GENERAL PUBLIC LICENSE Version 2
 * @author Guillermo Palma <gpalma@ldc.usb.ve>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

#include "dlist.h"
#include "types.h"
#include "util.h"
#include "memory.h"
#include "vec.h"
#include "graph.h"
#include "CA.h"
#include "metric.h"
#include "match.h"
#include "concorde.h"
#include "ann_sim.h"

#define ROOT 0

struct dmatrix {
  long r, c;
  long **lca;
  double **sim;
};

static void init_dmatrix(struct dmatrix *m, long row, long col)
{
  m->r = row;
  m->c = col;
  m->sim = double_matrix(0, m->r, 0, m->c);
  m->lca = long_matrix(0, m->r, 0, m->c);
}

static void free_dmatrix(struct dmatrix *m)
{
  free_double_matrix(m->sim, 0, 0);
  free_long_matrix(m->lca, 0, 0);
}

static inline void print_dmatrix(double * const *a, long n, long m)
{
  int i, j;

  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      printf("%.3f ", a[i][j]);
    }
    printf("\n");
  }
}

static struct dmatrix *calculate_metrics(const struct graph *g, const VEC(long) *vnodes1,
					 const VEC(long) *vnodes2)
{
  long i, j, x, y;
  double dtax;

  struct dmatrix *metric;
  metric = xmalloc(sizeof(struct dmatrix));
  init_dmatrix(metric, VEC_SIZE(*vnodes1), VEC_SIZE(*vnodes2));
  init_metric_data(g);
  for (i = 0; i < metric->r; i++) {
    x = VEC_GET(*vnodes1, i);
    for (j = 0; j < metric->c; j++) {
      y = VEC_GET(*vnodes2, j);
      dtax =  dist_tax_lca(g, x, y, &metric->lca[i][j]);
      assert(dtax >= 0 && dtax <= 1.0);
      metric->sim[i][j] = dtax;
    }
  }
  free_metric();
  return metric;
}

/*
  struct edge_blist {
  long id;
  long from;
  long to;
  double cost;
  struct list_head list;
  };

  struct graph_bipartite {
  long n;
  VEC(long) *x;
  VEC(long) *y;
  struct edge_blist e;
  };

  static struct graph_bipartite modifySet;

  static void get_modify_set(const VEC(long) *vnodes1,  const VEC(long) *vnodes2,
  struct dmatrix *metric)
  {
  long i, j, id;
  bool *xadd, *yadd;
  size_t alloc;
  struct edge_blist *etmp;

  alloc = metric->r*sizeof(bool);
  xadd = xmalloc(alloc);
  memset(xadd, 0, alloc);
  alloc = metric->c*sizeof(bool);
  yadd = xmalloc(alloc);
  memset(yadd, 0, alloc);
  VEC_INIT(long, *modifySet.x);
  VEC_INIT(long, *modifySet.y);
  INIT_LIST_HEAD(&modifySet.e.list);
  modifySet.n = 0;
  id = 0;
  printf("Modify Set\n");
  for (i = 0; i < metric->r; i++) {
  for (j = 0; j < metric->c; j++) {
  if (metric->lca[i][j] != ROOT) {
  if (xadd[i] == false) {
  xadd[i] = true;
  VEC_PUSH(long, *modifySet.x, i);
  }
  if (yadd[j] == false) {
  yadd[j] = true;
  VEC_PUSH(long, *modifySet.y, j);
  }
  etmp = (struct edge_blist *)xmalloc(sizeof(struct edge_blist));
  printf("Pair-Wise: %ld -- %ld (lca %ld) (cost %.3f)\n",
  VEC_GET(*vnodes1, i),
  VEC_GET(*vnodes2,j), metric->lca[i][j], metric->sim[i][j]);
  etmp->to = i;
  etmp->from = j;
  etmp->id = id++;
  etmp->cost = metric->sim[i][j];
  list_add_tail(&etmp->list, &modifySet.e.list);
  modifySet.n++;
  }
  }
  }
  assert(modifySet.n == id);
  }

  static void free_get_modify_set(void)
  {
  struct edge_blist *etmp;
  struct list_head *pos, *q;

  VEC_DESTROY(*modifySet.x);
  VEC_DESTROY(*modifySet.y);
  list_for_each_safe(pos, q, &modifySet.e.list){
  etmp = list_entry(pos, struct edge_blist, list);
  list_del(pos);
  free(etmp);
  }
  }
*/

static inline void print_max_matching(struct match_edge *me, struct dmatrix *matrix,
				      const VEC(long) *vnodes1, const VEC(long) *vnodes2)
{
  long k, n, i, j;


  n = me[0].b;
  printf("Perfect matching %ld\n", n);
  assert(matrix->r == n);
  for (k = 1; k <= n; k++) {
    i = me[k].a;
    j = me[k].b-n;
    printf("(%ld -- %ld) (%ld -- %ld)  %.3f\n", i, j, VEC_GET(*vnodes1, i),
           VEC_GET(*vnodes2, j), matrix->sim[i][j]);
  }
  printf("\n");
}

double similarity_ann_sim(const struct graph *g, const VEC(long) *vnodes1, const VEC(long) *vnodes2)
{
  double s;
  VEC(long) vx, vy;
  long i, j, n, m, q, n_edges, n_nodes, k;
  struct dmatrix *ametric;
  struct match_edge *in, *out;
  double sum;

  s = 0.0;
  n = VEC_SIZE(*vnodes1);
  m = VEC_SIZE(*vnodes2);
  VEC_INIT_N(long, vx, n);
  VEC_INIT_N(long, vy, m);
  VEC_COPY(long, *vnodes1, vx);
  VEC_COPY(long, *vnodes2, vy);
  assert(VEC_SIZE(vx) == (unsigned)n);
  assert(VEC_SIZE(vy) == (unsigned)m);
  if (n > m) {
    q = n - m;
    for (i = 0; i < q; i++) {
      VEC_PUSH(long, vy, ROOT);
    }
  } else {
    q = m - n;
    for (i = 0; i < q; i++) {
      VEC_PUSH(long, vx, ROOT);
    }
  }
  ametric = calculate_metrics(g, &vx, &vy);
#ifdef PRGDEBUG
  printf("Add %ld nodes\n\n", q);
  print_dmatrix(ametric->sim, ametric->r, ametric->c);
#endif
  n = VEC_SIZE(vx);
  assert((unsigned)n == VEC_SIZE(vy));
  n_nodes = n*2;
  n_edges = n*n;
  m = n_edges+1;
  in = (struct match_edge *)xmalloc(m*sizeof(struct match_edge));
  out = (struct match_edge *)xmalloc(m*sizeof(struct match_edge));
  in[0].a = n_nodes;
  in[0].b = n_edges;
  m = 1;
  for (i = 0; i < n; i++) {
    for (j = n; j < n_nodes; j++) {
      in[m].a = i;
      in[m].b = j;
      in[m].cost = (long)(ametric->sim[i][j-n]*10000.0);
      m++;
    }
  }
  assert((m-1) == n_edges);
  min_weight_perfect_match(in, out);
#ifdef PRGDEBUG
  print_max_matching(out, ametric, &vx, &vy);
#endif
  n = out[0].b;
  assert(ametric->r == n);
  sum = 0.0;
  for (k = 1; k <= n; k++) {
    i = out[k].a;
    j = out[k].b-n;
    sum += 1.0 - ametric->sim[i][j];
  }
  s = (2.0*sum) / (VEC_SIZE(*vnodes1) + VEC_SIZE(*vnodes2));
  free(out);
  free(in);
  free_dmatrix(ametric);
  free(ametric);
  VEC_DESTROY(vx);
  VEC_DESTROY(vy);

  return s;
}
