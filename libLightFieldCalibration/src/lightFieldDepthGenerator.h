/**
 * \file   lightFieldDepthGenerator.h
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#ifndef LIGHT_FIELD_DEPTH_GENERATOR_H
#define LIGHT_FIELD_DEPTH_GENERATOR_H

#include "lfError.h"
#include "lightFieldCalibrationParameter.h"

class cLightFieldDepthGenerator {

public:
  cLightFieldDepthGenerator() {};
  ~cLightFieldDepthGenerator() {};

  virtual lfError generate(void* raw_image, lfCalibrationParameter_t* params, void* depthmap);
};
#endif // LIGHT_FIELD_DEPTH_GENERATOR_H