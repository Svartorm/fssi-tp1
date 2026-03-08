#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "robdd.h"

typedef struct
{
  const char* s;
  size_t i;
} parser_t;

/* Saute les espaces et caractères blancs dans l'expression */
static void skip_spaces(parser_t* p)
{
  while (p->s[p->i] != '\0' && isspace((unsigned char)p->s[p->i]))
    p->i++;
}

static node_t* parse_or(parser_t* p);

/* Crée un nœud feuille pour une variable (avec enfants 0 et 1) */
static node_t* make_var_node(char var)
{
  char label[2] = { var, '\0' };
  node_t* zero = create_node("0", NULL, NULL);
  node_t* one = create_node("1", NULL, NULL);
  return create_node(label, zero, one);
}

/* Parse une valeur primaire : variable, constante (0 ou 1), ou expression entre parenthèses */
static node_t* parse_primary(parser_t* p)
{
  skip_spaces(p);
  char ch = p->s[p->i];

  if (ch == '(')
  {
    p->i++;
    node_t* inside = parse_or(p);
    skip_spaces(p);
    if (p->s[p->i] == ')')
      p->i++;
    return inside;
  }

  if (ch == '0' || ch == '1')
  {
    p->i++;
    if (ch == '0')
      return create_node("0", NULL, NULL);
    return create_node("1", NULL, NULL);
  }

  if (isalpha((unsigned char)ch))
  {
    p->i++;
    return make_var_node(ch);
  }

  return create_node("0", NULL, NULL);
}

/* Parse une expression unaire : gère la négation (!) */
static node_t* parse_unary(parser_t* p)
{
  skip_spaces(p);
  if (p->s[p->i] == '!')
  {
    p->i++;
    node_t* operand = parse_unary(p);
    return NOT(operand);
  }

  return parse_primary(p);
}

/* Parse une expression ET (.) : priorité moyenne */
static node_t* parse_and(parser_t* p)
{
  node_t* left = parse_unary(p);

  while (1)
  {
    skip_spaces(p);
    if (p->s[p->i] != '.')
      break;

    p->i++;
    node_t* right = parse_unary(p);
    left = AND(left, right);
  }

  return left;
}

/* Parse une expression XOR (*) : priorité basse-moyenne */
static node_t* parse_xor(parser_t* p)
{
  node_t* left = parse_and(p);

  while (1)
  {
    skip_spaces(p);
    if (p->s[p->i] != '*')
      break;

    p->i++;
    node_t* right = parse_and(p);
    left = XOR(left, right);
  }

  return left;
}

/* Parse une expression OU (+) : priorité basse */
static node_t* parse_or(parser_t* p)
{
  node_t* left = parse_xor(p);

  while (1)
  {
    skip_spaces(p);
    if (p->s[p->i] != '+')
      break;

    p->i++;
    node_t* right = parse_xor(p);
    left = OR(left, right);
  }

  return left;
}

/* Parser : analyse et construit le ROBDD à partir d'une expression */
node_t* parse(const char* expression)
{
  configure_variable_order(expression);

  parser_t p;
  p.s = expression;
  p.i = 0;

  return parse_or(&p);
}

void print_tree(node_t* node, int level)
{
  if (node == NULL)
    return;

  print_tree(node->right, level + 1);
  for (int i = 0; i < level; i++)
    printf("   ");

  printf("%s\n", node->label);
  print_tree(node->left, level + 1);
}
