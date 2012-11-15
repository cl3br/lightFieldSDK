/**
 * \file   lightFieldDepthGeneratorPlane.h
 * \author Clemens Brummer
 * \date   03.11.2012
 */

#ifndef LIGHT_FIELD_DEPTH_GENERATOR_FILE_H
#define LIGHT_FIELD_DEPTH_GENERATOR_FILE_H

#include "lightFieldDepthGenerator.h"

class cLightFieldDepthGeneratorFile : public cLightFieldDepthGenerator {

public:
  //cLightFieldDepthGeneratorFile() {};
  //~cLightFieldDepthGeneratorFile() {};

  virtual lfError generate(void* raw_image, lfDepthGeneratorParameter_t *params);
};
#endif // LIGHT_FIELD_DEPTH_GENERATOR_FILE_H