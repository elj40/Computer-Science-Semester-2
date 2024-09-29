#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "types.h"
#include "utils.h"
#include "map.h"
#include "update.h"
#include "validate.h"
#include "compass.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void test_compass(int row, int col, int speed, int size, int moves);
void test_linked_list_get_pos();
void test_linked_list();
void test_move_bee_between_cells();
void test_add_pollen();
int round_away_from_zero(float n);

int main(int argc, char* argv[]) {

	int n;
	bool b;

	printf("string_to_int:\n");
	b = string_to_int(&n, "12345");
	assert(b && "Boolean not correct");
	if (b) assert(n == 12345 && "Number not correct");
	printf(".");

	b = string_to_int(&n, "12f45");
	assert(!b && "Boolean not correct");
	if (b) assert(n == 12345 && "Number not correct");
	printf(".");

	printf("\n");
	printf("string_to_float:\n");
	float f;
	float eps = 0.00001;

	b = string_to_float(&f, "213");
	assert(b && "string_to_float failed");
	if (b) assert(f == 213.0 && "Number not correct");
	printf(".");

	b = string_to_float(&f, "213.12");
	assert(b && "string_to_float failed");
	if (b) assert(f > 213.12 - eps && f < 213.2 + eps && "Number not correct");
	printf(".");

	b = string_to_float(&f, "0.123456");
	assert(b && "string_to_float failed");
	if (b) assert(f > 0.123456 - eps && f < 0.123456 + eps && "Number not correct");
	printf(".");

	
	b = string_to_float(&f, "0.123.456");
	assert(!b && "string_to_float failed");
	if (b) assert(f > 0.123456 - eps && f < 0.123456 + eps && "Number not correct");
	printf(".");

	printf("\n");
	printf("test_compass\n");
	/* test_compass(1,1,1,10,8); */
	/* test_compass(2,1,2,10,8); */
	/* test_compass(8,8,2,10,8); */


	/* test_linked_list(); */
	/* test_move_bee_between_cells(); */
	/* test_add_pollen(); */

	test_linked_list_get_pos();

	return 0;
}
int round_away_from_zero(float n) {
	if (n < 0) return (int)-round(-n);
	return (int)round(n);
}

void test_compass(int row, int col, int speed, int size, int moves) {
	/* Tests finding the next position of an entitiy given a direction and speed */
	/* Must stay within an n by n grid where n = size */
	// Here we loop around all the axes
	// TODO: convert this into an assertable function
	//
	//
	float epsilon = 0.001;
	for (int s = 1; s < 4; s++) {
		printf("\n");
		for (int i = 0; i < moves; i++) {
			float dir = i * M_PI/4;

			float cos_dir = cos(dir);
			float sin_dir = sin(dir);
			
			int dc = 0;
			int dr = 0;

			if (sin_dir > epsilon) dr = s;
			else if(sin_dir < -epsilon) dr = -s;

			if (cos_dir > epsilon) dc = s;
			else if(cos_dir < -epsilon) dc = -s;
			
			int nc = col + dc;
			int nr = row + dr;

			nc = MIN(size-1, MAX(nc, 0));
			nr = MIN(size-1, MAX(nr, 0));

			printf("r:%d, c:%d, dist:%d, dir:%5.1f -> nr:%d, nc:%d\n",row, col, s, dir / M_PI * 180, nr, nc);
		}
	}
}

void test_linked_list() {
	BeeNode *head = NULL;

	Bee b1 = { .row=1, .col=1, .id=0 };
	Bee b2 = { .row=2, .col=2, .id=1 };
	Bee b3 = { .row=3, .col=3, .id=2 };
	
	add_bee_to_cell(&head, b1); 
	add_bee_to_cell(&head, b2); 
	add_bee_to_cell(&head, b3); 
	printf("Printing bee list\n");
	bee_print_list(head);

	remove_bee_from_cell(&head, b2);
	remove_bee_from_cell(&head, b1);
	printf("Printing bee list\n");
	bee_print_list(head);

	bee_free_linked_list(head);	
}

void test_move_bee_between_cells() {
	Cell c1, c2;
	c1.bee_head_ptr = NULL;
	c2.bee_head_ptr = NULL;

	Bee b1 = { .row=1, .col=1, .id=0 };
	Bee b2 = { .row=2, .col=2, .id=1 };
	Bee b3 = { .row=3, .col=3, .id=2 };

	add_bee_to_cell(&c1.bee_head_ptr, b1);
	printf("Iniitally in cell 1\n");
	bee_print_list(c1.bee_head_ptr);

	remove_bee_from_cell(&c1.bee_head_ptr, b1);
	add_bee_to_cell(&c2.bee_head_ptr, b1);

	printf("After in cell 1\n");
	bee_print_list(c1.bee_head_ptr);
	printf("After in cell 2\n");
	bee_print_list(c2.bee_head_ptr);

	bee_free_linked_list(c1.bee_head_ptr);	
	bee_free_linked_list(c2.bee_head_ptr);	
}

void test_add_pollen() {
	Flower f = { .pollen_len=0 };
	union Pollen pollen;

	// Use string copy here rather, that was the fix
/* 	pollen.string_info = "hello"; */

/* 	add_pollen(&f, pollen); */

/* 	pollen.string_info = "bob"; */

/* 	add_pollen(&f, pollen); */

/* 	print_flower(f); */
}

void test_linked_list_get_pos() {
	printf("\nTesting bee_linked_list_get_pos\n");
	BeeNode *head = NULL;

	Bee b0 = { .id=0 };
	Bee b1 = { .id=1 };
	Bee b2 = { .id=2 };
	Bee b3 = { .id=3 };

	add_bee_to_cell(&head, b0);
	add_bee_to_cell(&head, b1);
	add_bee_to_cell(&head, b2);

	bee_print_list(head);

	int b0_pos = bee_linked_list_get_node_pos(head, b0);
	int b1_pos = bee_linked_list_get_node_pos(head, b1);
	int b2_pos = bee_linked_list_get_node_pos(head, b2);
	int b3_pos = bee_linked_list_get_node_pos(head, b3);

	assert(b0_pos == 0 && "Did not get correct position for b0");
	printf(".");
	assert(b1_pos == 1 && "Did not get correct position for b1");
	printf(".");
	assert(b2_pos == 2 && "Did not get correct position for b2");
	printf(".");
	assert(b3_pos == -1 && "Did not get correct position for b3");
	printf(".");

}
