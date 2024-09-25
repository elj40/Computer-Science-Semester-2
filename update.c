#include <stdio.h>
#include <math.h>

#include "types.h"
#include "update.h"
#include "compass.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define EPS 0.01

void map_update(struct Map *map, struct Config *config) {
	for (int i = 0; i < map->hive_len; i++) {
		hive_update(&map->hives[i], map, config);
	}
}

void hive_update(struct Hive *hive, struct Map *map, struct Config *config) {
	for (int i = 0; i < hive->pollinator_len; i++) {
		switch (hive->type) {
			case 'W':
				break;
			case 'H':
			case 'B':
			case 'D':
				bee_move(&hive->pollinators[i].bee, map);
				break;
			default:
				printf("Unknown hive type in update");
				break;
		}
	}
}
void bee_move(struct Bee *bee, struct Map *map) {
	struct Trajectory t = get_next_trajectory(bee->speed);

	float dir = t.direction;
	int s = t.distance;


	printf("dir=%f dist=%d\n", dir, s);

	float cos_dir = cos(dir);
	float sin_dir = sin(dir);

	int dc = 0;
	int dr = 0;

	if (sin_dir > EPS) dr = s;
	else if(sin_dir < -EPS) dr = -s;

	if (cos_dir > EPS) dc = s;
	else if(cos_dir < -EPS) dc = -s;

	int nc = bee->col + dc;
	int nr = bee->row + dr;

	nc = MIN(map->map_size-1, MAX(nc, 0));
	nr = MIN(map->map_size-1, MAX(nr, 0));

	printf("Bee moved from %d,%d to %d,%d, d: %d,%d\n", bee->row, bee->col, nr, nc, dr, dc);

	if (map->map[bee->row][bee->col] == 'b') { 
		map->map[bee->row][bee->col] = ' ';
	}

	if (map->map[nr][nc] == ' ') {
		map->map[nr][nc] = 'b';
	}


	bee->row = nr;
	bee->col = nc;
}

void bee_update(struct Bee *bee, struct Map *map, struct Config *config) {

}
