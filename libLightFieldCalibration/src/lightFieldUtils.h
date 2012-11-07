#ifndef LIGHT_FIELD_UTILS_H
#define LIGHT_FIELD_UTILS_H

#include "lfTypes.h"

// Macros
#define LF_DELETE(p) if(p) { delete p; p = NULL;}

lfPoint2D_t lfTransformPoint2D(lfTransMat2D_t M, lfPoint2D_t pi);
lfPoint2D_t lfTransformPoint2D_inv(lfTransMat2D_t M, lfPoint2D_t pi);
double determinant(lfTransMat2D_t M);
void invertMatrix(lfTransMat2D_t M, lfTransMat2D_t M_inv);

#endif // LIGHT_FIELD_UTILS_H