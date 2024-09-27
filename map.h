#ifndef MAP_H
#define MAP_H

#include "types.h"

void clear_map( Map *map);
void read_map( Map *map, Config c);
void add_pollen( Flower *f, union Pollen p);

#endif
