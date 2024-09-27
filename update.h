#ifndef UPDATE_H
#define UPDATE_H

void bee_move( Bee *bee,  Map *map);
void wasp_move( Wasp *wasp,  Map *map);
void bee_update( Bee *bee,  Map *map, Config *config);
void map_update( Map *map, Config *config);
void hive_update( Hive *hive,  Map *map, Config *config);

#endif
