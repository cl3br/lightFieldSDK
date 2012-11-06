/**
 * \file   lightFieldViewGeneratorFactory.h
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#ifndef LIGHT_FIELD_VIEW_GENERATOR_FACTORY_H
#define LIGHT_FIELD_VIEW_GENERATOR_FACTORY_H

#include "lfTypes.h"
#include "lightFieldViewGenerator.h"

class cLightFieldViewGeneratorFactory {

public:
  cLightFieldViewGeneratorFactory() {};
  ~cLightFieldViewGeneratorFactory() {};

  virtual cLightFieldViewGenerator* create(lfViewGeneratorType_t type);
};

#endif // LIGHT_FIELD_VIEW_GENERATOR_FACTORY_H
