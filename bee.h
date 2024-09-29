#ifndef BEE_H
#define BEE_H

#include <stdbool.h>
#include "types.h"

void bee_action(Bee* bee, Map *map);
void add_bee_to_cell(BeeNode **head, Bee bee);
bool bee_check_for_flowers(Bee *bee,  Map *m);
void bee_locate_flowers(Bee *bee,  Map *m, int *fr, int *fc);
void bee_print_list(BeeNode * head);
void print_bee(Bee b);
Trajectory bee_get_next_trajectory(Bee *bee,  Map *map);

#endif
