#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "robdd_internal.h"

/* Exporte récursivement un noeud au format Graphviz DOT */
void export_dot_rec(FILE* f, node_t* node, visited_entry_t** visited)
{
  if (node == NULL || already_visited(*visited, node))
    return;

  mark_visited(visited, node);

  uintptr_t id = (uintptr_t)node;
  if (is_zero(node) || is_one(node))
    fprintf(f, "  n%zu [label=\"%s\", shape=box];\n", (size_t)id, node->label);
  else
    fprintf(f, "  n%zu [label=\"%s\"];\n", (size_t)id, node->label);

  if (node->left != NULL)
  {
    fprintf(f, "  n%zu -> n%zu [label=\"0\"];\n", (size_t)id, (size_t)(uintptr_t)node->left);
    export_dot_rec(f, node->left, visited);
  }

  if (node->right != NULL)
  {
    fprintf(f, "  n%zu -> n%zu [label=\"1\"];\n", (size_t)id, (size_t)(uintptr_t)node->right);
    export_dot_rec(f, node->right, visited);
  }
}

/* Exporte le ROBDD complet au format Graphviz DOT dans un fichier */
void export_tree_dot(node_t* root, const char* dot_path)
{
  FILE* f = fopen(dot_path, "w");
  if (f == NULL)
    return;

  fprintf(f, "digraph ROBDD {\n");
  fprintf(f, "  rankdir=TB;\n");
  fprintf(f, "  node [shape=circle];\n");

  visited_entry_t* visited = NULL;
  export_dot_rec(f, root, &visited);
  free_visited_list(visited);

  fprintf(f, "}\n");
  fclose(f);
}

/* Génère une image PNG du ROBDD en utilisant Graphviz (dot command) */
int render_tree_png(node_t* root, const char* dot_path, const char* png_path)
{
  export_tree_dot(root, dot_path);

  char command[1024];
  int written = snprintf(command, sizeof(command), "dot -Tpng \"%s\" -o \"%s\"", dot_path, png_path);
  if (written < 0 || (size_t)written >= sizeof(command))
    return -1;

  return system(command);
}
