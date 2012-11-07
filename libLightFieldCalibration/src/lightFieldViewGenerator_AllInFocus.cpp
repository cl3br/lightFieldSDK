/**
 * \file   lightFieldViewGenerator.cpp
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#include "lightFieldViewGenerator_AllInFocus.h"

lfError cLightFieldViewGenerator_AllInFocus::generate(void *raw_image, void *depth_image, lfCalibrationParameter_t params)
{
  IplImage* img = static_cast<IplImage*> (raw_image);
  IplImage* depth = static_cast<IplImage*> (depth_image);
  IplImage* view = CREATE_IMAGE(img);
  RETURN_IF_NULL(view);

  // TODO: view generieren
  _view = view;

  RETURN_NO_ERR;
}