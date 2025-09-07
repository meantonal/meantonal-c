#ifndef PC_SET_H
#define PC_SET_H

#include "pitch.h"
#include "types.h"

/**
 * Frees the memory previously allocated by a passed in PitchClassSet.
 */
void pc_set_destroy(PitchClassSet set);

/**
 * Adds a pitch class to a PitchClassSet by chroma.
 * @return
 * The enlarged set.
 */
void pc_set_insert(PitchClassSet *set, int chroma);

/**
 * Checks whether a given pitch class is in a PitchClassSet by its chroma.
 */
bool pc_set_contains(PitchClassSet set, int chroma);

/**
 * Creates a copy of a pitch class set transposed by the specified number of
 * fifths.
 */
PitchClassSet pc_set_transpose(PitchClassSet set, int offset);

/**
 * Creates a copy of a pitch class set inverted about a given axis. An easy way
 * to obtain the axis is by creating a MirrorAxis and then taking its chroma.
 */
PitchClassSet pc_set_invert(PitchClassSet set, int axis);

/**
 * Creates a new set that contains the elements of both passed in sets.
 */
PitchClassSet pc_set_union(PitchClassSet a, PitchClassSet b);

/**
 * Creates a new set that contains the common elements shared by the passed in
 * sets.
 */
PitchClassSet pc_set_intersection(PitchClassSet a, PitchClassSet b);

/**
 * Creates a new set that contains elements in the first set not shared by the
 * second.
 */
PitchClassSet pc_set_difference(PitchClassSet a, PitchClassSet b);

#endif
