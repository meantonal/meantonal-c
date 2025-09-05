#include "../include/pc_set.h"
#include <stdlib.h>

struct tnode {
    int value;
    struct tnode *left;
    struct tnode *right;
};

PitchClassSet *pc_set_create(void) {
    PitchClassSet *s = malloc(sizeof(PitchClassSet));
    s->first = NULL;
    return s;
}

static void tnode_destroy(struct tnode *node) {
    if (!node)
        return;
    tnode_destroy(node->right);
    tnode_destroy(node->left);
    free(node);
}

void pc_set_destroy(PitchClassSet *set) {
    tnode_destroy(set->first);
    free(set);
}

struct tnode *create_tnode(int chroma) {
    struct tnode *node = malloc(sizeof(struct tnode));
    node->value = chroma;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void insert_tnode(struct tnode *node, int chroma) {
    if (node->value > chroma) {
        if (node->left) {
            insert_tnode(node->left, chroma);
        } else {
            node->left = create_tnode(chroma);
        }
    } else if (node->value < chroma) {
        if (node->right) {
            insert_tnode(node->right, chroma);
        } else {
            node->right = create_tnode(chroma);
        }
    }
    return; // current node already holds chroma
}

void pc_set_insert(PitchClassSet *set, int chroma) {
    if (set->first == NULL) {
        set->first = create_tnode(chroma);
    } else
        insert_tnode(set->first, chroma);
}

static bool tnode_contains(struct tnode *node, int chroma) {
    if (!node)
        return false;
    if (node->value == chroma)
        return true;
    return (tnode_contains(node->left, chroma) ||
            tnode_contains(node->right, chroma));
}

bool pc_set_contains(PitchClassSet *set, int chroma) {
    if (set->first == NULL)
        return false;
    return tnode_contains(set->first, chroma);
}
