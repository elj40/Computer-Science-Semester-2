#ifndef TYPES_H
#define TYPES_H

#define MAX_MAP_SIZE 100
#define MAX_FLOWER_POLLEN 100
#define MAX_BEE_POLLEN 200
#define MAX_HIVE_POLLINATORS 200
#define MAX_POLLEN_LEN 100

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define EPS 0.01

enum PollenType { FLOAT, STRING };
enum BeehiveAction { MAX, MIN, SUM, SORT };
enum BeeRole { FORAGER, SCOUT };
enum BeeType { NORMAL, HONEY, DESERT };
enum PollinatorState { WANDER, RETURN, SEEK };


union Pollen {
	float float_info;
	char *string_info;
};

typedef struct {
	int duration;
	enum PollenType pollen_type;
	enum BeehiveAction beehive_action;
} Config;
/* All types of bees have a perception range at which they can “see” flowers. If any bee except the scout bee sees a flower within its perception range that contains pollen, it will move in a direction towards the flower on the next turn(s) (as long as it is not carrying an item), with the magnitude given by the trajectory. In contrast, if a scout bee sees a flower within its perception range that contains pollen, it will immediately fly back to its hive to inform the foragers. If there are multiple flowers in the range of the bee, the bee will always go towards the flower that is closest (i.e. within less perception range), and is the bottom-most and left-most within that proximity. */ 

/* When entities know the location of their destination and can fly towards the destination, they first move diagonally in the direction of the destination until they can move in a straight line towards the destination. The distance they move is always determined by their compass. */

typedef struct {
	int row;
	int col;
	char type;
} Hive;

typedef struct Bee {
	int id;
	int row;
	int col;
	int speed;
	int perception;
	Hive *hive_ptr;
	struct Bee *previous_ptr;
	struct Bee *next_ptr;
	enum BeeRole role;	
	enum PollinatorState state;
	enum BeeType type;
} Bee;


typedef struct {
	int id;
	int row;
	int col;
	int speed;
	Hive *hive_ptr;
} Wasp;

typedef struct BeeNode {
	Bee bee;
	struct BeeNode *next_ptr;
} BeeNode;

typedef struct WaspNode {
	Wasp wasp;
	struct WaspNode *next_ptr;
} WaspNode;

union Pollinator {
	Bee bee;
	Wasp wasp;
};
typedef struct {
	int row;
	int col;
	int pollen_len;
	enum PollenType pollen_type;
	union Pollen pollen[MAX_FLOWER_POLLEN];
} Flower;

typedef struct {
	int distance;
	float direction;
} Trajectory;

#endif
