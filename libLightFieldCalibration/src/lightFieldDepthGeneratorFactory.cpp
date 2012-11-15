/**
 * \file   lightFieldDepthGenerator.cpp
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#include "lightFieldDepthGeneratorFactory.h"
#include "lightFieldDepthGeneratorPlane.h"
#include "lightFieldDepthGeneratorFile.h"

cLightFieldDepthGenerator* cLightFieldDepthGeneratorFactory::create(lfDepthGeneratorType_t type)
{
  switch(type) {
  case LF_DEPTH_GENERATOR_PLANE:
    return new cLightFieldDepthGeneratorPlane();
  case LF_DEPTH_GENERATOR_FILE:
    return new cLightFieldDepthGeneratorFile();
  case LF_DEPTH_GENERATOR_UNKNOWN:
  default:
    return NULL;
  }
}