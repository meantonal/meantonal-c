# Table of Contents

- [Meantonal](#meantonal)
- [Installation](#installation)

<img align="left" src="/logo.svg" width="48">
  
# Meantonal

Meantonal is a specification for representing pitch information in Western music, and a suite of tools for operating on this information. It's a small, focused library that aims to empower developers to build musical apps more easily.

Meantonal is:

- **Flexible with I/O**: easily ingest and translate between Scientific Pitch Notation, Helmholtz notation, ABC and Lilypond. Extract MIDI values at any time.
- **Semantically nondestructive**: the distinction between enharmonic notes such as $\sf{C}\sharp$ and $\sf{D}\flat$ is maintained. Things that don't behave the same way musically are not encoded the same way in Meantonal.
- **Just vectors**: under the hood [pitches](https://meantonal.org/learn/pitch/) and [intervals](https://meantonal.org/learn/intervals/) are 2d vectors. Operations are simple to understand, surprisingly powerful, and fast to execute.
- **Tuning-agnostic**: Target any meantone tuning system, not just 12-tone equal temperament. You want 31 tones per octave? Done.

# Installation

Adding Meantonal to your project is as simple as copying the [`meantonal.h`](https://raw.githubusercontent.com/meantonal/meantonal/refs/heads/master/meantonal.h) header file somewhere into your project's directory, which can be done via the following bash script:

```bash
curl https://meantonal.org/meantonal.h > meantonal.h
```

You must `#define MEANTONAL` in exactly **one** file before the first inclusion of `meantonal.h`:

```c
// foo.c
#define MEANTONAL
#include "meantonal.h"

Pitch c4, e4; // declares but does not assign values to two Pitch vectors
Pitch g4 = { 28, 11 }; // directly initialises a Pitch
// assigning values
if (pitch_from_spn("C4", &c4))
    fprintf(stderr, "error parsing pitch from SPN");
if (pitch_from_spn("E4", &e4))
    fprintf(stderr, "error parsing pitch from SPN");

Interval M3 = interval_between(c4, e4); // creates a major 3rd Interval vector
Interval P5 = interval_between(c4, g5); // creates a perfect 5th interval vector

// etc.
```

After that, just include `meantonal.h` in any other files as needed.

```c
// bar.c
#include "meantonal.h"

Pitch p;
if (pitch_from_spn("E4", &p))
    fprintf(stderr, "error parsing pitch from SPN");

MirrorAxis a;
if (axis_from_spn("C4", "G4"))
    fprintf(stderr, "error parsing pitch(s) from SPN when creating MirrorAxis");

pitch_invert(p, a); // p has been inverted about the axis to Eb4

// etc.
```
