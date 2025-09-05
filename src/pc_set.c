#include <stdbool.h>
#include <stdlib.h>

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

PitchClassSet pc_set_insert(PitchClassSet set, int chroma) {
    if (set == NULL)
        set = create_tnode(chroma);
    else if (set->value > chroma)
        set->left = pc_set_insert(set->left, chroma);
    else if (set->value < chroma)
        set->right = pc_set_insert(set->right, chroma);

    return set;
}

bool pc_set_contains(PitchClassSet set, int chroma) {
    if (set == NULL)
        return false;
    return set->value == chroma || pc_set_contains(set->left, chroma) ||
           pc_set_contains(set->right, chroma);
}

void pc_set_destroy(PitchClassSet set) {
    if (set->left)
        pc_set_destroy(set->left);
    if (set->right)
        pc_set_destroy(set->right);
    free(set);
}
