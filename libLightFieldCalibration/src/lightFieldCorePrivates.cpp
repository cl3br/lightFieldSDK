/**
 * \file   lightFieldCorePrivates.cpp
 * \author Clemens Brummer
 * \date   03.11.2012
 */
#include "opencvhelpers.h"
#include "lightFieldCorePrivates.h"
corePrivs::corePrivs()
{
  _raw_img = NULL;
  _white_img = NULL;
  _proc_img = NULL;

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
  DELETE_IMAGE(_raw_img);
}

void corePrivs::deleteWhiteImage()
{
  DELETE_IMAGE(_white_img);
}

void corePrivs::deleteProcImage()
{
  DELETE_IMAGE(_proc_img);
}