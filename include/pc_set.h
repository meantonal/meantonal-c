#ifndef PC_SET_H
#define PC_SET_H

#include "pitch.h"
#include "types.h"

/**
 * Adds a pitch class to a PitchClassSet by chroma.
 * @return
 * The enlarged set.
 */
PitchClassSet pc_set_insert(PitchClassSet set, int chroma);

/**
 * Checks whether a given pitch class is in a PitchClassSet by its chroma.
 */
bool pc_set_contains(PitchClassSet set, int chroma);

/**
 * Frees the memory previously allocated by a passed in PitchClassSet.
 */
void pc_set_destroy(PitchClassSet set);

#endif
