#include "../include/parse.h"
#include "../include/tonality.h"
#include "test_framework.h"

void test_context_from_str(void) {
    TonalContext k;
    context_from_str("F#", DORIAN, &k);
    ASSERT_EQ(k.tonic.letter, 5);
    ASSERT_EQ(k.tonic.accidental, 1);
    ASSERT_EQ(k.chroma_offset, -3);
    ASSERT_EQ(k.mode, DORIAN);
    context_from_str("Eb", MAJOR, &k);
    ASSERT_EQ(k.chroma_offset, 4);
    ASSERT_EQ(k.mode, MAJOR);
}

void test_context_from_chroma(void) {
    TonalContext k = context_from_chroma(6, DORIAN);
    ASSERT_EQ(k.tonic.letter, 5);
    ASSERT_EQ(k.tonic.accidental, 1);
    ASSERT_EQ(k.chroma_offset, -3);
    ASSERT_EQ(k.mode, DORIAN);
    k = context_from_chroma(5, MAJOR);
    ASSERT_EQ(k.tonic.letter, 1);
    ASSERT_EQ(k.tonic.accidental, 0);
    k = context_from_chroma(-6, MAJOR);
    ASSERT_EQ(k.tonic.letter, 6);
    ASSERT_EQ(k.tonic.accidental, -1);
}

void test_context_from_pitch(void) {
    Pitch p = {3, 0};
    TonalContext k = context_from_pitch(p, DORIAN);
    ASSERT_EQ(k.tonic.letter, 5);
    ASSERT_EQ(k.tonic.accidental, 1);
    ASSERT_EQ(k.chroma_offset, -3);
    ASSERT_EQ(k.mode, DORIAN);
    p = (Pitch){-3, 0};
    k = context_from_pitch(p, MAJOR);
    ASSERT_EQ(k.tonic.letter, 6);
    ASSERT_EQ(k.tonic.accidental, -1);
    ASSERT_EQ(k.chroma_offset, 7);
    ASSERT_EQ(k.mode, MAJOR);
}

void test_degree_number(void) {
    TonalContext k;
    context_from_str("F#", DORIAN, &k);
    Pitch p;
    pitch_from_spn("F#4", &p);
    ASSERT_EQ(degree_number(p, k), 0);
    pitch_from_spn("F4", &p);
    ASSERT_EQ(degree_number(p, k), 0);
    pitch_from_spn("C4", &p);
    ASSERT_EQ(degree_number(p, k), 4);
    pitch_from_spn("C-3", &p);
    ASSERT_EQ(degree_number(p, k), 4);
    pitch_from_spn("D10", &p);
    ASSERT_EQ(degree_number(p, k), 5);
}

void test_degree_alteration(void) {
    TonalContext k;
    context_from_str("F#", DORIAN, &k);
    Pitch p;
    pitch_from_spn("F#4", &p);
    ASSERT_EQ(degree_alteration(p, k), DIATONIC_DEG);
    pitch_from_spn("Fx4", &p);
    ASSERT_EQ(degree_alteration(p, k), RAISED_DEG);
    pitch_from_spn("F4", &p);
    ASSERT_EQ(degree_alteration(p, k), LOWERED_DEG);
    pitch_from_spn("Dx4", &p);
    ASSERT_EQ(degree_alteration(p, k), FOREIGN_DEG_SHARP);
    pitch_from_spn("Db4", &p);
    ASSERT_EQ(degree_alteration(p, k), FOREIGN_DEG_FLAT);
}

