#ifndef OPENCV_HELPERS_H
#define OPENCV_HELPERS_H

#include "cv.h"
#include "highgui.h"

#define LOG cout


// MACROS
#define CREATE_IMAGE(i) cvCreateImage(cvSize(i->width, i->height), i->depth, i->nChannels)
#define CREATE_MAT(m) cvCreateMat(m->rows, m->cols, m->type)
#define DELETE_IMAGE(i) if (i) { cvReleaseImage((IplImage**)&i); i = NULL; }
#define IS_POINT_IN_IMAGE(img,x,y) ((x) >= 0.0 && (y) >= 0.0 && (x) < (img)->width && (y) < (img)->height)

// colors

#define MY_CV_COLORS    7
#define MY_CV_WHITE     255,255,255
#define MY_CV_RED       0,0,255
#define MY_CV_GREEN     0,255,0
#define MY_CV_BLUE      255,0,0
#define MY_CV_YELLOW    0,255,255
#define MY_CV_MAGENTA   255,0,255
#define MY_CV_CYAN      255,255,0
#define MY_CV_BLACK     0,0,0

const static CvScalar gColors[MY_CV_COLORS] = { 
  {MY_CV_BLUE},
  {MY_CV_GREEN},
  {MY_CV_RED},
  {MY_CV_YELLOW},
  {MY_CV_MAGENTA},
  {MY_CV_CYAN},
  {MY_CV_WHITE},
};

/// po = T * pi;
CvPoint2D64f transformPoint(CvMat* M, CvPoint2D64f pi);

/// pi = T * po;
CvPoint2D64f transformPoint_inv(CvMat* M, CvPoint2D64f pi);

void showImage(IplImage* img, const char* name, int delay = 0);

// interpolation (until now only bilinear)
CvScalar get2DInterpolated(CvArr* arr, double x, double y);

/**
 * Print a matrix to consol
 * \param M the matrix to be printed
 */
void printMatrix(CvMat* M);

void print3ChannelVectorMatrix(CvMat* M, int nChannels);

void printVectorMatrixToFile(CvMat* M, int nChannels, const char* file_name);

#undef LOG

#ifdef __cplusplus
inline CvScalar operator+(CvScalar s1, CvScalar s2)
{
  CvScalar s;
  for(int i=0; i<4; i++)
    s.val[i] = s1.val[i] + s2.val[i];

  return s;
}

inline CvScalar operator*(double s1, CvScalar s2)
{
  CvScalar s;
  for(int i=0; i<4; i++)
    s.val[i] = s1 * s2.val[i];

  return s;
}

inline CvScalar operator*(CvScalar s1, CvScalar s2)
{
  CvScalar s;
  for(int i=0; i<4; i++)
    s.val[i] = s1.val[i] * s2.val[i];

  return s;
}
#endif
#endif // OPENCV_HELPERS_H