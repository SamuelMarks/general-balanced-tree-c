#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "general_balanced_tree_c.h"

void gbt_default_key_assign(gbt_ky_type *dst, const gbt_ky_type src) {
  *dst = src;
}

int gbt_default_key_less(const gbt_ky_type a, const gbt_ky_type b) {
  return a < b;
}

int gbt_default_key_equal(const gbt_ky_type a, const gbt_ky_type b) {
  return a == b;
}

void gbt_default_assign(gbt_data_type *dst, const gbt_data_type src) {
  *dst = src;
}

void gbt_default_key_print(const gbt_ky_type key) {
  printf("%d", key);
}

void leftrot(gbt_noderef *t) {
  gbt_node *tmp;

  tmp = *t;
  *t = (*t)->right;
  tmp->right = (*t)->left;
  (*t)->left = tmp;
}

void rightrot(gbt_noderef *t) {
  gbt_node *tmp;

  tmp = *t;
  *t = (*t)->left;
  tmp->left = (*t)->right;
  (*t)->right = tmp;
}

void Skew(gbt_noderef *t) {
  do {
    while ((*t)->left)
      rightrot(t);
    t = &(*t)->right;
  } while (*t);
}

void oldSplit(gbt_noderef *t, long p1, long p2) {
  float tmp;
  long d;

  tmp = (float)(p1 - p2) / (float)p2;
  for (d = 1; d <= p2; d++) {
    if ((long)((float)d * tmp) > (long)(((float)d - 1) * tmp))
      leftrot(t);
    t = &(*t)->right;
  }
}

void Split(gbt_noderef *t, const size_t p1, const size_t p2) {
  const size_t incr = p1 - p2;
  size_t count = 0;
  size_t i;

  for (i = p2; i > 0; i--) {
    count += incr;
    if (count >= p2) {
      leftrot(t);
      count -= p2; /* incr <= p2 */
    }
    t = &(*t)->right;
  }
}

void gbt_PerfectBalance(gbt_noderef *t, size_t w) {
  long b;

  Skew(t);
  b = 1;
  while (b <= w)
    b *= 2;
  b /= 2;
  if (b != w)
    Split(t, w - 1, b - 1);
  while (b > 2) {
    Split(t, b - 1, b / 2 - 1);
    b /= 2;
  }
}

long gbt_TreeWeight(gbt_noderef t) {
  gbt_node *stack[GBT_MAXHEIGHT];
  long top, w;

  w = 1;
  GBT_NULLSTACK;
  while (t) {
    while (t->left) {
      w++;
      if (t->right)
        GBT_PUSH(t->right);
      t = t->left;
    }
    w++;
    if (!t->right)
      GBT_POP(t)
    else
      t = t->right;
  }
  return w;
}

void gbt_FixBalance(gbt_dictptr D, gbt_ky_type key, long d1) {
  long d2;
  gbt_noderef *p[GBT_MAXHEIGHT + 1];
  long w;

  if (d1 <= 1)
    return;

  p[1] = &(D->t); /* a */
  for (d2 = 1; d2 < d1; d2++) {
    if (D->key_less(key, (*p[d2])->key))
      p[d2 + 1] = &(*p[d2])->left;
    else
      p[d2 + 1] = &(*p[d2])->right;
  }
  w = 2; /* b */
  do {
    d2--;
    if (d2 < 1)
      break;
    if (&(*p[d2])->left == p[d2 + 1])
      w = w + gbt_TreeWeight((*p[d2])->right);
    else
      w = w + gbt_TreeWeight((*p[d2])->left);
  } while (w >= gbt_minweight[d1 - d2 + 1]);
  if (d2 >= 1)
    gbt_PerfectBalance(p[d2], w); /* c */
}

void gbt_InitGlobal(void) {
  long h;

  for (h = 1; h <= GBT_MAXHEIGHT; h++)
    gbt_minweight[h] =
        (long)(exp((double)(h - 1) / GBT_C * log(2.0)) + 0.5) + 1;
}

gbt_dictptr construct_dict(void) {
  return construct_dict_full(
      gbt_default_key_assign,
      gbt_default_key_less,
      gbt_default_key_equal,
      gbt_default_assign,
      gbt_default_key_print);
}

