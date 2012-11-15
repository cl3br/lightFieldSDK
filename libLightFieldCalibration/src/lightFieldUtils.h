#ifndef LIGHT_FIELD_UTILS_H
#define LIGHT_FIELD_UTILS_H

#include "lfTypes.h"

// Macros
#define LF_DELETE(p) if(p) { delete p; p = NULL;}

lfPoint2Dd_t lfTransformPoint2D(lfTransMat2D_t M, lfPoint2Dd_t pi);
lfPoint2Dd_t lfTransformPoint2D_inv(lfTransMat2D_t M, lfPoint2Dd_t pi);
double determinant(lfTransMat2D_t M);
void invertMatrix(lfTransMat2D_t M, lfTransMat2D_t M_inv);

double abs(lfPoint2Dd_t p);

#endif // LIGHT_FIELD_UTILS_H