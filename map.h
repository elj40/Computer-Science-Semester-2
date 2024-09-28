#ifndef MAP_H
#define MAP_H

#include "types.h"

typedef struct {
	char display_char;	
	Flower *flower_ptr;
	Hive *hive_ptr;
	BeeNode *bee_head_ptr;
	Wasp *wasp_head_ptr;
} Cell;

typedef struct {
	int map_size;
	Cell map[MAX_MAP_SIZE][MAX_MAP_SIZE];
} Map;

void clear_map( Map *map);
void free_map(Map *map);
void read_map( Map *map, Config c);
void add_pollen( Flower *f, union Pollen p);

#endif
