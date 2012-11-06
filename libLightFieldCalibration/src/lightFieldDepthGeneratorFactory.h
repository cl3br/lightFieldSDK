/**
 * \file   lightFieldDepthGenerator.h
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#ifndef LIGHT_FIELD_DEPTH_GENERATOR_FACTORY_H
#define LIGHT_FIELD_DEPTH_GENERATOR_FACTORY_H

#include "lfTypes.h"
#include "lightFieldDepthGenerator.h"

class cLightFieldDepthGeneratorFactory {

public:
  cLightFieldDepthGeneratorFactory() {};
  ~cLightFieldDepthGeneratorFactory() {};

  virtual cLightFieldDepthGenerator* create(lfDepthGeneratorType_t type);
};

#endif // LIGHT_FIELD_DEPTH_GENERATOR_FACTORY_H