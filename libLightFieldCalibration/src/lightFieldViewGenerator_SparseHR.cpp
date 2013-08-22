/**
 * \file   lightFieldViewGenerator_SparseHR.cpp
 * \author Clemens Brummer
 * \date   20.11.2012
 */
#include <vector>

#include "cv.h"
#include "opencvHelpers.h"
#include "lightFieldViewGenerator_SparseHR.h"

lfError cLightFieldViewGenerator_SparseHR::generate(void *raw_image, void *depth_image, lfCalibrationParameter_t* cparams, lfViewGeneratorParameter_t* vparams)
{
  IplImage* img = static_cast<IplImage*> (raw_image);
  IplImage* depthImg = static_cast<IplImage*> (depth_image);
  IplImage* view = CREATE_IMAGE(img);
  _lens_mask = cparams->createLensMask();
  IplImage* mask = (IplImage*) _lens_mask;
  RETURN_IF_NULL(img && view && depthImg && mask);

  cvZero(view);

  _view = view;

  RETURN_NO_ERR;
}