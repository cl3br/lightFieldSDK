/**
 * \file   lightFieldDepthGenerator.h
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#ifndef LIGHT_FIELD_DEPTH_GENERATOR_H
#define LIGHT_FIELD_DEPTH_GENERATOR_H

#include "lfError.h"
#include "lightFieldDepthGeneratorParameter.h"
#include "lightFieldCalibrationParameter.h"

class cLightFieldDepthGenerator {

public:
  cLightFieldDepthGenerator() : _depthImage(NULL) {};
  ~cLightFieldDepthGenerator() {};

  virtual lfError generate(void* raw_image, lfDepthGeneratorParameter_t *params);
  virtual void* getDepthImage() const {return _depthImage;}
protected:
  void* _depthImage;
};
#endif // LIGHT_FIELD_DEPTH_GENERATOR_H