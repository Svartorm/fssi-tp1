#include "robdd.h"

node_t* create_node(char* label)
{
  // alloc node(label, 0, 1)
  return NULL; 
}

node_t* OR(node_t* A, node_t* B)
{
  if (A->label == "0" && B->label == "0") => "0";
  if (A->left == "1" || B->label == "1") => "1";

  // Sort by order
  char* label = A->label;
  if (B->label > A->label)
    label = B->label;
    

  if (A > B)
  {
    label = A->label;
    l = OR(A->left, B);
    r = OR(A->right, B);
  }
  else if (B > A)
  {
    label = B->label;
    l = OR(A, B->left);
    r = OR(A, B->right);
  }
  else 
  {
    label = 
    l = OR(A.left, B->left);
    r = OR(A->right, B->right);
  }
  

  return create_node(label, l, r);
  // Simplification rules 
}
