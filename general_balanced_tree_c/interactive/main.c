#include <stdio.h>
#include <stdlib.h>

#include <general_balanced_tree_c.h>
#ifdef _MSC_VER
#define NUM_FMT "zu"
#else
#define NUM_FMT "ld"
#endif

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
  struct gbt_dict *thedict;
  char command;
  gbt_ky_type x;
  struct gbt_node *temp;
  const int rc = EXIT_SUCCESS;

  thedict = gbt_construct_dict();
  Help();
  do {
    printf("treeweight %" NUM_FMT " \n", thedict->weight);
    printf("Enter command, write ? for help.\n");
    command = getchar();
    if (command == '\n')
      command = ' ';
    switch (command) {

    case 'i':
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER) ||                         \
    defined(__STDC_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
    {
      if (scanf_s("%d", &x) == 1) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
      }
    }
#else
      scanf("%d%*[^\n]", &x);
#endif
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
        gbt_Display(thedict, thedict->t, 0L);
        if (temp->key != x)
          printf("something is wrong.\n");
        temp = gbt_lookup(thedict, x);
        if (temp->key != x)
          printf("something is wrong.\n");
      }
      break;

    case 'd':
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER) ||                         \
    defined(__STDC_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
    {
      if (scanf_s("%d", &x) == 1) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
      }
    }
#else
      scanf("%d%*[^\n]", &x);
#endif
      getchar();
      temp = gbt_lookup(thedict, x);
      if (!temp) {
        printf("Sorry, could not find %d\n", x);
      } else if (temp->key != x) {
        printf("Sorry, could not find %d\n", x);
      } else {
        gbt_delete(thedict, x);
        gbt_Display(thedict, thedict->t, 0L);
      }
      break;

    case 'b':
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER) ||                         \
    defined(__STDC_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
    {
      int ch;
      while ((ch = getchar()) != '\n' && ch != EOF) {
      }
    }
#else
      scanf("%*[^\n]");
#endif
      getchar();
      gbt_PerfectBalance(&thedict->t, thedict->weight);
      gbt_Display(thedict, thedict->t, 0L);
      break;

    case 'c':
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER) ||                         \
    defined(__STDC_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
    {
      int ch;
      while ((ch = getchar()) != '\n' && ch != EOF) {
      }
    }
#else
      scanf("%*[^\n]");
#endif
      getchar();
      gbt_clear(thedict);
      gbt_Display(thedict, thedict->t, 0L);
      break;

    case 'q':
      printf("  Bye\n");
      break;

    case '?':
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER) ||                         \
    defined(__STDC_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
    {
      int ch;
      while ((ch = getchar()) != '\n' && ch != EOF) {
      }
    }
#else
      scanf("%*[^\n]");
#endif
      getchar();
      Help();
      break;

    default:
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER) ||                         \
    defined(__STDC_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
    {
      int ch;
      while ((ch = getchar()) != '\n' && ch != EOF) {
      }
    }
#else
      scanf("%*[^\n]");
#endif
      getchar();
      printf("I don''t understand.\n");
      printf("Write ? for help. \n");
      break;
    }
  } while (command != 'q');
  return rc;
}

int main(int argc, char *argv[]) { return Go(); }