gbt_dictptr construct_dict_full(gbt_ky_assign_func key_assign_func,
                                gbt_ky_less_func key_less_than_func,
                                gbt_ky_equal_func key_equal_func,
                                gbt_assign_func assign_func,
                                gbt_key_print_func key_print_func) {
  gbt_dictptr p;

  gbt_InitGlobal();
  p = (gbt_dictptr)malloc(sizeof(gbt_dict));
  if (!p)
    return NULL;
  p->t = NULL;
  p->weight = 1;
  p->numofdeletions = 0;

  /* Store function pointers */
  p->key_assign = key_assign_func == NULL ? gbt_default_key_assign : key_assign_func;
  p->key_less = key_less_than_func == NULL? gbt_default_key_less : key_less_than_func;
  p->key_equal = key_equal_func == NULL ? gbt_default_key_equal : key_equal_func;
  p->assign = assign_func == NULL ? gbt_default_assign: assign_func;
  p->key_print = key_print_func == NULL ? gbt_default_key_print: key_print_func;

  return p;
}


void gbt_CreateNode(gbt_dictptr D, gbt_ky_type x, gbt_data_type in,
                    gbt_noderef *t) {
  *t = (gbt_node *)malloc(sizeof(gbt_node));
  if (!*t)
    return;
  D->key_assign(&(*t)->key, x);
  D->assign(&(*t)->data, in);
  (*t)->left = NULL;
  (*t)->right = NULL;
}

void gbt_Display(gbt_dictptr D, gbt_noderef t, const long depth) {
  if (t == NULL || depth > 8)
    return;
  gbt_Display(D, t->left, depth + 1);

  /* Print key with indent, then use the print function pointer */
  {
    const unsigned spaces = GBT_SCREENWIDTH - depth * 4 - 4;
    unsigned i;
    for (i = 0; i < spaces; ++i) putchar(' ');
    D->key_print(t->key);
    putchar('\n');
  }

  gbt_Display(D, t->right, depth + 1);
}

gbt_noderef gbt_insert(gbt_dictptr D, gbt_ky_type key, gbt_data_type in) {
  long d1;
  gbt_noderef *candidate, *p, newnode;

  d1 = 1;
  p = &(D->t);
  candidate = NULL;
  while (*p) {
    if (D->key_less(key, (*p)->key)) {
      p = &(*p)->left;
    } else {
      if (D->key_equal(key, (*p)->key))
        candidate = p;
      p = &(*p)->right;
    }
    d1++;
  }
  if (candidate)
    return *candidate;
  gbt_CreateNode(D, key, in, p);
  newnode = *p;
  D->weight++;
  if (D->weight < (size_t)(gbt_minweight[d1]))
    gbt_FixBalance(D, key, d1);
  return newnode;
}

gbt_noderef gbt_lookup(gbt_dictptr D, const gbt_ky_type key) {
  gbt_noderef t = D->t;
  while (t) {
    if (D->key_equal(key, t->key))
      return t;
    else if (D->key_less(key, t->key))
      t = t->left;
    else
      t = t->right;
  }
  return NULL;
}

void gbt_delete(gbt_dictptr D, const gbt_ky_type key) {
  gbt_noderef *candidate, *last = NULL, tmp, *t;

  t = &(D->t);
  candidate = NULL;
  while (*t) {
    last = t;
    if (D->key_less(key, (*t)->key))
      t = &(*t)->left;
    else {
      candidate = t;
      t = &(*t)->right;
    }
  }
  if (candidate && /* a */
      (D->key_equal((*candidate)->key, key))) {
    D->numofdeletions++;
    D->weight--;
    tmp = *last;
    if (candidate == last) { /* b */
      *last = (*last)->left;
      free(tmp);
    } else { /* c */
      *last = (*last)->right;
      tmp->right = (*candidate)->right;
      tmp->left = (*candidate)->left;
      free(*candidate);
      *candidate = tmp;
    }
      }
  if (D->numofdeletions > GBT_MAXDEL * D->weight /* d */
      && D->weight > 3) {
    gbt_PerfectBalance(&(D->t), D->weight);
    D->numofdeletions = 0;
      }
}

gbt_ky_type gbt_keyval(gbt_dictptr D, gbt_noderef item) {
  return ((gbt_noderef)item)->key;
} /*gbt_keyval*/

gbt_data_type *gbt_infoval(gbt_dictptr D, gbt_noderef item) {
  return &((gbt_noderef)item)->data;
} /*gbt_infoval*/

size_t gbt_size(gbt_dictptr D) { return D->weight - 1; } /*gbt_size*/

void gbt_ClearTree(gbt_noderef *t) {
  if (!*t)
    return;
  gbt_ClearTree(&(*t)->right);
  gbt_ClearTree(&(*t)->left);
  free(*t);
  *t = NULL;
}

void gbt_clear(gbt_dictptr D) {
  gbt_ClearTree(&(D->t));
  D->weight = 1;
  D->numofdeletions = 0;
} /*clear*/

void gbt_destruct_dict(gbt_dictptr D) {
  gbt_clear(D);
  free(D);
} /*destruct_dict */
