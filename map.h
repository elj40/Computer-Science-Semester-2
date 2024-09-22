#ifndef MAP_H
#define MAP_H

#include "types.h"

void clear_map(struct Map *map);
void read_map(struct Map *map, struct Config c);
void add_pollen(struct Flower *f, union Pollen p);

#endif
