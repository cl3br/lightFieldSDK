/**
 * \file   lightFieldDepthGeneratorPlane.cpp
 * \author Clemens Brummer
 * \date   03.11.2012
 */

#include "cv.h"
#include "opencvHelpers.h"
#include "lightFieldDepthGeneratorPlane.h"

lfError cLightFieldDepthGeneratorPlane::generate(void* raw_image, lfDepthGeneratorParameter_t* params)
{
  IplImage* img = static_cast<IplImage*> (raw_image);
  IplImage* depth = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_64F,1);
  RETURN_IF_NULL(depth);

  cvSet(depth, cvScalarAll(params->plane));
  _depthImage = depth;

  RETURN_NO_ERR;
}