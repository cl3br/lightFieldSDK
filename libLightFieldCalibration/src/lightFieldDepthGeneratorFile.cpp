/**
 * \file   lightFieldDepthGeneratorFile.cpp
 * \author Clemens Brummer
 * \date   03.11.2012
 */

#include "cv.h"
#include "opencvHelpers.h"
#include "lightFieldDepthGeneratorFile.h"

lfError cLightFieldDepthGeneratorFile::generate(void* raw_image, lfDepthGeneratorParameter_t* params)
{
  IplImage* img = static_cast<IplImage*> (raw_image);
  IplImage* depth = cvLoadImage(params->file_name, CV_LOAD_IMAGE_GRAYSCALE | CV_LOAD_IMAGE_ANYDEPTH);// CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR/*, CV_LOAD_IMAGE_ANYDEPTH*/);
  RETURN_IF_NULL(depth);
  IplImage *virtualDepth = cvCreateImage(cvSize(depth->width,depth->height), IPL_DEPTH_64F, depth->nChannels);
  IplImage *resizedDepth = cvCreateImage(cvSize(img->width,img->height), virtualDepth->depth, virtualDepth->nChannels);

  // convvert to virtual depth (vd = 1/(1-d/d_max))
  // d_max for 16 bit image is 65535
  const double scale = 1.0/65535.0;
  CvScalar s;
  double vd;
  for(int y=0; y < depth->height; y++){
    for(int x=0; x < depth->width; x++) {
      s = cvGet2D(depth, y, x);
      vd = -1 / (1 - s.val[0]*scale);
      cvSet2D(virtualDepth, y, x, cvScalar(vd));
    }
  }
  cvResize(virtualDepth, resizedDepth, img->width/depth->width);
  cvReleaseImage(&virtualDepth);

  _depthImage = resizedDepth;
  RETURN_NO_ERR;
}