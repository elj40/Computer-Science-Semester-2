#ifndef TYPES_H
#define TYPES_H

enum PollenType { FLOAT, STRING };
enum BeehiveAction { MAX, MIN, SUM, SORT };

struct Config {
	int map_size;
	int duration;
	enum PollenType pollen_type;
	enum BeehiveAction beehive_action;
};

#endif
