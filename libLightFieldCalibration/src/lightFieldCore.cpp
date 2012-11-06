/**
 * \file   lightFieldCore.cpp
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#include "cv.h"
#include "highgui.h"
#include "lightFieldCore.h"
#include "lightFieldCorePrivates.h"


cLightFieldCore::cLightFieldCore()
{
  p = new corePrivs();
}

cLightFieldCore::~cLightFieldCore()
{
  if(p) {
    p->cleanUp();
    delete p;
  }
}

lfError cLightFieldCore::loadCalibration(const char* file_name, lfCalibrationFileType_t type)
{
  RETURN_IF_FAILED(p->_calib->loadCalibration(file_name, type));
  RETURN_NO_ERR;
}

lfError cLightFieldCore::loadImage(const char* file_name)
{
  RETURN_IF_NULL(file_name);

  p->deleteRawImage();
  p->_raw_img = cvLoadImage(file_name);
  RETURN_IF_NULL(p->_raw_img);

  p->isImageNew = true;

  RETURN_NO_ERR;
}

// TODO: load white image
// TODO: generate processed image (e.g. rectified white filtered)
// TODO: light field type?
// TODO: 