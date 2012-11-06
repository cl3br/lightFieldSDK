/**
 * \file   lightFieldViewGeneratorFactory.cpp
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#include "lightFieldViewGeneratorFactory.h"
//#include "lightFieldViewGeneratorRaytrix.h"

cLightFieldViewGenerator* cLightFieldViewGeneratorFactory::create(lfViewGeneratorType_t type)
{
  switch(type) {
  case LF_VIEW_GENERATOR_RAYTRIX:
    return NULL; //TODO: new cLightFieldDepthGenerator();
  case LF_VIEW_GENERATOR_UNKNOWN:
  default:
    return NULL;
  }
}