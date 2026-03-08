#include <stdlib.h>
#include <string.h>
#include "robdd_internal.h"

/* Vérifie si deux noeuds sont équivalents (même label et enfants) */
int node_equals(const node_t* n1, const char* label, const node_t* left, const node_t* right)
{
  return n1->left == left && n1->right == right && strcmp(n1->label, label) == 0;
}

/* Cherche un noeud dans la table d'unicité, retourne NULL s'il n'existe pas */
node_t* find_or_create_node(const char* label, node_t* left, node_t* right)
{
  unique_entry_t* entry = g_unique_table;
  while (entry != NULL)
  {
    if (node_equals(entry->node, label, left, right))
      return entry->node;
    entry = entry->next;
  }
  return NULL;
}

/* Supprime un noeud de la table d'unicité */
void remove_from_unique_table(node_t* node)
{
  unique_entry_t* prev = NULL;
  unique_entry_t* cur = g_unique_table;

  while (cur != NULL)
  {
    if (cur->node == node)
    {
      if (prev == NULL)
        g_unique_table = cur->next;
      else
        prev->next = cur->next;
      free(cur);
      return;
    }
    prev = cur;
    cur = cur->next;
  }
}

/* Ajoute un noeud à la table d'unicité */
void add_to_unique_table(node_t* node)
{
  unique_entry_t* new_entry = malloc(sizeof(unique_entry_t));
  new_entry->node = node;
  new_entry->next = g_unique_table;
  g_unique_table = new_entry;
  tu_index++;
}

/* Vérifie si un noeud a déjà été visité */
int already_visited(visited_entry_t* visited, node_t* node)
{
  while (visited != NULL)
  {
    if (visited->node == node)
      return 1;
    visited = visited->next;
  }
  return 0;
}

/* Marque un noeud comme visité */
void mark_visited(visited_entry_t** visited, node_t* node)
{
  visited_entry_t* v = malloc(sizeof(visited_entry_t));
  v->node = node;
  v->next = *visited;
  *visited = v;
}

/* Libère la liste des noeuds visités */
void free_visited_list(visited_entry_t* visited)
{
  while (visited != NULL)
  {
    visited_entry_t* next = visited->next;
    free(visited);
    visited = next;
  }
}
