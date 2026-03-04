#include <stdlib.h>
#include <string.h>

#include "robdd.h"

/* Variables ----------------------------------------------------------------*/
char* alphabet[5] = {
  "a",
  "b",
  "c",
  "d",
  "e",
};

/* Functions ----------------------------------------------------------------*/

node_t* create_node(const char* label, node_t* left, node_t* right)
{
  node_t* node = malloc(sizeof(node_t));
  node->label = strdup(label);
  node->left  = left;
  node->right = right;

  return node; 
}

node_t* copy_node(node_t* n)
{
  if (n == NULL)
    return NULL;
  return create_node(n->label, copy_node(n->left), copy_node(n->right));
}

void free_node(node_t* n)
{
  if (n == NULL)
    return;
  
  free_node(n->left);
  free_node(n->right);
  free(n->label);
  free(n);
  
  return;
}

bool is_zero(node_t* n)
{
  if (strcmp(n->label, "0") == 0)
    return TRUE;
  return FALSE;
}

bool is_one(node_t* n)
{
  if (strcmp(n->label, "1") == 0)
    return TRUE;
  return FALSE;
}

bool is_greater(node_t* a, node_t* b)
{
  for(int i = 0; i < 5; i++)
  {
    if (strcmp(alphabet[i], a->label) == 0)
    {
      if (strcmp(alphabet[i], b->label) == 0)
        return FALSE;
      else 
        return TRUE;
    }
    if (strcmp(alphabet[i], b->label) == 0)
      return FALSE;
  }

  /**
   * Label not in alphabet.
   * Should NEVER happen !!
   */
  return FALSE;
}

node_t* AND(node_t *a, node_t *b) 
{
  /**
   * Base case: AND 
   *  - if any is zero then return zero 
   *  - if one of them is one then return the other
   *  - if both are one then a is one and we return b (also one)
   *  In any other case we compute the children 
   */
  if (is_zero(a) || is_zero(b))
      return create_node("0", NULL, NULL);  

  if (is_one(a)) 
      return copy_node(b);  
  
  if (is_one(b)) 
      return copy_node(a);  
  
  /**
   * Current node will take the highest label in the ranking and go through its
   * children. If equal, go through both.
   */
  node_t* current = malloc(sizeof(node_t));
  
  if (is_greater(a, b)) 
  {
    current->label = strdup(a->label);

    current->left  = AND(a->left, b);
    current->right = AND(a->right, b);
  } 
  else if (is_greater(b, a))
  {
    current->label = strdup(b->label);

    current->left = AND(a, b->left);
    current->right = AND(a, b->right);
  } 
  else
  {
    current->label = strdup(a->label);

    current->left = AND(a->left, b->left);
    current->right = AND(a->right, b->right);
  }

  return current;
}

//TODO replace OR by the other one and fix other operations to feat this new implementation
node_t *OR(node_t *a, node_t *b) 
{
  /**
   * Base case: OR
   *  - if any is one then return one 
   *  - if one of them is zero then return the other
   *  - if both are zero then a is zero and we return b (also zero)
   *  In any other case we compute the children 
   */
  if (is_one(a) || is_one(b))
      return create_node("0", NULL, NULL);  

  if (is_zero(a)) 
      return copy_node(b);  
  
  if (is_zero(b)) 
      return copy_node(a);  
  
  /**
   * Current node will take the highest label in the ranking and go through its
   * children. If equal, go through both.
   */
  node_t* current = malloc(sizeof(node_t));
  
  if (is_greater(a, b)) 
  {
    current->label = strdup(a->label);

    current->left  = OR(a->left, b);
    current->right = OR(a->right, b);
  } 
  else if (is_greater(b, a))
  {
    current->label = strdup(b->label);

    current->left = OR(a, b->left);
    current->right = OR(a, b->right);
  } 
  else
  {
    current->label = strdup(a->label);

    current->left = OR(a->left, b->left);
    current->right = OR(a->right, b->right);
  }

  return current;
};

node_t *NOT(node_t *n) 
{
  /* Base case : if n is zero, return one */
  if (is_zero(n))
    return create_node("1", NULL, NULL);
  
  /* Base case : if n is one, return zero */
  if (is_one(n))
    return create_node("0", NULL, NULL);

  node_t* current = malloc(sizeof(node_t));

  current->label = strdup(n->label);

  current->left = NOT(n->left);
  current->right = NOT(n->right);

  return current;
};

node_t* XOR(node_t *a, node_t *b) 
{
  /* XOR can be implemented using AND, OR and NOT */
  return OR(AND(a, NOT(b)), AND(NOT(a), b));
};

/*---------------------------------------------------------------------------*/
