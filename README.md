general_balanced_tree_c
=======================
[![License](https://img.shields.io/badge/license-Apache--2.0%20OR%20MIT%20OR%20CC0-blue.svg)](https://opensource.org/licenses/Apache-2.0) [![.github/workflows/linux-Windows-macOS.yml](https://github.com/SamuelMarks/general-balanced-tree-c/actions/workflows/linux-Windows-macOS.yml/badge.svg)](https://github.com/SamuelMarks/general-balanced-tree-c/actions/workflows/linux-Windows-macOS.yml)

General Balanced trees (from Arne Andersson).
https://user.it.uu.se/~arneande/ps/gb.pdf

> We show that, in order to achieve efficient maintenance of a balanced binary search tree, no shape restriction other
> than a logarithmic height is required. The obtained class of trees, general balanced trees, may be maintained at a
> logarithmic amortized cost with no balance information stored in the nodes. Thus, in the case when amortized bounds
> are sufficient, there is no need for sophisticated balance criteria.
>
> The maintenance algorithms use partial rebuilding. This is important for certain applications, and has previously been
> used with weight-balanced trees. We show that the amortized cost incurred by general balanced trees is lower than what
> has been shown for weight-balanced trees.

This codebase makes minor changes to https://user.it.uu.se/~arneande/abs/gbimpl.html in order to support:

- C23 (it broke many of the old K&R pre-C89 concepts)
- CMake build system
- Creating a library:
    - Guard macros
    - Carefully choose which symbols to export to shared library
    - Rename all symbols to be prefixed with `GBT` or `gbt`
- Separating test CLI into a separate `main.c` in its own `target`
- Minor bug fixes (out-of-bounds checks; `size_t` for size types; &etc.)
- Test suite (integrated into CTest)

(whilst retaining cross-platform support in C89)

## Build

```sh
$ cmake -DCMAKE_BUILD_TYPE='Debug' -S . -B 'build'
$ cmake --build 'build'
```

## Usage

### Configuration

```c
/* Modify constants, types, and comparators                    */
#define GBT_C          1.35 /* Other values could be used.     */
                            /* as long as GBT_C > 1.           */
#define GBT_MAXDEL     10   /* The number of deletions         */
                            /* since last global rebalancing   */
                            /* is at most than 10 times the    */
                            /* tree weight.                    */
                            /* (Other constant possible.)      */
#define GBT_MAXHEIGHT   40  /* We assume GBT_C * log n < 40.   */
                            /* Keep an eye on this one!        */
#define GBT_SCREENWIDTH 40  /* For displaying tree.            */
```

These are all optional, and ↑ are the defaults.

### Basics

```c
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifndef GBT_KEY_TYPE
#define GBT_KEY_TYPE
typedef const char* gbt_ky_type;
#endif /* !GBT_KEY_TYPE */
#include <general_balanced_tree_c.h>

void my_key_print(const gbt_ky_type key) {
  printf("%s", key);
}

void my_key_assign(gbt_ky_type *dst, const gbt_ky_type src) {
  *dst = strdup(src);
}

int main(void) {
    /* const gbt_dictptr dict = construct_dict();
       // this default dict is int->int ^
    */

    const gbt_dictptr dict = return construct_dict_full(
      my_key_assign,
      /*fallsback to default: gbt_default_key_less*/ NULL,
      /*fallsback to default: gbt_default_key_equal*/ NULL,
      /*fallsback to default: gbt_default_assign*/ NULL,
      my_key_print);
    gbt_insert(dict, "foo", "bar"); /* insert key `"foo"` with value `"bar"` */
    {
      const gbt_noderef needle = gbt_lookup(dict, "foo");  /* find value of key "foo" */
      assert(needle != NULL);
      assert(strcmp(gbt_keyval(dict, needle), "bar") == 0);
    }
    gbt_delete(dict, "foo");     /* delete key `"foo"` */

    return EXIT_SUCCESS;
}
```

See [`test_general_balanced_tree_c.h`](general_balanced_tree_c/tests/test_general_balanced_tree_c.h) for more examples.

See `extern GENERAL_BALANCED_TREE_C_EXPORT` prefixed symbols in [
`general_balanced_tree_c.h`](general_balanced_tree_c/general_balanced_tree_c.h) for whole public API.

---

## License

NOTE: 🤷 paper was from 26 years ago [code most likely from 1989]—before software licenses were popular—not sure what to
do about this… the author also hasn't published in decades… so I'll just license super permissively below and hopefully
that's ok ツ

Licensed under any of:

- Apache License, Version 2.0 ([LICENSE-APACHE](LICENSE-APACHE) or <https://www.apache.org/licenses/LICENSE-2.0>)
- MIT license ([LICENSE-MIT](LICENSE-MIT) or <https://opensource.org/licenses/MIT>)
- CC0 license ([LICENSE-CC0](LICENSE-CC0) or <https://creativecommons.org/public-domain/cc0>)

at your option.

### Contribution

Unless you explicitly state otherwise, any contribution intentionally submitted
for inclusion in the work by you, as defined in the Apache-2.0 license, shall be
licensed as above, without any additional terms or conditions.
