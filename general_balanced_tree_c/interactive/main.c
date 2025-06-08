#include <stdio.h>
#include <stdlib.h>

#include <general_balanced_tree_c.h>

void Help() {
  puts("Write\n"
       " i x to insert integer x,\n"
       " d x to delete x,\n"
       " c to clear the tree,\n"
       " b to balance the entire tree,\n"
       " q to quit, \n"
       " ? to see this message. \n");
}

/*-----------------------*/
/* A small test program. */
/*-----------------------*/

int Go() {
  gbt_dictptr thedict;
  char command;
  gbt_ky_type x;
  gbt_noderef temp;
  const int rc = EXIT_SUCCESS;

  thedict = construct_dict();
  Help();
  do {
    printf("treeweight %ld \n", thedict->weight);
    printf("Enter command, write ? for help.\n");
    command = getchar();
    if (command == '\n')
      command = ' ';
    switch (command) {

    case 'i':
      scanf("%d%*[^\n]", &x);
      getchar();
      temp = gbt_lookup(thedict, x);
      if (temp) {
        if (temp->key == x) {
          printf("No insertion, %d is already present.\n", x);
          break;
        }
      }
      {
        temp = gbt_insert(thedict, x, 0);
        gbt_Display(thedict->t, 0L);
        if (temp->key != x)
          printf("something is wrong.\n");
        temp = gbt_lookup(thedict, x);
        if (temp->key != x)
          printf("something is wrong.\n");
      }
      break;

    case 'd':
      scanf("%d%*[^\n]", &x);
      getchar();
      temp = gbt_lookup(thedict, x);
      if (!temp) {
        printf("Sorry, could not find %d\n", x);
      } else if (temp->key != x) {
        printf("Sorry, could not find %d\n", x);
      } else {
        gbt_delete(thedict, x);
        gbt_Display(thedict->t, 0L);
      }
      break;

    case 'b':
      scanf("%*[^\n]");
      getchar();
      gbt_PerfectBalance(&thedict->t, thedict->weight);
      gbt_Display(thedict->t, 0L);
      break;

    case 'c':
      scanf("%*[^\n]");
      getchar();
      gbt_clear(thedict);
      gbt_Display(thedict->t, 0L);
      break;

    case 'q':
      printf("  Bye\n");
      break;

    case '?':
      scanf("%*[^\n]");
      getchar();
      Help();
      break;

    default:
      scanf("%*[^\n]");
      getchar();
      printf("I don''t understand.\n");
      printf("Write ? for help. \n");
      break;
    }
  } while (command != 'q');
  return rc;
}

int main(int argc, char *argv[]) { return Go(); }
