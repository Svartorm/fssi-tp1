#include "robdd.h"

int main() {
  char* expression = "(a.b)"; // example expression
  node_t* result = parse(expression);
  // print the result
  print_tree(result, 0);
  
  free_node(result);
  return 0;
}


