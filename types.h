#ifndef TYPES_H
#define TYPES_H

#define MAX_MAP_SIZE 100
#define MAX_FLOWER_POLLEN 100
#define MAX_BEE_POLLEN 200
#define MAX_HIVE_POLLINATORS 200
#define MAX_POLLEN_LEN 100

enum PollenType { FLOAT, STRING };
enum BeehiveAction { MAX, MIN, SUM, SORT };
enum BeeRole { FORAGER, SCOUT };
enum BeeType { NORMAL, HONEY, DESERT };
enum PollinatorState { WANDER, RETURN, SEEK };


union Pollen {
	float float_info;
	char *string_info;
};

struct Config {
	int duration;
	enum PollenType pollen_type;
	enum BeehiveAction beehive_action;
};
/* All types of bees have a perception range at which they can “see” flowers. If any bee except the scout bee sees a flower within its perception range that contains pollen, it will move in a direction towards the flower on the next turn(s) (as long as it is not carrying an item), with the magnitude given by the trajectory. In contrast, if a scout bee sees a flower within its perception range that contains pollen, it will immediately fly back to its hive to inform the foragers. If there are multiple flowers in the range of the bee, the bee will always go towards the flower that is closest (i.e. within less perception range), and is the bottom-most and left-most within that proximity. */ 

/* When entities know the location of their destination and can fly towards the destination, they first move diagonally in the direction of the destination until they can move in a straight line towards the destination. The distance they move is always determined by their compass. */


struct Bee {
	int row;
	int col;
	int speed;
	int perception;
	enum BeeRole role;	
	enum PollinatorState state;
	enum BeeType type;
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
	struct Hive hives[MAX_MAP_SIZE];
	struct Flower flowers[MAX_MAP_SIZE];
};

struct Trajectory {
	int distance;
	float direction;
};

#endif
