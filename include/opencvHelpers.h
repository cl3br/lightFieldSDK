#ifndef OPENCV_HELPERS_H
#define OPENCV_HELPERS_H

#include <iostream>
#include "cv.h"
#include "highgui.h"

#define LOG cout


// MACROS
#define CREATE_IMAGE(i) cvCreateImage(cvSize(i->width, i->height), i->depth, i->nChannels)
#define CREATE_MAT(m) cvCreateMat(m->rows, m->cols, m->type)

// colors

#define MY_CV_COLORS    8
#define MY_CV_WHITE     255,255,255
#define MY_CV_RED       0,0,255
#define MY_CV_GREEN     0,255,0
#define MY_CV_BLUE      255,0,0
#define MY_CV_YELLOW    0,255,255
#define MY_CV_MAGENTA   255,0,255
#define MY_CV_CYAN      255,255,0

const static CvScalar gColors[MY_CV_COLORS] = { 
  {MY_CV_BLUE},
  {MY_CV_GREEN},
  {MY_CV_RED},
  {MY_CV_YELLOW},
  {MY_CV_MAGENTA},
  {MY_CV_CYAN}
};

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

void showImage(IplImage* img, const char* name, int delay = 0)
{    
    cvNamedWindow(name);
    cvShowImage(name, img);
    cvWaitKey(delay);
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
#endif OPENCV_HELPERS_H