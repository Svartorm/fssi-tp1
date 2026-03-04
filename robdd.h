#ifndef ROBDD_H
#define ROBDD_H

/* Definitions --------------------------------------------------------------*/
#define TABLE_UNICITE_MAX_SIZE 256

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
/**/


/* parser.c */
node_t* parse(char* expression);
void print_tree(node_t* node, int level);
/**/ 

/* Globals ------------------------------------------------------------------*/
extern char* alphabet[5];
extern int tu_index;
extern const char table_unicite[200];

/**/
#endif // ROBDD_H
