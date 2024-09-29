#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include "types.h"

int bee_linked_list_get_node_pos(BeeNode * head, Bee bee);
int wasp_linked_list_len(WaspNode *head);
int bee_linked_list_len(BeeNode *head);
void split_string(char *buffer[], int buf_len, char *string, const char *delims);
bool string_to_int(int *n, char *s);
bool string_to_float(float *n, char *s);
void invalid_object_setup(int l);

#endif

