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

void gbt_default_assign(gbt_data_type *const dst, const gbt_data_type src) {
  *dst = src;
}

void gbt_default_key_destroy(const gbt_ky_type _) {}

void gbt_default_key_print(const gbt_ky_type key) { printf("%d", key); }

void leftrot(struct gbt_node **const t) {
  struct gbt_node *tmp;

  tmp = *t;
  *t = (*t)->right;
  tmp->right = (*t)->left;
  (*t)->left = tmp;
}

void rightrot(struct gbt_node **const t) {
  struct gbt_node *tmp;

  tmp = *t;
  *t = (*t)->left;
  tmp->left = (*t)->right;
  (*t)->right = tmp;
}

void Skew(struct gbt_node **t) {
  do {
    while ((*t)->left)
      rightrot(t);
    t = &(*t)->right;
  } while (*t);
}

void oldSplit(struct gbt_node **t, const long p1, const long p2) {
  float tmp;
  long d;

  tmp = (float)(p1 - p2) / (float)p2;
  for (d = 1; d <= p2; d++) {
    if ((long)((float)d * tmp) > (long)(((float)d - 1) * tmp))
      leftrot(t);
    t = &(*t)->right;
  }
}

void Split(struct gbt_node **t, const size_t p1, const size_t p2) {
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

void gbt_PerfectBalance(struct gbt_node **const t, const size_t w) {
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

long gbt_TreeWeight(struct gbt_node *t) {
  struct gbt_node *stack[GBT_MAXHEIGHT];
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

void gbt_FixBalance(struct gbt_dict *const D, const gbt_ky_type key,
                    const long d1) {
  long d2;
  struct gbt_node **p[GBT_MAXHEIGHT + 1];
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

struct gbt_dict *gbt_construct_dict(void) {
  return gbt_construct_dict_full(
      gbt_default_key_assign, gbt_default_key_less, gbt_default_key_equal,
      gbt_default_assign, gbt_default_key_destroy, gbt_default_key_print);
}

struct gbt_dict *
gbt_construct_dict_full(const gbt_ky_assign_func key_assign_func,
                        const gbt_ky_less_func key_less_than_func,
                        const gbt_ky_equal_func key_equal_func,
                        const gbt_assign_func assign_func,
                        const gbt_key_destroy_func key_destroy_func,
                        const gbt_key_print_func key_print_func) {
  struct gbt_dict *p;

  gbt_InitGlobal();
  p = calloc(1, sizeof(*p));
  if (!p)
    return NULL;
  p->t = NULL;
  p->weight = 1;
  p->numofdeletions = 0;

  /* Store function pointers */
  p->key_assign =
      key_assign_func == NULL ? gbt_default_key_assign : key_assign_func;
  p->key_less =
      key_less_than_func == NULL ? gbt_default_key_less : key_less_than_func;
  p->key_equal =
      key_equal_func == NULL ? gbt_default_key_equal : key_equal_func;
  p->assign = assign_func == NULL ? gbt_default_assign : assign_func;
  p->key_destroy =
      key_destroy_func == NULL ? gbt_default_key_destroy : key_destroy_func;
  p->key_print =
      key_print_func == NULL ? gbt_default_key_print : key_print_func;

  return p;
}

void gbt_CreateNode(struct gbt_dict *const D, const gbt_ky_type key,
                    const gbt_data_type val, struct gbt_node **const t) {
  *t = calloc(1, sizeof(**t));
  if (!*t)
    return;

  D->key_assign(&(*t)->key, key);
  D->assign(&(*t)->data, val);
}

void gbt_Display(struct gbt_dict *D, struct gbt_node *t, const long depth) {
  if (t == NULL || depth > 8)
    return;
  gbt_Display(D, t->left, depth + 1);

  /* Print key with indent, then use the print function pointer */
  {
    const unsigned spaces = GBT_SCREENWIDTH - depth * 4 - 4;
    unsigned i;
    for (i = 0; i < spaces; ++i)
      putchar(' ');
    D->key_print(t->key);
    putchar('\n');
  }

  gbt_Display(D, t->right, depth + 1);
}

struct gbt_node *gbt_insert(struct gbt_dict *D, gbt_ky_type key,
                            gbt_data_type in) {
  long d1;
  struct gbt_node **candidate, **p, *newnode;

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

struct gbt_node *gbt_lookup(struct gbt_dict *D, const gbt_ky_type key) {
  struct gbt_node *t = D->t;
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

void gbt_delete(struct gbt_dict *D, const gbt_ky_type key) {
  struct gbt_node **candidate, **last = NULL, *tmp, **t;

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
  if (candidate && (D->key_equal((*candidate)->key, key))) {
    D->numofdeletions++;
    D->weight--;
    tmp = *last;
    if (candidate == last) {
      *last = (*last)->left;
      if (D->key_destroy)
        D->key_destroy(tmp->key);
      free(tmp);
    } else {
      *last = (*last)->right;
      tmp->right = (*candidate)->right;
      tmp->left = (*candidate)->left;

      if (D->key_destroy)
        D->key_destroy((*candidate)->key);
      free(*candidate);
      *candidate = tmp;
    }
  }
  if (D->numofdeletions > GBT_MAXDEL * D->weight && D->weight > 3) {
    gbt_PerfectBalance(&(D->t), D->weight);
    D->numofdeletions = 0;
  }
}

gbt_ky_type gbt_keyval(struct gbt_dict *const _, struct gbt_node *const item) {
  return item->key;
} /*gbt_keyval*/

gbt_data_type *gbt_infoval(struct gbt_dict *const _,
                           struct gbt_node *const item) {
  return &(item)->data;
} /*gbt_infoval*/

size_t gbt_size(struct gbt_dict *const D) { return D->weight - 1; } /*gbt_size*/

void gbt_ClearTree(struct gbt_dict *const D, struct gbt_node **const t) {
  if (!*t)
    return;
  gbt_ClearTree(D, &(*t)->right);
  gbt_ClearTree(D, &(*t)->left);

  if (D->key_destroy)
    D->key_destroy((*t)->key); /* Destroy key resource */

  free(*t);
  *t = NULL;
}

void gbt_clear(struct gbt_dict *const D) {
  gbt_ClearTree(D, &(D->t));
  D->weight = 1;
  D->numofdeletions = 0;
} /*clear*/

void gbt_destruct_dict(struct gbt_dict *D) {
  gbt_clear(D);
  free(D);
} /*destruct_dict */
