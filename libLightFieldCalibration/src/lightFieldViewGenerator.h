/**
 * \file   lightFieldViewGenerator.h
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#ifndef LIGHT_FIELD_VIEW_GENERATOR_H
#define LIGHT_FIELD_VIEW_GENERATOR_H

#include "lfError.h"
#include "lightFieldCalibrationParameter.h"

class cLightFieldViewGenerator {

public:
  cLightFieldViewGenerator() {};
  ~cLightFieldViewGenerator() {};

  virtual lfError generate(void* raw_image, void* depth_image, lfCalibrationParameter_t params);
  virtual void* getView() const {return _view;}
protected:
  void *_view;
};
#endif // LIGHT_FIELD_VIEW_GENERATOR_H