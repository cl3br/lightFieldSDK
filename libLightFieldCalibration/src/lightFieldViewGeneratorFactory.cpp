/**
 * \file   lightFieldViewGeneratorFactory.cpp
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#include "lightFieldViewGeneratorFactory.h"
#include "lightFieldViewGenerator_AllInFocus.h"

cLightFieldViewGenerator* cLightFieldViewGeneratorFactory::create(lfViewGeneratorType_t type)
{
  switch(type) {
  case LF_VIEW_GENERATOR_RAYTRIX:
    return new cLightFieldViewGenerator_AllInFocus();
  case LF_VIEW_GENERATOR_UNKNOWN:
  default:
    return NULL;
  }
}