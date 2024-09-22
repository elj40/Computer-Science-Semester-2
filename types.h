#ifndef TYPES_H
#define TYPES_H

#define MAX_MAP_SIZE 100
#define MAX_FLOWER_POLLEN 100
#define MAX_HIVE_POLLINATORS 200
#define MAX_POLLEN_LEN 100

enum PollenType { FLOAT, STRING };
enum BeehiveAction { MAX, MIN, SUM, SORT };


union Pollen {
	float float_info;
	char *string_info;
};

struct Config {
	int duration;
	enum PollenType pollen_type;
	enum BeehiveAction beehive_action;
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
	char type;
	int pollinator_len;
	union Pollinator pollinators[MAX_HIVE_POLLINATORS];
};

struct Flower {
	int row;
	int col;
	int pollen_len;
	enum PollenType pollen_type;
	union Pollen pollen[MAX_FLOWER_POLLEN];
};


struct Map {
	int map_size;
	char map[MAX_MAP_SIZE][MAX_MAP_SIZE];
	int hive_len;
	int flower_len;
	// struct Hive hives[MAX_MAP_SIZE*MAX_MAP_SIZE];
	// struct Flower flowers[MAX_MAP_SIZE*MAX_MAP_SIZE];
};
#endif
