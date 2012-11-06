/**
 * \file   lightFieldCorePrivates.h
 * \author Clemens Brummer
 * \date   03.11.2012
 */

#ifndef LIGHT_FIELD_CORE_PRIVS_H
#define LIGHT_FIELD_CORE_PRIVS_H

#include "cv.h"

#include "lfTypes.h"
#include "lightFieldCalibration.h"
#include "lightFieldDepthGeneratorFactory.h"
#include "lightFieldViewGeneratorFactory.h"

struct corePrivs
{
  corePrivs();
  ~corePrivs();
  void cleanUp();
  void deleteRawImage();
  void deleteWhiteImage();
  void deleteProcImage();

  IplImage* _raw_img;
  IplImage* _white_img;
  IplImage* _proc_img;
  cLightFieldCalibration* _calib;
  cLightFieldDepthGeneratorFactory* _depth_generator_factory;
  cLightFieldViewGeneratorFactory* _view_generator_factory;

  lfCalibrationFileType_t _calib_type;
  lfDepthGeneratorType_t _depth_gen_type;
  lfViewGeneratorType_t _view_gen_type;

  bool isImageNew;
};

#endif // LIGHT_FIELD_CORE_PRIVS_H