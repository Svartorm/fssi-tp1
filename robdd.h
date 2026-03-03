#ifndef ROBDD_H
#define ROBDD_H

#define TABLE_UNICITE_MAX_SIZE 256

typedef struct node 
{
  char* label;

  struct node* left;
  struct node* right;

} node_t;

extern int tu_index;
extern const char table_unicite[200];

#endif // ROBDD_H
