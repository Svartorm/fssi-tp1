#ifndef ROBDD_H
#define ROBDD_H

#include <stddef.h>

/* Definitions --------------------------------------------------------------*/
#define TABLE_UNICITE_MAX_SIZE 256
#define MAX_VARIABLES 52

#define TRUE  1
#define FALSE 0
typedef int bool;

typedef struct node 
{
  char* label;

  struct node* left;
  struct node* right;

} node_t;

/* Functions ----------------------------------------------------------------*/

/* robdd.c */
node_t* create_node(const char* label, node_t* left, node_t* right);
node_t* copy_node(node_t* n);
void free_node(node_t* n);

bool is_zero(node_t* n);
bool is_one(node_t* n);
bool is_greater(node_t* a, node_t* b);

node_t* AND(node_t *a, node_t *b);
node_t *OR(node_t *a, node_t *b);
node_t *NOT(node_t *n);
node_t* XOR(node_t *a, node_t *b);
void configure_variable_order(const char* expression);
void reset_unique_table(void);
void export_tree_dot(node_t* root, const char* dot_path);
int render_tree_png(node_t* root, const char* dot_path, const char* png_path);
/**/


/* parser.c */
node_t* parse(const char* expression);
void print_tree(node_t* node, int level);
/**/ 

/* Globals ------------------------------------------------------------------*/
extern char* alphabet[MAX_VARIABLES];
extern int tu_index;
extern const char table_unicite[200];

/**/
#endif // ROBDD_H
