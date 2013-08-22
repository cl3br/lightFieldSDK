#include <iostream>
#include "opencvHelpers.h"

#define LOG cout
using namespace std;

CvScalar get2DInterpolated(CvArr* arr, double x, double y)
{
  const int x1 = cvFloor(x);
  const int x2 = cvCeil(x);
  const int y1 = cvFloor(y);
  const int y2 = cvCeil(y);
  return  (x2-x) * (y2-y) * cvGet2D(arr, y1, x1) + \
          (x-x1) * (y2-y) * cvGet2D(arr, y1, x2) + \
          (x2-x) * (y-y1) * cvGet2D(arr, y2, x1) + \
          (x-x1) * (y-y1) * cvGet2D(arr, y2, x2);  
}


/// po = T * pi;
CvPoint2D64f transformPoint(CvMat* M, CvPoint2D64f pi)
{
  CvPoint2D64d po;
  const float* pM = M->data.fl;
  assert(M->cols == 3 && M->rows == 3);
  
  po.x = pM[0] * pi.x + pM[1] * pi.y + pM[2];
  po.y = pM[3] * pi.x + pM[4] * pi.y + pM[5];
  {
    const double s = pM[6] * pi.x + pM[7] * pi.y + 1;
    po.x /= s;
    po.y /= s;
  }
  return po;
}

/// pi = T * po;
CvPoint2D64f transformPoint_inv(CvMat* M, CvPoint2D64f pi)
{
  CvMat* m = CREATE_MAT(M);
  cvInvert(M,m);
  return transformPoint(m,pi);
}

void showImage(IplImage* img, const char* name, int delay)
{
  IplImage* _img = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, img->nChannels);
  cvConvert(img, _img);
  cvNamedWindow(name,0);
  cvShowImage(name, _img);
  cvWaitKey(delay);
  cvReleaseImage(&_img);
  cvDestroyWindow(name);
}

/**
 * Print a matrix to consol
 * \param M the matrix to be printed
 */
void printMatrix(CvMat* M) {

    // dont print empty matrices
    if (!M) {
        LOG << "---" << endl;
        return;
    }
    // loop through columns and rows of the matrix
    for (int i = 0; i < M->rows; i++) {
        for (int j = 0; j < M->cols - 1; j++) {
            LOG << cvGetReal2D(M, i, j) << ", ";
        }
        LOG << cvGetReal2D(M, i, M->cols - 1) << endl;
    }
}

void print3ChannelVectorMatrix(CvMat* M, int nChannels) {

    // dont print empty matrices
    if (!M) {
        LOG << "---" << endl;
        return;
    }
    // loop through columns and rows of the matrix
    for (int i = 0; i < M->rows; i++) {
        CvScalar s = cvGet1D(M, i);
        for (int i=0; i < nChannels; i++) {
            LOG << s.val[i] << ", ";
        }
        LOG << endl;
    }

}

void printVectorMatrixToFile(CvMat* M, int nChannels, const char* file_name)
{
    FILE* f = fopen(file_name, "w");
    if (!M) {
        fprintf(f,"---\n");
    } else {
        // loop through rows and channels of the matrix
        for (int i = 0; i < M->rows; i++) {
            CvScalar s = cvGet1D(M, i);
            for (int j=0; j < nChannels; j++) {
                fprintf(f, "%e  ", s.val[j]);
            }
            fprintf(f,"\n");
        }
    }
    fclose(f);
}

#undef LOG