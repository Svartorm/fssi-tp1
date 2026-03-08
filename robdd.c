#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "robdd.h"
#include "robdd_internal.h"

/* Variables ----------------------------------------------------------------*/
char* alphabet[MAX_VARIABLES] = {0};
int tu_index = 0;
const char table_unicite[200] = "";

char g_alphabet_storage[MAX_VARIABLES][2] = {{0}};
int g_var_rank[256] = {0};
unique_entry_t* g_unique_table = NULL;

/* Initialise l'ordre des variables par défaut (a-z, A-Z) */
static void init_default_order(void)
{
  static int initialized = 0;
  if (initialized)
    return;

  const char* defaults = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  for (int i = 0; i < MAX_VARIABLES; ++i)
  {
    g_alphabet_storage[i][0] = defaults[i];
    g_alphabet_storage[i][1] = '\0';
    alphabet[i] = g_alphabet_storage[i];
  }

  for (int i = 0; i < 256; ++i)
    g_var_rank[i] = 100000;

  for (int i = 0; i < MAX_VARIABLES; ++i)
    g_var_rank[(unsigned char)alphabet[i][0]] = i;

  initialized = 1;
}

/* Retourne le rang (priorité) d'une variable pour l'ordre */
int rank_of_label(const char* label)
{
  if (label == NULL || label[0] == '\0')
    return 100000;

  if (strcmp(label, "0") == 0 || strcmp(label, "1") == 0)
    return 100000;

  return g_var_rank[(unsigned char)label[0]];
}

/* Libère récursivement un nœud et ses descendants (évite les cycles) */
static void free_node_rec(node_t* node, visited_entry_t** visited)
{
  if (node == NULL || already_visited(*visited, node))
    return;

  mark_visited(visited, node);
  free_node_rec(node->left, visited);
  free_node_rec(node->right, visited);

  remove_from_unique_table(node);
  free(node->label);
  free(node);
}

/* Crée un nœud ROBDD (ou le récupère de la table d'unicité s'il existe) */
node_t* create_node(const char* label, node_t* left, node_t* right)
{
  init_default_order();

  node_t* existing = find_or_create_node(label, left, right);
  if (existing != NULL)
    return existing;

  node_t* node = malloc(sizeof(node_t));
  node->label = strdup(label);
  node->left = left;
  node->right = right;

  add_to_unique_table(node);
  return node;
}

/* Retourne une copie du nœud (simple retour du pointeur) */
node_t* copy_node(node_t* n)
{
  return n;
}

/* Libère un nœud et tous ses descendants */
void free_node(node_t* n)
{
  visited_entry_t* visited = NULL;
  free_node_rec(n, &visited);
  free_visited_list(visited);
}

/* Réinitialise la table d'unicité (vide tous les nœuds) */
void reset_unique_table(void)
{
  unique_entry_t* entry = g_unique_table;
  while (entry != NULL)
  {
    unique_entry_t* next = entry->next;
    free(entry->node->label);
    free(entry->node);
    free(entry);
    entry = next;
  }
  g_unique_table = NULL;
  tu_index = 0;
}

/* Teste si un nœud est le terminal 0 */
bool is_zero(node_t* n)
{
  return (n != NULL && strcmp(n->label, "0") == 0) ? TRUE : FALSE;
}

/* Teste si un nœud est le terminal 1 */
bool is_one(node_t* n)
{
  return (n != NULL && strcmp(n->label, "1") == 0) ? TRUE : FALSE;
}

/* Teste si le rang de 'a' est plus grand (plus proche de la racine) que celui de 'b' */
bool is_greater(node_t* a, node_t* b)
{
  if (a == NULL || b == NULL)
    return FALSE;
  return rank_of_label(a->label) < rank_of_label(b->label);
}

/* Configure l'ordre des variables en fonction des variables présentes dans l'expression */
void configure_variable_order(const char* expression)
{
  init_default_order();

  int order[256] = {-1};
  int next_order = 0;
  int placed[256] = {0};

  for (size_t i = 0; expression[i] != '\0' && next_order < MAX_VARIABLES; ++i)
  {
    unsigned char c = (unsigned char)expression[i];
    if (isalpha(c) && !placed[c])
    {
      placed[c] = 1;
      order[c] = next_order++;
    }
  }

  int pos = 0;
  const char* defaults = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

  for (int i = 0; defaults[i] != '\0' && pos < MAX_VARIABLES; ++i)
  {
    unsigned char c = (unsigned char)defaults[i];
    if (order[c] >= 0)
    {
      g_alphabet_storage[pos][0] = (char)c;
      g_alphabet_storage[pos][1] = '\0';
      alphabet[pos] = g_alphabet_storage[pos];
      pos++;
    }
  }

  for (int i = 0; defaults[i] != '\0' && pos < MAX_VARIABLES; ++i)
  {
    unsigned char c = (unsigned char)defaults[i];
    if (order[c] < 0)
    {
      g_alphabet_storage[pos][0] = (char)c;
      g_alphabet_storage[pos][1] = '\0';
      alphabet[pos] = g_alphabet_storage[pos];
      pos++;
    }
  }

  for (int i = 0; i < 256; ++i)
    g_var_rank[i] = 100000;

  for (int i = 0; i < MAX_VARIABLES; ++i)
    g_var_rank[(unsigned char)alphabet[i][0]] = i;
}
