#ifndef TYPES_H
#define TYPES_H

#define MAX_MAP_SIZE 1000
#define MAX_FLOWER_POLLEN 100
#define MAX_HIVE_POLLINATORS 200
#define MAX_POLLEN_LEN 100

enum PollenType { FLOAT, STRING };
enum BeehiveAction { MAX, MIN, SUM, SORT };


union Pollen {
	float float_info;
	char string_info[MAX_POLLEN_LEN];
};

struct Config {
	int duration;
	enum PollenType pollen_type;
	enum BeehiveAction beehive_action;
};

struct Map {
	int map_size;
	char map[MAX_MAP_SIZE][MAX_MAP_SIZE];
};

struct Bee {
	int row;
	int col;
	int speed;
	int perception;
};

struct Wasp {
	int row;
	int col;
	int speed;
};

union Pollinator {
	struct Bee bee;
	struct Wasp wasp;
};

struct Hive {
	int row;
	int col;
	int num_pollinators;
	union Pollinator pollinators[MAX_HIVE_POLLINATORS];
};

struct Flower {
	int row;
	int col;
	enum PollenType pollen_type;
	union Pollen pollen[MAX_FLOWER_POLLEN];
};


#endif
