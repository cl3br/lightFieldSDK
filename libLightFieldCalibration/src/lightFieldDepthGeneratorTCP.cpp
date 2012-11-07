/**
 * \file   lightFieldDepthGeneratorTCP.cpp
 * \author Clemens Brummer
 * \date   03.11.2012
 */

#include "cv.h"
#include "opencvHelpers.h"
#include "lfError.h"
#include "lightFieldDepthGeneratorTCP.h"

lfError cLightFieldDepthGeneratorTCP::generate(void* raw_image, lfCalibrationParameter_t params)
{
  IplImage* img = static_cast<IplImage*> (raw_image);
  IplImage* depth = CREATE_IMAGE(img);
  RETURN_IF_NULL(depth);

  cvSet(depth, cvScalarAll(params.tcp));
  _depthImage = depth;

  RETURN_NO_ERR;
}