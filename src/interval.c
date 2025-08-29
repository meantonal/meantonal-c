#include "../include/interval.h"
#include "../include/note.h"

int interval_from_spn(char *p_str, char *q_str, Interval *out) {
    Note p, q;
    if (parse_spn(p_str, &p))
        return 1;
    if (parse_spn(q_str, &q))
        return 1;
    out->w = q.w - p.w;
    out->h = q.h - p.h;
    return 0;
}
