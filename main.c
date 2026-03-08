#include "robdd.h"
#include <stdio.h>
#include <string.h>

/* Affiche l'aide d'utilisation du programme */
static void print_help(const char* program_name)
{
  printf("Usage: %s [--dotview] <expression>\n", program_name);
  printf("\n");
  printf("Symboles supportes:\n");
  printf("  .  : ET\n");
  printf("  +  : OU\n");
  printf("  !  : NON\n");
  printf("  *  : XOR\n");
  printf("  ( ) : parenthèses\n");
  printf("\n");
  printf("Options:\n");
  printf("  --dotview   Genere robdd.dot et tente de generer robdd.png\n");
  printf("  --help      Affiche cette aide\n");
  printf("\n");
  printf("Exemple:\n");
  printf("  %s --dotview \"!(a*b).(c+d)\"\n", program_name);
}

int main(int argc, char** argv)
{
  int generate_dotview = 0;
  const char* expression = NULL;

  if (argc < 2)
  {
    print_help(argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; ++i)
  {
    if (strcmp(argv[i], "--help") == 0)
    {
      print_help(argv[0]);
      return 0;
    }

    if (strcmp(argv[i], "--dotview") == 0)
    {
      generate_dotview = 1;
      continue;
    }

    if (expression == NULL)
    {
      expression = argv[i];
      continue;
    }

    printf("Erreur: argument inconnu '%s'\n", argv[i]);
    print_help(argv[0]);
    return 1;
  }

  if (expression == NULL)
  {
    printf("Erreur: expression manquante\n");
    print_help(argv[0]);
    return 1;
  }

  node_t* result = parse(expression);
  print_tree(result, 0);

  if (generate_dotview)
  {
    export_tree_dot(result, "robdd.dot");
    if (render_tree_png(result, "robdd.dot", "robdd.png") == 0)
      printf("Image generee: robdd.png\n");
    else
      printf("Graphviz non disponible: DOT genere dans robdd.dot\n");
  }

  free_node(result);
  return 0;
}


