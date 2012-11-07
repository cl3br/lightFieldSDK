#include "lightFieldUtils.h"

// Macros
#define LF_DELETE(p) if(p) { delete p; p = NULL;}

lfPoint2D_t lfTransformPoint2D(lfTransMat2D_t M, lfPoint2D_t pi)
{
  lfPoint2D_t po;
  
  po.x = M[0] * pi.x + M[1] * pi.y + M[2];
  po.y = M[3] * pi.x + M[4] * pi.y + M[5];
  {
    const double s = M[6] * pi.x + M[7] * pi.y + 1;
    po.x /= s;
    po.y /= s;
  }
  return po;
}

double determinant(lfTransMat2D_t M)
{  
  return M[0] * M[4] * M[8] + M[1] * M[5] * M[6] + M[2] * M[3] * M[7] -
         M[2] * M[4] * M[6] - M[1] * M[3] * M[8] * M[0] * M[5] * M[7];
}

void invertMatrix(lfTransMat2D_t M, lfTransMat2D_t M_inv)
{
  const double inv_det = 1/determinant(M);
  M_inv[0] = inv_det * (M[4] * M[8] - M[5] * M[7]);
  M_inv[1] = inv_det * (M[7] * M[2] - M[8] * M[1]);
  M_inv[2] = inv_det * (M[1] * M[5] - M[2] * M[4]);
  M_inv[3] = inv_det * (M[5] * M[6] - M[3] * M[8]);
  M_inv[4] = inv_det * (M[8] * M[0] - M[6] * M[2]);
  M_inv[5] = inv_det * (M[2] * M[3] - M[0] * M[5]);
  M_inv[6] = inv_det * (M[3] * M[7] - M[4] * M[6]);
  M_inv[7] = inv_det * (M[6] * M[1] - M[7] * M[0]);
  M_inv[8] = inv_det * (M[0] * M[4] - M[1] * M[3]);
}

lfPoint2D_t lfTransformPoint2D_inv(lfTransMat2D_t M, lfPoint2D_t pi)
{
  lfPoint2D_t po;
  lfTransMat2D_t invM;
  invertMatrix(M, invM);

  po = lfTransformPoint2D(invM, pi);    
  return po;
}