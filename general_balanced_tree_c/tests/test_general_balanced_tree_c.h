#ifndef TEST_GENERAL_BALANCED_TREE_C_H
#define TEST_GENERAL_BALANCED_TREE_C_H

#ifdef __cplusplus
extern "GBT_C" {
#endif /* __cplusplus */

#include <general_balanced_tree_c.h>
#include <greatest.h>

/* Helper to insert multiple keys, setting data = key */
static void insert_keys(const gbt_dictptr dict, int keys[],
                        const size_t count) {
  size_t i;
  for (i = 0; i < count; i++) {
    gbt_insert(dict, keys[i], keys[i]);
  }
}

/* Test insertion, lookup, and gbt_size */
TEST general_balanced_tree_insert_lookup_size(void) {
  const gbt_dictptr dict = construct_dict();
  ASSERT(dict != NULL);
  ASSERT(gbt_size(dict) == 0);

  {
    const int keys[] = {10, 20, 30}; /* ascending order */
    insert_keys(dict, keys, sizeof(keys) / sizeof(keys[0]));
    ASSERT(gbt_size(dict) == 3);

    size_t i;
    for (i = 0; i < 3; i++) {
      gbt_noderef n = lookup(dict, keys[i]);
      ASSERT(n != NULL);
      ASSERT(gbt_keyval(dict, n) == keys[i]);
      ASSERT(*gbt_infoval(dict, n) == keys[i]);
    }

    {
      const gbt_noderef n = lookup(dict, 999);
      ASSERT(n == NULL);
    }
  }

  gbt_destruct_dict(dict);
  PASS();
}

/* Test duplicate inserts don't overwrite */
TEST general_balanced_tree_duplicate_insert(void) {
  const gbt_dictptr dict = construct_dict();
  ASSERT(dict != NULL);

  {
    gbt_noderef n1 = gbt_insert(dict, 42, 100);
    ASSERT(n1 != NULL);
    ASSERT(gbt_keyval(dict, n1) == 42);
    ASSERT(*gbt_infoval(dict, n1) == 100);

    {
      const gbt_noderef n2 = gbt_insert(dict, 42, 200);
      ASSERT(n2 == n1);                      /* should be same node */
      ASSERT(*gbt_infoval(dict, n2) == 100); /* data unchanged */
    }
  }

  gbt_destruct_dict(dict);
  PASS();
}

/* Test deletion of keys */
TEST general_balanced_tree_delete(void) {
  const gbt_dictptr dict = construct_dict();
  ASSERT(dict != NULL);

  {
    const int keys[] = {5, 15, 25, 35};
    insert_keys(dict, keys, sizeof(keys) / sizeof(keys[0]));
    ASSERT(gbt_size(dict) == 4);

    gbt_delete(dict, 15);
    ASSERT(gbt_size(dict) == 3);

    {
      const gbt_noderef n = lookup(dict, 15);
      ASSERT(n == NULL);
    }

    gbt_delete(dict, 9999); /* deleting non-existing key */
    ASSERT(gbt_size(dict) == 3);

    gbt_delete(dict, 5);
    gbt_delete(dict, 25);
    gbt_delete(dict, 35);
    ASSERT(gbt_size(dict) == 0);
  }

  gbt_destruct_dict(dict);
  PASS();
}

/* Test clear function */
TEST general_balanced_tree_clear(void) {
  const gbt_dictptr dict = construct_dict();
  ASSERT(dict != NULL);

  {
    const int keys[] = {1, 2, 3, 4, 5};
    insert_keys(dict, keys, sizeof(keys) / sizeof(keys[0]));
    ASSERT(gbt_size(dict) == 5);

    gbt_clear(dict);
    ASSERT(gbt_size(dict) == 0);

    ASSERT(lookup(dict, 1) == NULL);
    ASSERT(lookup(dict, 5) == NULL);
  }

  gbt_destruct_dict(dict);
  PASS();
}

/* Test perfect balance on tree */
TEST general_balanced_tree_perfect_balance(void) {
  const gbt_dictptr dict = construct_dict();
  ASSERT(dict != NULL);

  {
    const int keys[] = {20, 30, 40, 50, 60, 70, 80}; /* ascending */
    insert_keys(dict, keys, sizeof(keys) / sizeof(keys[0]));
    ASSERT(gbt_size(dict) == 7);

    gbt_PerfectBalance(&(dict->t), dict->weight);

    {
      size_t i;
      for (i = 0; i < 7; i++) {
        gbt_noderef n = lookup(dict, keys[i]);
        ASSERT(n != NULL);
        ASSERT(gbt_keyval(dict, n) == keys[i]);
      }
    }
  }

  gbt_destruct_dict(dict);
  PASS();
}

/* Test inserting many keys and deleting some at random */
TEST general_balanced_tree_large_insert_delete(void) {
  const gbt_dictptr dict = construct_dict();
  size_t i;
  int keys[100];
  ASSERT(dict != NULL);

  /* Insert 100 keys */
  for (i = 0; i < 100; i++) {
    keys[i] = i * 2;
  }
  insert_keys(dict, keys, 100);
  ASSERT_EQ(gbt_size(dict), 100);

  /* Delete all even-keys divisible by 10 */
  for (i = 0; i < 100; i++) {
    if (keys[i] % 10 == 0)
      gbt_delete(dict, keys[i]);
  }
  {
    /* since every 10th even number: 0, 10, 20, ... */
    const size_t expected = 100 - (100 / 5);
    ASSERT_EQ(gbt_size(dict), expected);
  }

  /* Check lookups */
  for (i = 0; i < 100; i++) {
    const gbt_noderef n = lookup(dict, keys[i]);
    if (keys[i] % 10 == 0)
      ASSERT(n == NULL);
    else {
      ASSERT(n != NULL);
      ASSERT_EQ(keys[i], gbt_keyval(dict, n));
    }
  }

  gbt_destruct_dict(dict);
  PASS();
}

SUITE(general_balanced_tree_c_suite) {
  RUN_TEST(general_balanced_tree_insert_lookup_size);
  RUN_TEST(general_balanced_tree_duplicate_insert);
  RUN_TEST(general_balanced_tree_delete);
  RUN_TEST(general_balanced_tree_clear);
  RUN_TEST(general_balanced_tree_perfect_balance);
  RUN_TEST(general_balanced_tree_large_insert_delete);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TEST_GENERAL_BALANCED_TREE_C_H */
