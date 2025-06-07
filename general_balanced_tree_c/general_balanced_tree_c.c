#include "general_balanced_tree_c.h"
#include <math.h>
#include <stdlib.h>

void InitGlobal(void) {
  long h;

  for (h = 1; h <= maxheight; h++)
    minweight[h] = (long)(exp((h - 1) / C * log(2.0)) + 0.5) + 1;
}

dictptr construct_dict() {
  dictptr p;

  InitGlobal();
  p = (dictptr)malloc(sizeof(dict));
  p->t = NULL;
  p->weight = 1;
  p->numofdeletions = 0;
  return p;
}

void CreateNode(ky_type x, data_type in, noderef *t) {
  *t = (node *)malloc(sizeof(node));
  KY_ASSIGN((*t)->key, x);
  IN_ASSIGN((*t)->data, in);
  (*t)->left = NULL;
  (*t)->right = NULL;
}

void Display(noderef t, long depth) {
  if ((t == NULL) || (depth > 8))
    return;
  Display(t->left, depth + 1);
  printf("%*d\n", (int)(screenwidth - depth * 4 - 4), t->key);
  Display(t->right, depth + 1);
}

noderef insert(dictptr D, ky_type key, data_type in) {
  long d1;
  noderef *candidate, *p, newnode;

  d1 = 1;
  p = &(D->t);
  candidate = NULL;
  while (*p) { /* a */
    if (KY_LESS(key, (*p)->key))
      p = &(*p)->left;
    else {
      candidate = p;
      p = &(*p)->right;
    }
    d1++;
  }
  if (candidate && /* b */
      (KY_EQUAL((*candidate)->key, key)))
    return *candidate;
  CreateNode(key, in, p); /* c */
  newnode = *p;
  D->weight++;
  if (D->weight < minweight[d1]) /* d */
    FixBalance(D, key, d1);
  return newnode;
}

noderef lookup(dictptr D, ky_type key) {
  register noderef t, candidate;

  t = D->t;
  candidate = NULL; /* a */
  while (t) {
    if (KY_LESS(key, t->key))
      t = t->left;
    else {
      candidate = t; /* b */
      t = t->right;
    }
  }
  return candidate; /* c */
}

void delete(dictptr D, ky_type key) {
  noderef *candidate, *last, tmp, *t;

  t = &(D->t);
  candidate = NULL;
  while (*t) {
    last = t;
    if (KY_LESS(key, (*t)->key))
      t = &(*t)->left;
    else {
      candidate = t;
      t = &(*t)->right;
    }
  }
  if (candidate && /* a */
      (KY_EQUAL((*candidate)->key, key))) {
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
  if (D->numofdeletions > maxdel * D->weight /* d */
      && D->weight > 3) {
    PerfectBalance(&(D->t), D->weight);
    D->numofdeletions = 0;
  }
}

ky_type keyval(dictptr D, noderef item) {
  return ((noderef)item)->key;
} /*keyval*/

data_type *infoval(dictptr D, noderef item) {
  return &((noderef)item)->data;
} /*infoval*/

int size(dictptr D) { return D->weight - 1; } /*size*/

void ClearTree(noderef *t) {
  if (!*t)
    return;
  ClearTree(&(*t)->right);
  ClearTree(&(*t)->left);
  free(*t);
  *t = NULL;
}

void clear(dictptr D) {
  ClearTree(&(D->t));
  D->weight = 1;
  D->numofdeletions = 0;
} /*clear*/

void destruct_dict(dictptr D) {
  clear(D);
  free(D);
} /*destruct_dict */
