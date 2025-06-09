#ifndef GENERAL_BALANCED_TREE_C_H
#define GENERAL_BALANCED_TREE_C_H

#ifdef __cplusplus
extern "C" {
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
typedef void (*gbt_ky_assign_func)(gbt_ky_type *, gbt_ky_type);
typedef int (*gbt_ky_less_func)(gbt_ky_type, gbt_ky_type);
typedef int (*gbt_ky_equal_func)(gbt_ky_type, gbt_ky_type);
typedef void (*gbt_assign_func)(gbt_data_type *, gbt_data_type);
typedef void (*gbt_key_destroy_func)(gbt_ky_type);
typedef void (*gbt_key_print_func)(gbt_ky_type);

/*----- Procedures for external use -----------------

The following procedures are for external use:

struct gbt_dict * gbt_construct_dict()
   Generates a new dictionary.

struct gbt_node * (struct gbt_dict * D, gbt_ky_type key,
                data_type in)
   Insert key and data, returns a reference.

struct gbt_node * gbt_lookup (struct gbt_dict * D, gbt_ky_type key)
   Returns a reference.

void gbt_delete (struct gbt_dict * D, gbt_ky_type key)
   Delete key (and data)

ky_type gbt_keyval (struct gbt_dict * D, struct gbt_node * item)
   Get key via reference.

data_type *gbt_infoval (struct gbt_dict * D, struct gbt_node * item)
   Get a pointer to data.

size_t gbt_size (struct gbt_dict * D)
   Number of stored items (= tree weight - 1)

void clear (struct gbt_dict * D)
   Remove everything from dictionary.

void destruct_dict (struct gbt_dict * D)
  Destruct the dictionary.

Furthermore, the data type struct gbt_node * is used by
the external user as reference to stored items.
(It is also a reference to tree nodes.)

---------------------------------------------------*/

struct gbt_node {
  gbt_ky_type key;
  gbt_data_type data;
  struct gbt_node *left, *right;
};
struct gbt_dict {
  struct gbt_node *t;
  size_t weight, numofdeletions;

  gbt_ky_assign_func key_assign;
  gbt_ky_less_func key_less;
  gbt_ky_equal_func key_equal;
  gbt_assign_func assign;
  gbt_key_destroy_func key_destroy;
  gbt_key_print_func key_print;
};

static long gbt_minweight[GBT_MAXHEIGHT + 1];

/*---------------------------*/
/* The tree is shown on the  */
/* screen in a simple way.   */
/* (We only display the      */
/*  higest levels.)          */
/*---------------------------*/

extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_Display(struct gbt_dict *,
                                                       struct gbt_node *, long);

/*-------------- default ------------------------*/

extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_default_key_assign(gbt_ky_type *,
                                                                  gbt_ky_type);

extern GENERAL_BALANCED_TREE_C_EXPORT int gbt_default_key_less(gbt_ky_type,
                                                               gbt_ky_type);

extern GENERAL_BALANCED_TREE_C_EXPORT int gbt_default_key_equal(gbt_ky_type,
                                                                gbt_ky_type);

extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_default_assign(gbt_data_type *,
                                                              gbt_data_type);

extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_default_key_destroy(gbt_ky_type);

extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_default_key_print(gbt_ky_type);

/*-------------- construction -------------------*/

extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_InitGlobal(void);

extern GENERAL_BALANCED_TREE_C_EXPORT struct gbt_dict *
    gbt_construct_dict_full(gbt_ky_assign_func, gbt_ky_less_func,
                            gbt_ky_equal_func, gbt_assign_func,
                            gbt_key_destroy_func, gbt_key_print_func);

extern struct gbt_dict *gbt_construct_dict(void);

/*---------------- Rebalancing ------------------*/

extern GENERAL_BALANCED_TREE_C_EXPORT void
gbt_PerfectBalance(struct gbt_node **, size_t);

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
extern long gbt_TreeWeight(struct gbt_node *);

extern void gbt_FixBalance(struct gbt_dict *, gbt_ky_type, long);

extern void gbt_CreateNode(struct gbt_dict *, gbt_ky_type, gbt_data_type,
                           struct gbt_node **);

extern GENERAL_BALANCED_TREE_C_EXPORT struct gbt_node *
gbt_insert(struct gbt_dict *, gbt_ky_type, gbt_data_type);

extern GENERAL_BALANCED_TREE_C_EXPORT struct gbt_node *
gbt_lookup(struct gbt_dict *, gbt_ky_type);

extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_delete(struct gbt_dict *,
                                                      gbt_ky_type);

extern gbt_ky_type gbt_keyval(struct gbt_dict *, struct gbt_node *);

extern gbt_data_type *gbt_infoval(struct gbt_dict *, struct gbt_node *);

extern GENERAL_BALANCED_TREE_C_EXPORT size_t gbt_size(struct gbt_dict *);

extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_ClearTree(struct gbt_dict *,
                                                         struct gbt_node **);

extern GENERAL_BALANCED_TREE_C_EXPORT void gbt_clear(struct gbt_dict *);

extern void gbt_destruct_dict(struct gbt_dict *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GENERAL_BALANCED_TREE_C_H */
