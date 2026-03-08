#include <stdlib.h>
#include <string.h>
#include "robdd_internal.h"

/* Réduit un noeud : si les deux enfants sont identiques, retourne l'enfant */
static node_t* reduce_node(const char* label, node_t* left, node_t* right)
{
  if (left == right)
    return left;
  return create_node(label, left, right);
}

/* Opération ET (AND) : expansion de Shannon sur la plus haute variable */
node_t* AND(node_t *a, node_t *b)
{
  node_t* zero = create_node("0", NULL, NULL);
  node_t* one = create_node("1", NULL, NULL);

  if (is_zero(a) || is_zero(b))
    return zero;
  if (is_one(a))
    return b;
  if (is_one(b))
    return a;
  if (a == b)
    return a;

  const char* label;
  node_t *a_low, *a_high, *b_low, *b_high;

  if (is_greater(a, b))
  {
    label = a->label;
    a_low = a->left;
    a_high = a->right;
    b_low = b;
    b_high = b;
  }
  else if (is_greater(b, a))
  {
    label = b->label;
    a_low = a;
    a_high = a;
    b_low = b->left;
    b_high = b->right;
  }
  else
  {
    label = a->label;
    a_low = a->left;
    a_high = a->right;
    b_low = b->left;
    b_high = b->right;
  }

  node_t* left = AND(a_low, b_low);
  node_t* right = AND(a_high, b_high);
  (void)one;
  return reduce_node(label, left, right);
}

/* Opération OU (OR) : expansion de Shannon sur la plus haute variable */
node_t* OR(node_t *a, node_t *b)
{
  node_t* zero = create_node("0", NULL, NULL);
  node_t* one = create_node("1", NULL, NULL);

  if (is_one(a) || is_one(b))
    return one;
  if (is_zero(a))
    return b;
  if (is_zero(b))
    return a;
  if (a == b)
    return a;

  const char* label;
  node_t *a_low, *a_high, *b_low, *b_high;

  if (is_greater(a, b))
  {
    label = a->label;
    a_low = a->left;
    a_high = a->right;
    b_low = b;
    b_high = b;
  }
  else if (is_greater(b, a))
  {
    label = b->label;
    a_low = a;
    a_high = a;
    b_low = b->left;
    b_high = b->right;
  }
  else
  {
    label = a->label;
    a_low = a->left;
    a_high = a->right;
    b_low = b->left;
    b_high = b->right;
  }

  node_t* left = OR(a_low, b_low);
  node_t* right = OR(a_high, b_high);
  (void)zero;
  return reduce_node(label, left, right);
}

/* Opération NON (NOT) : inverse les enfants gauche et droit */
node_t* NOT(node_t *n)
{
  if (is_zero(n))
    return create_node("1", NULL, NULL);
  if (is_one(n))
    return create_node("0", NULL, NULL);

  node_t* left = NOT(n->left);
  node_t* right = NOT(n->right);
  return reduce_node(n->label, left, right);
}

/* Opération XOR (OU exclusif) : expansion de Shannon sur la plus haute variable */
node_t* XOR(node_t *a, node_t *b)
{
  node_t* zero = create_node("0", NULL, NULL);

  if (a == b)
    return zero;
  if (is_zero(a))
    return b;
  if (is_zero(b))
    return a;
  if (is_one(a))
    return NOT(b);
  if (is_one(b))
    return NOT(a);

  const char* label;
  node_t *a_low, *a_high, *b_low, *b_high;

  if (is_greater(a, b))
  {
    label = a->label;
    a_low = a->left;
    a_high = a->right;
    b_low = b;
    b_high = b;
  }
  else if (is_greater(b, a))
  {
    label = b->label;
    a_low = a;
    a_high = a;
    b_low = b->left;
    b_high = b->right;
  }
  else
  {
    label = a->label;
    a_low = a->left;
    a_high = a->right;
    b_low = b->left;
    b_high = b->right;
  }

  node_t* left = XOR(a_low, b_low);
  node_t* right = XOR(a_high, b_high);
  return reduce_node(label, left, right);
}
