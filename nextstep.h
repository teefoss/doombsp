#ifndef NEXTSTEP_H
#define NEXTSTEP_H

#define MAX(a, b)       (((a) > (b)) ? (a) : (b))
#define MIN(A,B)        ((A) < (B) ? (A) : (B))

/* From NeXTSTEP 3.3 Documentation: */

typedef float NXCoord;

typedef struct {
    NXCoord x;
    NXCoord y;
} NXPoint;

typedef struct {
    NXCoord width;
    NXCoord height;
} NXSize;

typedef struct {
    NXPoint origin;
    NXSize size;
} NXRect;

/*
 *  As its name suggests, NXEraseRect() erases the rectangle referred to by its
 *  argument, filling it with white.  It does not alter the current color.
 */
void NXEraseRect(const NXRect *aRect);

void NXPing(void);

#endif /* NEXTSTEP_H */
