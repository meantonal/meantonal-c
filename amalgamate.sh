#!/usr/bin/env bash
set -e

OUT="meantonal.h"

# start fresh
printf "#include <stdlib.h>\n" > "$OUT"
printf "#include <stdbool.h>\n\n" >> "$OUT"
printf "// -----------------------------------------\n" >> "$OUT"
printf "// HEADER DECLARATIONS ---------------------\n" >> "$OUT"
printf "// -----------------------------------------\n\n" >> "$OUT"
printf "#ifndef MEANTONAL_HEADER\n" >> "$OUT"
printf "#define MEANTONAL_HEADER\n\n" >> "$OUT"

# helper: strip include guards and local includes
strip_headers() {
    sed -E '/^#ifndef|^#define|^#endif/d; /^#include/d'
}

strip_headers < "include/types.h" >> "$OUT"
strip_headers < "include/constants.h" >> "$OUT"
strip_headers < "include/pitch.h" >> "$OUT"
strip_headers < "include/interval.h" >> "$OUT"
strip_headers < "include/key.h" >> "$OUT"

printf "#endif // MEANTONAL_HEADER\n\n" >> "$OUT"
printf "// -----------------------------------------\n" >> "$OUT"
printf "// IMPLEMENTATION --------------------------\n" >> "$OUT"
printf "// -----------------------------------------\n\n" >> "$OUT"
printf "#ifdef MEANTONAL\n" >> "$OUT"
printf "#undef MEANTONAL\n\n" >> "$OUT"

strip_headers < "src/constants.c" >> "$OUT"
strip_headers < "src/pitch.c" >> "$OUT"
strip_headers < "src/interval.c" >> "$OUT"
strip_headers < "src/key.c" >> "$OUT"
# for src in src/*.c; do
#     sed -E '/^#include/d' "$src" >> "$OUT"
#     echo "" >> "$OUT"
# done

printf "#endif // MEANTONAL\n\n" >> "$OUT"
