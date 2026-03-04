#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "robdd.h"

node_t* parse(char* expression) 
{
    // base case : if the expression is a variable, return a node with the variable as label
  if (expression[0] != '(')
  {
    return create_node((char[2]){ expression[0], 0 }, 
                       create_node("0", NULL, NULL),
                       create_node("1", NULL, NULL));
  }

  // recursive case : parse the left and right subexpressions
  size_t i = 1; // skip the first '('
  int count = 0; // count the number of parentheses
  while (i < strlen(expression)) {
      if (expression[i] == '(') {
          count++;
      } else if (expression[i] == ')') {
          count--;
      } else if ((expression[i] == '.' || expression[i] == '+' || expression[i] == '!' || expression[i] == '*') && count == 0) {
          break; // found the operator at the top level
      }
      i++;
  }
  char operator = expression[i];
  char* left_expr = strndup(expression + 1, i - 1); // left subexpression
  char* right_expr = strndup(expression + i + 1, strlen(expression) - i - 2); // right subexpression
  node_t* left_node = parse(left_expr);
  node_t* right_node = parse(right_expr);
  
  node_t* result;
  if (operator == '.') {
    result = AND(left_node, right_node);
  } else if (operator == '+') {
    result = OR(left_node, right_node);
  } else if (operator == '!') {
    result = NOT(left_node);
  } else if (operator == '*') {
    result = XOR(left_node, right_node);
  }

  free_node(left_node);
  free_node(right_node);
  free(left_expr);
  free(right_expr);
  return result;
}

void print_tree(node_t* node, int level) 
{ // TODO: use lib to display the tree
    if (node == NULL) {
        return;
    }
    print_tree(node->right, level + 1);
    for (int i = 0; i < level; i++) {
        printf("   ");
    }
    printf("%s\n", node->label);
    print_tree(node->left, level + 1);
}

