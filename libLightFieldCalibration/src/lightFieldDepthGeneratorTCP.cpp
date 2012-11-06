/**
 * \file   lightFieldDepthGeneratorTCP.cpp
 * \author Clemens Brummer
 * \date   03.11.2012
 */

#include "cv.h"
#include "lfError.h"
#include "lightFieldDepthGeneratorTCP.h"

lfError cLightFieldDepthGeneratorTCP::generate(void* raw_image, lfCalibrationParameter_t* params, void* depth_map)
{
  static_cast<IplImage*> (raw_image);
  IplImage* depth = static_cast<IplImage*> (depth_map);

  cvSet(depth, cvScalarAll(params->tcp));

  RETURN_NO_ERR;
}