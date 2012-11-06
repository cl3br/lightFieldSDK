/**
 * \file   lightFieldDepthGenerator.cpp
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#include "lightFieldDepthGeneratorFactory.h"
#include "lightFieldDepthGeneratorTCP.h"

cLightFieldDepthGenerator* cLightFieldDepthGeneratorFactory::create(lfDepthGeneratorType_t type)
{
  switch(type) {
  case LF_DEPTH_GENERATOR_TCP:
    return new cLightFieldDepthGeneratorTCP();
  case LF_DEPTH_GENERATOR_UNKNOWN:
  default:
    return NULL;
  }
}