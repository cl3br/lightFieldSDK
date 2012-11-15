/**
 * \file   lightFieldCalibration.cpp
 * \author Clemens Brummer
 * \date   18.10.2012
 */

#include <cstdio>
#include <assert.h>
#define _USE_MATH_DEFINES
#include "cv.h"
#include "highgui.h"

#include "lightFieldUtils.h"
#include "opencvHelpers.h"
#include "lightFieldCalibration.h"
#include "lightFieldCalibrationPrivates.h"

//#define LENSE_VIS_LINES
#define LENSE_VIS_CIRCLE

using namespace std;

cLightFieldCalibration::cLightFieldCalibration()
{
  p = new calibPrivs();
}

cLightFieldCalibration::~cLightFieldCalibration()
{
  if(p) {
    p->cleanUp();
    delete p;
  }
}

lfCalibrationParameter_t cLightFieldCalibration::getParameters(void *img)
{
  if(p->img != img) {
    p->img = (IplImage*) img;
    computeImageSpecificParams();
    computeLenseCenters();
  }
  return p->params;
}

lfError cLightFieldCalibration::loadCalibration(const char* file_name, lfCalibrationFileType_t type)
{
  RETURN_IF_NULL(file_name);

  cLightFieldCalibrationFileReader* reader = p->reader_factory->create(type);
  RETURN_IF_NULL(reader);
  RETURN_IF_FAILED(reader->read(file_name, &p->params));

  p->isCalibNew = true;
  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::computeImageSpecificParams()
{
  RETURN_IF_NULL(p->img);

  const double factor_x = 1  / (p->params.grid_step.x * p->params.diameter);
  const double factor_y = 1  / (p->params.grid_step.y * p->params.diameter);

  computeTransMat();

  lfPoint2Dd_t p0 = lfTransformPoint2D_inv(p->params.transMat, lfPoint2D(0.0,0.0));
  lfPoint2Dd_t p1 = lfTransformPoint2D_inv(p->params.transMat, lfPoint2D((double) p->img->width, (double) p->img->height));
  p->params.grid_min.x = (int) (min(p0.x, p1.x) * factor_x) - 1;
  p->params.grid_min.y = (int) (min(p0.y, p1.y) * factor_y) - 1;
  p->params.grid_max.x = (int) (max(p0.x, p1.x) * factor_x) + 1;
  p->params.grid_max.y = (int) (max(p0.y, p1.y) * factor_y) + 1;
  p->params.numX = p->params.grid_max.x - p->params.grid_min.x + 1;
  p->params.numY = p->params.grid_max.y - p->params.grid_min.y + 1;
  p->params.numLenses = p->params.numX * p->params.numY * p->params.sub_grid_nums;

  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::saveMaskImage(const char* file_name)
{
  RETURN_IF_FAILED(createImageMask());
  
  if(cvSaveImage(file_name, p->imask) == 0)
    return LF_ERR;

  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::saveMaskedImage(const char* file_name)
{
  RETURN_IF_FAILED(createImageMask());
  
  IplImage* img = CREATE_IMAGE(p->img);
  cvSet(img, cvScalar(0));
  cvCopy(p->img, img, p->imask);
  
  if(cvSaveImage(file_name, img) == 0)
    return LF_ERR;

  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::showRectifiedImage()
{
  IplImage* img = CREATE_IMAGE(p->img);

  RETURN_IF_NULL(img);
  RETURN_IF_FAILED(rectify(img));

  showImage(img, "Rectified image");
  
  cvReleaseImage(&img);
  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::saveRectifiedImage(const char* file_name)
{
  IplImage* img = CREATE_IMAGE(p->img);
  RETURN_IF_FAILED(rectify(img));
  
  if(cvSaveImage(file_name, img) == 0)
    return LF_ERR;

  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::showCenterImage()
{
  IplImage *img = cvCreateImage(cvSize(p->params.numX*2, p->params.numY), p->img->depth, p->img->nChannels);
  RETURN_IF_FAILED(computeCenterImage(img));
  showImage(img, "Center image");

  cvReleaseImage(&img);
  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::saveCenterImage(const char* file_name)
{
  IplImage *img = cvCreateImage(cvSize(p->params.numX*2, p->params.numY), p->img->depth, p->img->nChannels);
  RETURN_IF_FAILED(computeCenterImage(img));
  
  if(cvSaveImage(file_name, img) == 0)
    return LF_ERR;

  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::rectify(void* img)
{
  RETURN_IF_FAILED(computeTransMat());

  //cvWarpPerspective(p->img, img, p->params.transMat, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS+CV_WARP_INVERSE_MAP );

  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::computeTransMat()
{
  RETURN_IF_NULL(p->img);

  const double a_sin = sin(p->params.rotation);
  const double a_cos = cos(p->params.rotation);
  p->params.transMat[0] = p->params.scale_x*a_cos;
  p->params.transMat[1] = -p->params.scale_x*a_sin;
  p->params.transMat[2] =  p->params.offset.x + p->img->width / 2;
  p->params.transMat[3] =  p->params.scale_y*a_sin;
  p->params.transMat[4] =  p->params.scale_y*a_cos;
  p->params.transMat[5] =  p->params.offset.y + p->img->height / 2;
  p->params.transMat[6] =  0.0; // TODO: perspective params?
  p->params.transMat[7] =  0.0; // TODO: perspective params?
  p->params.transMat[8] =  1.0;

  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::computeLenseCenters()
{
  if(p->isCalibNew /*|| p->isImageNew*/)
  {
    const double d = p->params.diameter;
    lfPoint2Dd_t pointPattern;
    lfPoint2Dd_t pointLens;
    //CvPoint2D64f* pointLensCV;
    //CvScalar s;

    // allow up to 4 different lense types
    assert(p->params.lens_type_nums <= 4);

    // release all previous lens center matrices
    p->deleteLensCenters();

    RETURN_IF_FAILED(computeImageSpecificParams());

    // create new matrices
    for(int l = 0; l < p->params.lens_type_nums; l++) {
      /*CvMat* m = cvCreateMat(1, p->params.numLenses, CV_32FC2);*/
      vector<lfPoint2Dd_t> vec;
      p->params.lens_centers.push_back(vec);
      p->params.lens_centers[l].reserve(p->params.numLenses);
    }

    const lfPoint2Dd_t subGridStep = d * p->params.sub_grid_step;
    const lfPoint2Dd_t gridStep = d * p->params.grid_step;
    int numLens = 0;

    for (int v=(int)p->params.grid_min.y; v <= (int)p->params.grid_max.y; v++) {
      for (int u=(int)p->params.grid_min.x; u <= (int)p->params.grid_max.x; u++) {
        pointPattern = gridStep * lfPoint2D(u,v);
        for (int pat = 0; pat < p->params.sub_grid_nums; pat++) {
          for(int l = 0; l < p->params.lens_type_nums; l++)
          {
            //pointLens.x = pointPattern.x + p->params.lens_types[l].offset.x * d;
            //pointLens.y = pointPattern.y + p->params.lens_types[l].offset.y * d;
            pointLens = pointPattern + (d * p->params.lens_types[l].offset);

            pointLens = lfTransformPoint2D(p->params.transMat, pointLens);
            //s.val[0] = pointLens.x; s.val[1] = pointLens.y;
            assert(numLens < p->params.numLenses);
            //cvSet1D(p->lensCenters[l], numLens, s);
            p->params.lens_centers[l].push_back(pointLens);
          }
          pointPattern += subGridStep;
          numLens++;
        }
      }
    }
    p->isCalibNew = false;
  }  
  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::drawLenses(void* img, int line_width, int colors)
{
  const int r = static_cast<int> (p->params.diameter / 2 - p->params.lens_border);
  CvScalar *color = new CvScalar[colors];
  p->img = (IplImage*) img;
  for (int c=0; c < colors; c++) {
    color[c] = gColors[c%MY_CV_COLORS];
  }
  lfPoint2Dd_t p1;
  computeLenseCenters();

#ifdef LENSE_VIS_CIRCLE
  for (int l=0; l < p->params.lens_type_nums; l++) {
    for (int i=0; i < p->params.numLenses; i++) {
      //p1 = cvGet1D(p->lensCenters[l], i);
      p1 = p->params.lens_centers[l][i];
      cvDrawCircle(img, cvPoint((int) p1.x/*val[0]*/, (int) p1.y/*val[1]*/), r, color[l%colors], line_width);
    }
  }
#elif defined LENSE_VIS_LINES
#error LENSE_VIS_LINES not tested. behaviour undefined!
  CvScalar p2;

  for (int u=0; u < p->params.numX; u++) {
    p1 = cvGet2D(p->lensCenters, 0, u);
    p2 = cvGet2D(p->lensCenters, p->params.numY-1, u);
    cvDrawLine(img, cvPoint((int) p1.val[0]-r, (int) p1.val[1]-r), cvPoint((int) p2.val[0]-r, (int) p2.val[1]+r), color);
  }
  for (int v=0; v < p->params.numY; v++) {
    p1 = cvGet2D(p->lensCenters, v, 0);
    p2 = cvGet2D(p->lensCenters, v, p->params.numX-1);
    cvDrawLine(img, cvPoint((int) p1.val[0]-r, (int) p1.val[1]-r), cvPoint((int) p2.val[0]+r, (int) p2.val[1]-r), color);
  }
#endif
  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::computeCenterImage(void* img)
{
  RETURN_IF_FAILED(computeLenseCenters());

  IplImage* i = (IplImage*) img;
  //cvZero(img);
  //CvScalar c;
  //int x, y;

  //for (int u=0; u < i->width; u++) {
  //  for (int v=0; v < i->height; v++) {
  //    c = cvGet2D(p->lensCenters[0], v, u);
  //    x = (int) c.val[0]; y = (int) c.val[1];
  //    if(x< 0 || x >= p->img->width || y < 0 || y >= p->img->height)
  //      continue;
  //    c = cvGet2D(p->img, y, x);
  //    cvSet2D(i, v, u, c);
  //  }
  //}
  RETURN_NO_ERR;
}


lfError cLightFieldCalibration::createImageMask()
{
  p->deleteIMask();
  p->imask = cvCreateImage(cvSize(p->img->width, p->img->height), IPL_DEPTH_8U, 1);

  if (!p->img || !p->imask)
    return LF_ERR;

  cvZero(p->imask);
  RETURN_IF_FAILED(computeLenseCenters());
  drawLenses(p->imask, -1, 1);

  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::createLensMask()
{
  p->deleteLMask();
  const int d = cvRound(p->params.diameter);
  p->lmask = cvCreateImage(cvSize(d, d), IPL_DEPTH_8U, 1);

  if (!p->lmask)
    return LF_ERR;
  
  cvZero(p->lmask);
  const int npts = 6;
  CvPoint pts[npts];
  const int x1 = (int) d/2;
  const int x2 = d-1;
  const int y1 = (int) d/4;
  const int y3 = x2;
  const int y2 = y3 - y1;

  pts[0].x = x1; pts[0].y = 0;
  pts[1].x = x2; pts[1].y = y1;
  pts[2].x = x2; pts[2].y = y2;
  pts[3].x = x1; pts[3].y = y3;
  pts[4].x = 0;  pts[4].y = y2;
  pts[5].x = 0;  pts[5].y = y1;

  cvFillConvexPoly(p->lmask, pts, 6, cvScalar(MY_CV_WHITE));

  RETURN_NO_ERR;
}