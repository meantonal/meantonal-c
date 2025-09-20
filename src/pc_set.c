#include <stdbool.h>
#include <stdlib.h>

#include "../include/pc_set.h"

typedef struct tnode {
    int value;
    struct tnode *left;
    struct tnode *right;
} *PitchClassSet;

static struct tnode *create_tnode(int chroma) {
    struct tnode *node = malloc(sizeof(struct tnode));
    node->value = chroma;
    node->left = node->right = NULL;
    return node;
}

void pc_set_destroy(PitchClassSet set) {
    if (set->left)
        pc_set_destroy(set->left);
    if (set->right)
        pc_set_destroy(set->right);
    free(set);
}

void pc_set_insert(PitchClassSet *set, int chroma) {
    if (*set == NULL)
        *set = create_tnode(chroma);
    else if ((*set)->value > chroma)
        pc_set_insert(&(*set)->left, chroma);
    else if ((*set)->value < chroma)
        pc_set_insert(&(*set)->right, chroma);
}

bool pc_set_contains(PitchClassSet set, int chroma) {
    if (set == NULL)
        return false;
    return set->value == chroma || pc_set_contains(set->left, chroma) ||
           pc_set_contains(set->right, chroma);
}

PitchClassSet pc_set_transpose(PitchClassSet set, int offset) {
    if (set == NULL)
        return NULL;
    PitchClassSet new_node = create_tnode(set->value + offset);
    new_node->left = pc_set_transpose(set->left, offset);
    new_node->right = pc_set_transpose(set->right, offset);

    return new_node;
}

PitchClassSet pc_set_invert(PitchClassSet set, int axis) {
    if (set == NULL)
        return NULL;
    PitchClassSet new_node = create_tnode(axis - set->value);
    new_node->left = pc_set_invert(set->left, axis);
    new_node->right = pc_set_invert(set->right, axis);

    return new_node;
}

PitchClassSet pc_set_clone(PitchClassSet set) {
    if (set == NULL)
        return NULL;
    PitchClassSet copy = create_tnode(set->value);
    copy->left = pc_set_clone(set->left);
    copy->right = pc_set_clone(set->right);
    return copy;
}

void pc_set_insert_all(PitchClassSet *dest, PitchClassSet src) {
    if (src == NULL)
        return;
    pc_set_insert(dest, src->value);
    pc_set_insert_all(dest, src->left);
    pc_set_insert_all(dest, src->right);
}

PitchClassSet pc_set_union(PitchClassSet a, PitchClassSet b) {
    PitchClassSet result = pc_set_clone(a);
    pc_set_insert_all(&result, b);
    return result;
}

PitchClassSet pc_set_intersection(PitchClassSet a, PitchClassSet b) {
    if (a == NULL || b == NULL)
        return NULL;

    PitchClassSet result = NULL;
    if (pc_set_contains(b, a->value))
        pc_set_insert(&result, a->value);

    PitchClassSet left = pc_set_intersection(a->left, b);
    PitchClassSet right = pc_set_intersection(a->right, b);

    result = pc_set_union(result, left);
    result = pc_set_union(result, right);

    return result;
}

PitchClassSet pc_set_difference(PitchClassSet a, PitchClassSet b) {
    if (a == NULL)
        return NULL;

    PitchClassSet result = NULL;

    if (!pc_set_contains(b, a->value)) {
        pc_set_insert(&result, a->value);
    }

    // Recurse left and right
    PitchClassSet left = pc_set_difference(a->left, b);
    PitchClassSet right = pc_set_difference(a->right, b);

    // Merge results
    result = pc_set_union(result, left);
    result = pc_set_union(result, right);

    return result;
}
