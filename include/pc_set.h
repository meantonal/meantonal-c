#ifndef PC_SET_H
#define PC_SET_H

#include "pitch.h"
#include "types.h"

typedef struct {
    struct tnode *first;
} PitchClassSet;

/**
 * Create an empty pitch class set.
 * Note: you are responsible for freeing the resources allocated to create this
 * set.
 */
PitchClassSet *pc_set_create(void);

void pc_set_destroy(PitchClassSet *set);

void pc_set_insert(PitchClassSet *set, int chroma);

bool pc_set_contains(PitchClassSet *set, int chroma);

#endif
