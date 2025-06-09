#ifndef GENERAL_BALANCED_TREE_C_H
#define GENERAL_BALANCED_TREE_C_H

#ifdef __cplusplus
extern "GBT_C" {
#endif /* __cplusplus */

#include <general_balanced_tree_c_export.h>

#ifndef GBT_C
#define GBT_C 1.35 /* Other values could be used.         */
                   /* as long as GBT_C > 1.               */
#endif             /* !GBT_C */
#ifndef GBT_MAXDEL
#define GBT_MAXDEL 10 /* The number of deletions          */
                      /* since last global rebalancing    */
                      /* is at most than 10 times the     */
                      /* tree weight.                     */
                      /* (Other constant possible.)       */
#endif                /* !GBT_MAXDEL */
#ifndef GBT_MAXHEIGHT
#define GBT_MAXHEIGHT 40 /* We assume GBT_C * log n < 40. */
                         /* Keep an eye on this one!      */
#endif                   /* !GBT_MAXHEIGHT */
#ifndef GBT_SCREENWIDTH
#define GBT_SCREENWIDTH 40 /* For displaying tree.        */
#endif                     /* !GBT_SCREENWIDTH            */

/*---------- User defined data types --------------*/

#ifndef GBT_DATA_TYPE
  typedef long gbt_data_type; /* User defined */
#endif                      /* !GBT_DATA_TYPE */
#ifndef GBT_KEY_TYPE
#define GBT_KEY_TYPE
typedef int gbt_ky_type; /* User defined */
#define KEY_TYPE_FMT "d"
#endif /* !GBT_KEY_TYPE */

/* Type definitions for operations as function pointers */
typedef void (*gbt_ky_assign_func)(gbt_ky_type *dst, gbt_ky_type src);
typedef int (*gbt_ky_less_func)(gbt_ky_type a, gbt_ky_type b);
typedef int (*gbt_ky_equal_func)(gbt_ky_type a, gbt_ky_type b);
typedef void (*gbt_assign_func)(gbt_data_type *dst, gbt_data_type src);
typedef void (*gbt_key_print_func)(gbt_ky_type key);

/*----- Procedures for external use -----------------

The following procedures are for exernal use:

gbt_dictptr gbt_construct_dict()
   Generates a new dictionary.

gbt_noderef gbt_insert (gbt_dictptr D, gbt_ky_type key,
                data_type in)
   Insert key and data, returns a reference.

gbt_noderef gbt_lookup (gbt_dictptr D, gbt_ky_type key)
   Returns a reference.

void gbt_delete (gbt_dictptr D, gbt_ky_type key)
   Delete key (and data)

ky_type gbt_keyval (gbt_dictptr D, gbt_noderef item)
   Get key via reference.

data_type *gbt_infoval (gbt_dictptr D, gbt_noderef item)
   Get a pointer to data.

size_t gbt_size (gbt_dictptr D)
   Number of stored items (= tree weight - 1)

void clear (gbt_dictptr D)
   Remove everything from dictionary.

void destruct_dict (gbt_dictptr D)
  Destruct the dictionary.

Furthermore, the data type gbt_noderef is used by
the external user as reference to stored items.
(It is also a reference to tree nodes.)

---------------------------------------------------*/

typedef struct gbt_node {
  gbt_ky_type key;
  gbt_data_type data;
  struct gbt_node *left, *right;
} gbt_node;
typedef gbt_node *gbt_noderef;
typedef struct gbt_dict {
  gbt_noderef t;
  size_t weight, numofdeletions;

  gbt_ky_assign_func key_assign;
  gbt_ky_less_func key_less;
  gbt_ky_equal_func key_equal;
  gbt_assign_func assign;
  gbt_key_print_func key_print;
} gbt_dict;
typedef gbt_dict *gbt_dictptr;

static long gbt_minweight[GBT_MAXHEIGHT + 1];

/*---------------------------*/
/* The tree is shown on the  */
/* screen in a simple way.   */
/* (We only display the      */
/*  higest levels.)          */
/*---------------------------*/

extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_Display(gbt_dictptr, gbt_noderef, long);

/*-------------- default ------------------------*/

  extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_default_key_assign(gbt_ky_type *, gbt_ky_type );

  extern GENERAL_BALANCED_TREE_C_EXPORT int gbt_default_key_less(gbt_ky_type , gbt_ky_type );

  extern GENERAL_BALANCED_TREE_C_EXPORT int gbt_default_key_equal(gbt_ky_type , gbt_ky_type );

  extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_default_assign(gbt_data_type *, gbt_data_type );

  extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_default_key_print(gbt_ky_type );

/*-------------- construction -------------------*/

extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_InitGlobal(void);

extern GENERAL_BALANCED_TREE_C_EXPORT gbt_dictptr construct_dict_full(
      gbt_ky_assign_func,
      gbt_ky_less_func,
      gbt_ky_equal_func,
      gbt_assign_func,
      gbt_key_print_func);

extern gbt_dictptr construct_dict(void);

/*---------------- Rebalancing ------------------*/

extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_PerfectBalance(gbt_noderef *t,
                                                              size_t w);

#define GBT_NULLSTACK                                                          \
  {                                                                            \
    top = 0;                                                                   \
    stack[0] = NULL;                                                           \
  }
#define GBT_PUSH(t)                                                            \
  {                                                                            \
    top++;                                                                     \
    stack[top] = t;                                                            \
  }
#define GBT_POP(t)                                                             \
  {                                                                            \
    t = stack[top];                                                            \
    top--;                                                                     \
  }
extern long gbt_TreeWeight(gbt_noderef t);

extern void gbt_FixBalance(gbt_dictptr D, gbt_ky_type key, long d1);

  extern void gbt_CreateNode(gbt_dictptr D, gbt_ky_type x, gbt_data_type in,
                      gbt_noderef *t);

extern GENERAL_BALANCED_TREE_C_EXPORT gbt_noderef gbt_insert(gbt_dictptr D, gbt_ky_type key, gbt_data_type in);

extern GENERAL_BALANCED_TREE_C_EXPORT gbt_noderef gbt_lookup(gbt_dictptr D,
                                                             gbt_ky_type key);

extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_delete(gbt_dictptr D, gbt_ky_type key);

extern gbt_ky_type gbt_keyval(gbt_dictptr D, gbt_noderef item);

extern gbt_data_type *gbt_infoval(gbt_dictptr D, gbt_noderef item);

extern GENERAL_BALANCED_TREE_C_EXPORT size_t gbt_size(gbt_dictptr D);

extern void gbt_ClearTree(gbt_noderef *t);

extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_clear(gbt_dictptr D);

extern void gbt_destruct_dict(gbt_dictptr D);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GENERAL_BALANCED_TREE_C_H */
