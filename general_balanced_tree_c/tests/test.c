#include <greatest.h>

#include "test_general_balanced_tree_c.h"

/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(general_balanced_tree_c_suite);
  GREATEST_MAIN_END();
}
