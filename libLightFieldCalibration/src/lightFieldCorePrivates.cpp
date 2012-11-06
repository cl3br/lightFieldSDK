/**
 * \file   lightFieldCorePrivates.cpp
 * \author Clemens Brummer
 * \date   03.11.2012
 */

#include "lightFieldCorePrivates.h"
corePrivs::corePrivs()
{
  _raw_img = NULL;
  _calib = new cLightFieldCalibration();
  _depth_generator_factory = new cLightFieldDepthGeneratorFactory();
  _view_generator_factory = new cLightFieldViewGeneratorFactory();

  _calib_type = LF_CALIBRATION_FILE_UNKNOWN;
  _depth_gen_type = LF_DEPTH_GENERATOR_UNKNOWN;
  _view_gen_type = LF_VIEW_GENERATOR_UNKNOWN;

  isImageNew = false;
}


corePrivs::~corePrivs()
{
  cleanUp();
  delete _calib;
  delete _depth_generator_factory;
  delete _view_generator_factory;
}

void corePrivs::cleanUp()
{
  deleteRawImage();
}

void corePrivs::deleteRawImage()
{
  if (_raw_img) {
    cvReleaseImage(&_raw_img);
    _raw_img = NULL;
  }
}