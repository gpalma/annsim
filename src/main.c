/**
 * Copyright (C) 2012, 2013 Universidad Simón Bolívar
 *
 * Copying: GNU GENERAL PUBLIC LICENSE Version 2
 * @author Guillermo Palma <gpalma@ldc.usb.ve>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "types.h"
#include "memory.h"
#include "timer.h"
#include "graph.h"
#include "util.h"
#include "input.h"
#include "ann_sim.h"

#define N_ARG   5

struct global_args {
  char *graph_filename;
  char *desc_filename;
  char *annt1_filename;
  char *annt2_filename;
};

static struct global_args g_args;

/*********************************
 **  Parse Arguments
 *********************************/

static void display_usage(void)
{
  fatal("Incorrect arguments \n\tannsim <graph> <terms> <annotations 1> <annotations 2>\n");
}

static void get_name(const char *filename, int n, char *instance)
{
     char buf[n];
     char *aux = NULL;

     strcpy(buf,filename);
     aux = strtok(buf,"/");
     do {
	  strcpy(instance, aux);
     } while((aux = strtok(NULL,"/")) != NULL);

     strcpy(buf,instance);
     aux = strtok(buf,".");
     strcpy(instance, aux);
}

static void initialize_arguments(void)
{
  g_args.graph_filename = NULL;
  g_args.desc_filename = NULL;
  g_args.annt1_filename = NULL;
  g_args.annt2_filename = NULL;
}

static void print_args(void)
{
  printf("\n*********************\n");
  printf("Parameters:\n");
  printf("Graph: %s\n", g_args.graph_filename);
  printf("Terms description: %s\n", g_args.desc_filename);
  printf("Annotations 1: %s\n", g_args.annt1_filename);
  printf("Annotations 2: %s\n", g_args.annt2_filename);
  printf("*********************\n");
}

static void parse_args(int argc, char **argv)
{
  int i = 1;

  initialize_arguments();
  if (argc != N_ARG)
    display_usage();
  g_args.graph_filename = argv[i++];
  g_args.desc_filename = argv[i++];
  g_args.annt1_filename = argv[i++];
  g_args.annt2_filename = argv[i];
}

/*********************************
 *********************************
 **
 **       Main section
 **
 *********************************
 **********************************/

int main(int argc, char **argv)
{
  double ti, tf, similarity;
  static char *name1, *name2;
  struct input_data in;
  int len;

  ti = elapsed_time(REAL);
  parse_args(argc, argv);
  print_args();
  ti = 0.0;
  tf = 0.0;
  similarity = 0.0;
  start_timers();

  /* get the names of the concepts */
  len = strlen(g_args.annt1_filename) + 1;
  name1 = xcalloc(len, 1);
  get_name(g_args.annt1_filename, len, name1);

  len = strlen(g_args.annt2_filename) + 1;
  name2 = xcalloc(len, 1);
  get_name(g_args.annt2_filename, len, name2);

  /* start solver */
  printf("\n**** Solver Begins ****\n");
  ti = elapsed_time(REAL);
  in = get_input_ontology_data(g_args.graph_filename,
                               g_args.desc_filename,
                               g_args.annt1_filename,
                               g_args.annt2_filename);
  similarity = similarity_ann_sim(&in.g, &in.anntt1, &in.anntt2);
  printf("\nSimilarity\t%s\t%s\t%.3f\n", name1, name2, similarity);
  tf = elapsed_time(REAL);
  printf("\nTotal Time %.3f secs\n", tf-ti);
  free(name1);
  free(name2);
  free_input_data(&in);

  return 0;
}
