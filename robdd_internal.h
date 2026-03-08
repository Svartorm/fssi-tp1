#ifndef ROBDD_INTERNAL_H
#define ROBDD_INTERNAL_H

#include "robdd.h"
#include <stdint.h>
#include <stdio.h>

/* Internal types for ROBDD implementation */

typedef struct unique_entry
{
  node_t* node;
  struct unique_entry* next;
} unique_entry_t;

typedef struct visited_entry
{
  node_t* node;
  struct visited_entry* next;
} visited_entry_t;

/* Global variables shared across modules */
extern char* alphabet[MAX_VARIABLES];
extern int tu_index;
extern char g_alphabet_storage[MAX_VARIABLES][2];
extern int g_var_rank[256];
extern unique_entry_t* g_unique_table;

/* Function declarations for cross-module use */
int rank_of_label(const char* label);
node_t* find_or_create_node(const char* label, node_t* left, node_t* right);
void remove_from_unique_table(node_t* node);
void add_to_unique_table(node_t* node);
int already_visited(visited_entry_t* visited, node_t* node);
void mark_visited(visited_entry_t** visited, node_t* node);
void free_visited_list(visited_entry_t* visited);
void export_dot_rec(FILE* f, node_t* node, visited_entry_t** visited);

#endif
