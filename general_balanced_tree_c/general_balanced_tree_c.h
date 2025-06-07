#ifndef GENERAL_BALANCED_TREE_C_H
#define GENERAL_BALANCED_TREE_C_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <general_balanced_tree_c_export.h>
#include <stdio.h>

#define C 1.35    /* Other values could be used.      */
                  /* as long as C > 1.                 */
#define maxdel 10 /* The number of deletions       */
                  /* since last global rebalancing */
                  /* is at most than 10 times the  */
                  /* tree weight.                  */
                  /* (Other constant possible.)    */

#define maxheight 40 /* We assume C * log n < 40.  */
                     /* Keep an eye on this one!   */

#define screenwidth 40 /* For displaying tree.     */

/*---------- User defined data types --------------*/

typedef long data_type;         /* User defined */
typedef int ky_type;            /* User defined */
#define KY_ASSIGN(a, b) a = b   /* User defined */
#define KY_LESS(a, b) (a < b)   /* User defined */
#define KY_EQUAL(a, b) (a == b) /* User defined */
#define IN_ASSIGN(a, b) a = b   /* User defined */

/*----- Procedures for external use -----------------

The following procedures are for exernal use:

dictptr construct_dict()
   Generates a new dictionary.

noderef insert (dictptr D, ky_type key,
                data_type in)
   Insert key and data, returns a reference.

noderef lookup (dictptr D, ky_type key)
   Returns a reference.

void delete (dictptr D, ky_type key)
   Delete key (and data)

ky_type keyval (dictptr D, noderef item)
   Get key via reference.

data_type *infoval (dictptr D, noderef item)
   Get a pointer to data.

int size (dictptr D)
   Number of stored items (= tree weight - 1)

void clear (dictptr D)
   Remove everything from dictionary.

void destruct_dict (dictptr D)
  Destruct the dictionary.

Furthermore, the data type noderef is used by
the external user as reference to stored items.
(It is also a reference to tree nodes.)

---------------------------------------------------*/

typedef struct node {
  ky_type key;
  data_type data;
  struct node *left, *right;
} node;
typedef node *noderef;
typedef struct dict {
  noderef t;
  long weight, numofdeletions;
} dict;
typedef dict *dictptr;

static long minweight[maxheight + 1];

/*---------------------------*/
/* The tree is shown on the  */
/* screen in a simple way.   */
/* (We only display the      */
/*  higest levels.)          */
/*---------------------------*/

extern GENERAL_BALANCED_TREE_C_EXPORT void Display(noderef t, long depth);

/*-------------- construction -------------------*/

extern GENERAL_BALANCED_TREE_C_EXPORT void InitGlobal(void);

extern dictptr construct_dict(void);

/*---------------- Rebalancing ------------------*/

extern GENERAL_BALANCED_TREE_C_EXPORT void PerfectBalance(noderef *t, long w);

#define NULLSTACK                                                              \
  {                                                                            \
    top = 0;                                                                   \
    stack[0] = NULL;                                                           \
  }
#define PUSH(t)                                                                \
  {                                                                            \
    top++;                                                                     \
    stack[top] = t;                                                            \
  }
#define POP(t)                                                                 \
  {                                                                            \
    t = stack[top];                                                            \
    top--;                                                                     \
  }
extern long TreeWeight(noderef t);

extern void FixBalance(dictptr D, ky_type key, long d1);

extern void CreateNode(ky_type x, data_type in,
                                                      noderef *t);

extern GENERAL_BALANCED_TREE_C_EXPORT noderef insert(dictptr D, ky_type key,
                                                     data_type in);

extern GENERAL_BALANCED_TREE_C_EXPORT noderef lookup(dictptr D, ky_type key);

extern GENERAL_BALANCED_TREE_C_EXPORT void delete(dictptr D, ky_type key);

extern ky_type keyval(dictptr D, noderef item);

extern data_type *infoval(dictptr D,
                                                         noderef item);

extern GENERAL_BALANCED_TREE_C_EXPORT int size(dictptr D);

extern void ClearTree(noderef *t);

extern GENERAL_BALANCED_TREE_C_EXPORT void clear(dictptr D);

extern void destruct_dict(dictptr D);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !GENERAL_BALANCED_TREE_C_H */
