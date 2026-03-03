#ifndef ROBDD_H
#define ROBDD_H

struct node 
{
  char* label;

  struct node* left;
  struct node* right;
}

const char table_unicite[200];

#endif // ROBDD_H
