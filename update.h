#ifndef UPDATE_H
#define UPDATE_H

void bee_move(struct Bee *bee, struct Map *map);
void wasp_move(struct Wasp *wasp, struct Map *map);
void bee_update(struct Bee *bee, struct Map *map, struct Config *config);
void map_update(struct Map *map, struct Config *config);
void hive_update(struct Hive *hive, struct Map *map, struct Config *config);

#endif
