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
	Cell next_map[MAX_MAP_SIZE][MAX_MAP_SIZE];
} Map;

void clear_map_cells(Cell *map, int map_size);
void copy_map_cells(Cell *dest, Cell *src, int map_size);
void copy_map_enitites(Cell *dest, Cell *src, int map_size);
void free_map(Map *map);
void read_map( Map *map, Config c);
void add_pollen( Flower *f, union Pollen p);
void bee_free_linked_list(BeeNode *head);

#endif
