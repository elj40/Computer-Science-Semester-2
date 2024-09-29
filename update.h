#ifndef UPDATE_H
#define UPDATE_H

void bee_move( Bee *bee,  Map *map);
void wasp_move( Wasp *wasp,  Map *map);
void bee_update( Bee *bee,  Map *map, Config *config);
void map_update( Map *map, Config *config);
void hive_update( Hive *hive,  Map *map, Config *config);
void add_bee_to_cell(BeeNode **head, Bee bee);
void remove_bee_from_cell(BeeNode **head, Bee bee);
void bee_print_list(BeeNode * head);

#endif
