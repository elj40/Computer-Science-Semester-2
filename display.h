#ifndef DISPLAY_H
#define DISPLAY_H

#include "map.h"

void display_map(Map *map);
void display_char_map(char *map, int s);
void print_horz_border(int s);
void print_horz_row(char *map, int r, int s);

#endif