void test_degree_chroma(void) {
    TonalContext k;
    context_from_str("C", MAJOR, &k);
    ASSERT_EQ(degree_chroma(TONIC, 0, k), 0);
    ASSERT_EQ(degree_chroma(SUPERTONIC, 0, k), 2);
    ASSERT_EQ(degree_chroma(MEDIANT, 0, k), 4);
    ASSERT_EQ(degree_chroma(SUBDOMINANT, 0, k), -1);
    ASSERT_EQ(degree_chroma(DOMINANT, 0, k), 1);
    ASSERT_EQ(degree_chroma(SUBMEDIANT, 0, k), 3);
    ASSERT_EQ(degree_chroma(SUBTONIC, 0, k), 5);
    ASSERT_EQ(degree_chroma(TONIC, 1, k), 7);
    ASSERT_EQ(degree_chroma(TONIC, -1, k), -7);
    ASSERT_EQ(degree_chroma(TONIC, 3, k), 21);
    context_from_str("C", DORIAN, &k);
    ASSERT_EQ(degree_chroma(TONIC, 0, k), 0);
    ASSERT_EQ(degree_chroma(SUPERTONIC, 0, k), 2);
    ASSERT_EQ(degree_chroma(MEDIANT, 0, k), -3);
    ASSERT_EQ(degree_chroma(SUBDOMINANT, 0, k), -1);
    ASSERT_EQ(degree_chroma(DOMINANT, 0, k), 1);
    ASSERT_EQ(degree_chroma(SUBMEDIANT, 0, k), 3);
    ASSERT_EQ(degree_chroma(SUBTONIC, 0, k), -2);
    context_from_str("F#", PHRYGIAN, &k);
    ASSERT_EQ(degree_chroma(TONIC, 0, k), 6);
    ASSERT_EQ(degree_chroma(SUPERTONIC, 0, k), 1);
    ASSERT_EQ(degree_chroma(MEDIANT, 0, k), 3);
    ASSERT_EQ(degree_chroma(SUBDOMINANT, 0, k), 5);
    ASSERT_EQ(degree_chroma(DOMINANT, 0, k), 7);
    ASSERT_EQ(degree_chroma(SUBMEDIANT, 0, k), 2);
    ASSERT_EQ(degree_chroma(SUBTONIC, 0, k), 4);
}

void test_pitch_from_degree(void) {
    TonalContext k;
    context_from_str("C", MAJOR, &k);
    Pitch p = pitch_from_degree(TONIC, 0, 4, k);
    ASSERT_EQ(pitch_letter(p) + 'a', 'c');
    ASSERT_EQ(pitch_accidental(p), 0);
    ASSERT_EQ(pitch_octave(p), 4);
    p = pitch_from_degree(SUPERTONIC, 1, 3, k);
    ASSERT_EQ(pitch_letter(p) + 'a', 'd');
    ASSERT_EQ(pitch_accidental(p), 1);
    ASSERT_EQ(pitch_octave(p), 3);
    context_from_str("F#", PHRYGIAN, &k);
    p = pitch_from_degree(SUBDOMINANT, -1, 0, k);
    ASSERT_EQ(pitch_letter(p) + 'a', 'b');
    ASSERT_EQ(pitch_accidental(p), -1);
    ASSERT_EQ(pitch_octave(p), 0);
}

void test_snap_diatonic(void) {
    Pitch p, q;
    TonalContext k;
    pitch_from_spn("D#4", &p);
    pitch_from_spn("D4", &q);
    context_from_str("C", MINOR, &k);
    ASSERT_EQ(snap_diatonic(p, k).w, q.w);
    ASSERT_EQ(snap_diatonic(p, k).h, q.h);
    pitch_from_spn("Dbbb4", &p);
    ASSERT_EQ(snap_diatonic(p, k).w, q.w);
    ASSERT_EQ(snap_diatonic(p, k).h, q.h);
    context_from_str("C", PHRYGIAN, &k);
    pitch_from_spn("D4", &p);
    pitch_from_spn("Db4", &q);
    ASSERT_EQ(snap_diatonic(p, k).w, q.w);
    ASSERT_EQ(snap_diatonic(p, k).h, q.h);
}

void test_transpose_diatonic(void) {
    Pitch p, q;
    TonalContext k;
    pitch_from_spn("D#4", &p);
    pitch_from_spn("A4", &q);
    context_from_str("Bb", MAJOR, &k);
    p = transpose_diatonic(p, 4, k);
    ASSERT_EQ(snap_diatonic(p, k).w, q.w);
    ASSERT_EQ(snap_diatonic(p, k).h, q.h);
}

void test_tonality_functions(void) {
    RUN_TESTS(test_context_from_str);
    RUN_TESTS(test_context_from_chroma);
    RUN_TESTS(test_context_from_pitch);
    RUN_TESTS(test_degree_number);
    RUN_TESTS(test_degree_alteration);
    RUN_TESTS(test_degree_chroma);
    RUN_TESTS(test_pitch_from_degree);
    RUN_TESTS(test_snap_diatonic);
    RUN_TESTS(test_transpose_diatonic);
}
