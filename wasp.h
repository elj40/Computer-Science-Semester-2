#ifndef BEE_H
#define BEE_H

#include "types.h"

void add_wasp_to_cell(WaspNode **head, Wasp wasp);
void wasp_check_for_flowers( Wasp *wasp,  Map *m, int *fr, int *fc);
void wasp_print_list(WaspNode * head);
void print_wasp(Wasp b);
Trajectory wasp_get_next_trajectory(Wasp *wasp,  Map *map);

#endif

