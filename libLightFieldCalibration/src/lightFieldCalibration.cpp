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

#include "opencvHelpers.h"
#include "lightFieldCalibrationFileReader.h"
#include "lightFieldCalibrationFileReaderFactory.h"
#include "lightFieldCalibration.h"
#include "lightFieldCalibrationPrivates.h"

//#define LENSE_VIS_LINES
#define LENSE_VIS_CIRCLE


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

  p->params.offset.x += p->img->width / 2;
  p->params.offset.y += p->img->height / 2;

  computeTransMat();

  CvPoint2D64f p0 = transformPoint_inv(p->transMat, cvPoint2D64f(0,0));
  CvPoint2D64f p1 = transformPoint_inv(p->transMat, cvPoint2D64f(p->img->width,p->img->height));
  p->grid_min.x = (int) (min(p0.x, p1.x) * factor_x) - 1;
  p->grid_min.y = (int) (min(p0.y, p1.y) * factor_y) - 1;
  p->grid_max.x = (int) (max(p0.x, p1.x) * factor_x) + 1;
  p->grid_max.y = (int) (max(p0.y, p1.y) * factor_y) + 1;
  p->numX = p->grid_max.x - p->grid_min.x + 1;
  p->numY = p->grid_max.y - p->grid_min.y + 1;
  p->numLenses = p->numX * p->numY * p->params.sub_grid_nums;

  //p->isImageNew = false;

  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::showCalibImage()
{
  IplImage* img = cvCloneImage(p->img);
  drawLenses(img, 1, p->params.lensTypes);
  showImage(img, "Calibration Debug Image");

  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::saveCalibImage(const char* file_name)
{
  IplImage* img = cvCloneImage(p->img);
  RETURN_IF_FAILED(drawLenses(img, 1, p->params.lensTypes));

  if(cvSaveImage(file_name, img) == 0)
    return LF_ERR;

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
  IplImage *img = cvCreateImage(cvSize(p->numX*2, p->numY), p->img->depth, p->img->nChannels);
  RETURN_IF_FAILED(computeCenterImage(img));
  showImage(img, "Center image");

  cvReleaseImage(&img);
  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::saveCenterImage(const char* file_name)
{
  IplImage *img = cvCreateImage(cvSize(p->numX*2, p->numY), p->img->depth, p->img->nChannels);
  RETURN_IF_FAILED(computeCenterImage(img));
  
  if(cvSaveImage(file_name, img) == 0)
    return LF_ERR;

  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::rectify(void* img)
{
  RETURN_IF_FAILED(computeTransMat());

  cvWarpPerspective(p->img, img, p->transMat, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS+CV_WARP_INVERSE_MAP );

  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::computeTransMat()
{
  if (p->transMat)
    cvReleaseMat(&p->transMat);

  p->transMat = cvCreateMat(3, 3, CV_32FC1);
  
  const double a_sin = sin(p->params.rotation);
  const double a_cos = cos(p->params.rotation);
  cvSetReal2D(p->transMat, 0, 0, p->params.scale_x*a_cos);
  cvSetReal2D(p->transMat, 0, 1,-p->params.scale_x*a_sin);
  cvSetReal2D(p->transMat, 0, 2, p->params.offset.x);
  cvSetReal2D(p->transMat, 1, 0, p->params.scale_y*a_sin);
  cvSetReal2D(p->transMat, 1, 1, p->params.scale_y*a_cos);
  cvSetReal2D(p->transMat, 1, 2, p->params.offset.y);
  cvSetReal2D(p->transMat, 2, 0, 0.0); // TODO: perspective params?
  cvSetReal2D(p->transMat, 2, 1, 0.0); // TODO: perspective params?
  cvSetReal2D(p->transMat, 2, 2, 1.0);

  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::computeLenseCenters()
{
  if(p->isCalibNew /*|| p->isImageNew*/)
  {
    const double d = p->params.diameter;
    lfPoint2D_t pointPattern;
    CvPoint2D64f pointLens;
    CvScalar s;

    // allow up to 4 different lense types
    assert(p->params.lensTypes <= 4);

    // release all previous lens center matrices
    p->deleteLensCenters();

//    if(p->isImageNew)
      computeImageSpecificParams();
    //else if(p->isCalibNew)
    //  computeTransMat();

    // create new matrices
    for(int l = 0; l < p->params.lensTypes; l++) {
      CvMat* m = cvCreateMat(1, p->numLenses, CV_32FC2);
      if(!m) {
        p->deleteLensCenters();
        return LF_ERR;
      }
      p->lensCenters.push_back(m);
    }

    const lfPoint2D_t subGridStep = d * p->params.sub_grid_step;
    const lfPoint2D_t gridStep = d * p->params.grid_step;
    int numLens = 0;

    for (int u=p->grid_min.x; u <= p->grid_max.x; u++) {
      for (int v=p->grid_min.y; v <= p->grid_max.y; v++) {
        pointPattern = gridStep * lfPoint2D(u,v);
        for (int pat = 0; pat < p->params.sub_grid_nums; pat++) {
          for(int l = 0; l < p->params.lensTypes; l++)
          {
            pointLens.x = pointPattern.x + p->params.lenses[l].offset.x * d;
            pointLens.y = pointPattern.y + p->params.lenses[l].offset.y * d;
            pointLens = transformPoint(p->transMat, pointLens);
            s.val[0] = pointLens.x; s.val[1] = pointLens.y;
            assert(numLens < p->numLenses);
            cvSet1D(p->lensCenters[l], numLens, s);
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
  for (int c=0; c < colors; c++) {
    color[c] = gColors[c%MY_CV_COLORS];
  }
  CvScalar p1;
  computeLenseCenters();

#ifdef LENSE_VIS_CIRCLE
  for (int l=0; l < p->params.lensTypes; l++) {
    for (int i=0; i < p->numLenses; i++) {
      p1 = cvGet1D(p->lensCenters[l], i);
      cvDrawCircle(img, cvPoint((int) p1.val[0], (int) p1.val[1]), r, color[l%colors], line_width);
    }
  }
#elif defined LENSE_VIS_LINES
#error LENSE_VIS_LINES not tested. behaviour undefined!
  CvScalar p2;

  for (int u=0; u < p->numX; u++) {
    p1 = cvGet2D(p->lensCenters, 0, u);
    p2 = cvGet2D(p->lensCenters, p->numY-1, u);
    cvDrawLine(img, cvPoint((int) p1.val[0]-r, (int) p1.val[1]-r), cvPoint((int) p2.val[0]-r, (int) p2.val[1]+r), color);
  }
  for (int v=0; v < p->numY; v++) {
    p1 = cvGet2D(p->lensCenters, v, 0);
    p2 = cvGet2D(p->lensCenters, v, p->numX-1);
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



lfError cLightFieldCalibration::extractLensImages()
{
  RETURN_IF_FAILED(computeLenseCenters());

  RETURN_IF_FAILED(createLensMask());

  p->deleteLensImages();

  // create new lens images
  IplImage* img = NULL;
  p->lensImages.reserve(p->params.lensTypes);
  for(int l = 0; l < p->params.lensTypes; l++) {  
    vector<IplImage*> v(p->numLenses);
    p->lensImages.push_back(v);
    for(int i = 0; i < p->numLenses; i++) {
      img = cvCreateImage(cvSize(p->lmask->width, p->lmask->height), p->img->depth, p->img->nChannels);
      if(!img) {
        printf("Not enought memory to create all lens images!\n");
        p->deleteLensImages();
        return LF_ERR;
      }
      p->lensImages[l][i] = img;
    }
  }

  for (int l=0; l < p->params.lensTypes; l++) {
    for (int i=0; i < p->numLenses; i++) {
      getLensImage(l, i, p->lensImages[l][i], true);
    }
  }

  for (int i = 500; i<620; i+=2) 
    showImage(p->lensImages[1][i], "lens image");

  RETURN_NO_ERR;
}

lfError cLightFieldCalibration::getLensImage(int lens_type, int lens_num, void* lens_img, bool useMask)
{
  if (p->lensCenters.empty())
    return LF_ERR;

  IplImage* img = (IplImage*) lens_img;
  CvScalar lc;
  CvRect roi;
  const int r = cvRound(p->params.diameter/2);

  lc = cvGet1D(p->lensCenters[lens_type], lens_num);
  roi.x = (int) lc.val[0] - r;
  roi.y = (int) lc.val[1] - r;
  roi.height = p->lmask->height;
  roi.width = p->lmask->width;
  cvZero(img);

  if(roi.x < 0 || roi.y < 0 || roi.x + roi.width >= p->img->width || roi.y + roi.height >= p->img->height)
    return LF_ERR;

  cvSetImageROI(p->img, roi);
  cvCopy(p->img, img, useMask ? p->lmask : NULL);

  RETURN_NO_ERR;
}
