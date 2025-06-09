#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GBT_KEY_TYPE
typedef const char *gbt_ky_type;

#define GBT_DATA_TYPE
typedef const char *gbt_data_type;

#include <general_balanced_tree_c.h>

void str_key_print(const gbt_ky_type key) { puts(key); }

void str_assign(gbt_ky_type *dst, const gbt_ky_type src) {
  if (*dst)
    free((void *)*dst);
  *dst = strdup(src);
}

int str_key_less(const gbt_ky_type a, const gbt_ky_type b) {
  return strcmp(a, b) < 0;
}

int str_key_equal(const gbt_ky_type a, const gbt_ky_type b) {
  return strcmp(a, b) == 0;
}

void str_key_destroy(gbt_ky_type key) { free((void *)key); }

int main(void) {
  /* const struct gbt_dict * dict = gbt_construct_dict();
     // this default dict is int->int ^
  */

  struct gbt_dict *const dict = gbt_construct_dict_full(
      /*gbt_ky_assign_func*/ str_assign,
      /*gbt_ky_less_func*/ str_key_less,
      /*gbt_ky_equal_func*/ str_key_equal,
      /*gbt_assign_func*/ str_assign,
      /*gbt_key_destroy_func*/ str_key_destroy,
      /*gbt_key_print_func*/ str_key_print);

  gbt_insert(dict, "foo", "bar"); /* insert key `"foo"` with value `"bar"` */
  {
    struct gbt_node *const needle =
        gbt_lookup(dict, "foo"); /* find value of key "foo" */
    assert(needle != NULL);
    assert(strcmp(gbt_keyval(dict, needle), "bar") == 0);
  }
  gbt_delete(dict, "foo"); /* delete key `"foo"` */

  return EXIT_SUCCESS;
}
