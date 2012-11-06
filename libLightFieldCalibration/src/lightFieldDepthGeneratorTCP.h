/**
 * \file   lightFieldDepthGeneratorTCP.h
 * \author Clemens Brummer
 * \date   03.11.2012
 */

#ifndef LIGHT_FIELD_DEPTH_GENERATOR_TCP_H
#define LIGHT_FIELD_DEPTH_GENERATOR_TCP_H

#include "lightFieldDepthGenerator.h"

class cLightFieldDepthGeneratorTCP : public cLightFieldDepthGenerator {

public:
  cLightFieldDepthGeneratorTCP() {};
  ~cLightFieldDepthGeneratorTCP() {};

  virtual lfError generate(void* raw_image, lfCalibrationParameter_t* params, void* depthmap);
};
#endif // LIGHT_FIELD_DEPTH_GENERATOR_H