#ifndef  __MATCH_H
#define  __MATCH_H

#include "concorde.h"

#ifdef CC_PROTOTYPE_ANSI

int
  perfect_match (int ncount, CCdatagroup *dat, int ecount, int **elist,
							 int **elen, char *blo_filename, char *mat_filename,
							 int just_fractional, int no_fractional, int use_all_trees,
							 int partialprice, double *totalzeit),
 perfect_matchPS (int ncount, CCdatagroup *dat, int ecount, int **elist,
							 int **elen, char *blo_filename, match_edge_t *mat_filename,
							 int just_fractional, int no_fractional, int use_all_trees,
							 int partialprice, double *totalzeit),
	matching_price (int ncount, CCdatagroup *dat, double *orig_pi,
									int *orig_parent, int *badcount, int **badlist, int **badlen,
									double *penalty, CCtsp_edgegenerator *starteg, char *hit),
	matching_check (int ncount, double *orig_pi, int *orig_parent, 
									int *matchlist, int *mlen, int *csbad),
	min_weight_perfect_match (match_edge_t *edgefilenamePS, match_edge_t *matchPS);


#else

int
  perfect_match (),
	matching_price (),
	matching_check (),
	min_weight_perfect_match();
#endif

#endif  /* __MATCH_H */
