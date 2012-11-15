/**
 * \file   lightFieldCorePrivates.h
 * \author Clemens Brummer
 * \date   03.11.2012
 */

#ifndef LIGHT_FIELD_CORE_PRIVS_H
#define LIGHT_FIELD_CORE_PRIVS_H

#include <vector>
#include "cv.h"

#include "lfTypes.h"
#include "lightFieldCalibration.h"
#include "lightFieldDepthGeneratorFactory.h"
#include "lightFieldViewGeneratorFactory.h"

using namespace std;

//      lens_type < number < lens_img > > >
typedef vector< vector< IplImage* > > lfImg_t;

struct corePrivs
{
  corePrivs();
  ~corePrivs();
  void cleanUp();
  void deleteRawImage();
  void deleteWhiteImage();
  void deleteProcImage();
  void deleteLensImages();

  IplImage* _img;
  IplImage* _raw_img;
  IplImage* _white_img;
  IplImage* _proc_img;

  cLightFieldCalibration* _calib;
  cLightFieldDepthGenerator* _depth_generator;
  cLightFieldViewGenerator* _view_generator;
  cLightFieldDepthGeneratorFactory* _depth_generator_factory;
  cLightFieldViewGeneratorFactory* _view_generator_factory;

  lfCalibrationFileType_t _calib_type;
  lfDepthGeneratorType_t _depth_gen_type;
  lfViewGeneratorType_t _view_gen_type;
  
  lfImg_t _lensImages;             ///> all lense images

  bool isImageNew;
};

#endif // LIGHT_FIELD_CORE_PRIVS_H