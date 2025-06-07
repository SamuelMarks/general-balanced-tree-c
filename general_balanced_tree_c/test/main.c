#include <_stdlib.h>
#include <stdio.h>

#include <general_balanced_tree_c.h>

void Help() {
  printf("Write\n");
  printf(" i x to insert integer x,\n");
  printf(" d x to delete x,\n");
  printf(" c to clear the tree,\n");
  printf(" b to balance the entire tree,\n");
  printf(" q to quit, \n");
  printf(" ? to see this message. \n");
}

/*-----------------------*/
/* A small test program. */
/*-----------------------*/

int Go() {
  dictptr thedict;
  char command;
  ky_type x;
  noderef temp;
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
      temp = lookup(thedict, x);
      if (temp) {
        if (temp->key == x) {
          printf("No insertion, %d is already present.\n", x);
          break;
        }
      }
      {
        temp = insert(thedict, x, 0);
        Display(thedict->t, 0L);
        if (temp->key != x)
          printf("something is wrong.\n");
        temp = lookup(thedict, x);
        if (temp->key != x)
          printf("something is wrong.\n");
      }
      break;

    case 'd':
      scanf("%d%*[^\n]", &x);
      getchar();
      temp = lookup(thedict, x);
      if (!temp) {
        printf("Sorry, could not find %d\n", x);
      } else if (temp->key != x) {
        printf("Sorry, could not find %d\n", x);
      } else {
        delete(thedict, x);
        Display(thedict->t, 0L);
      }
      break;

    case 'b':
      scanf("%*[^\n]");
      getchar();
      PerfectBalance(&thedict->t, thedict->weight);
      Display(thedict->t, 0L);
      break;

    case 'c':
      scanf("%*[^\n]");
      getchar();
      clear(thedict);
      Display(thedict->t, 0L);
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
