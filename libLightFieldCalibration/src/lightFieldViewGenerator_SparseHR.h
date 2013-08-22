/**
 * \file   lightFieldViewGenerator_SparseHR.h
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#ifndef LIGHT_FIELD_VIEW_GENERATOR_SPARSEHR_H
#define LIGHT_FIELD_VIEW_GENERATOR_SPARSEHR_H

#include "lightFieldViewGenerator.h"

class cLightFieldViewGenerator_SparseHR : public cLightFieldViewGenerator {

public:
  cLightFieldViewGenerator_SparseHR() : cLightFieldViewGenerator(), _lens_mask(NULL) {}
  virtual ~cLightFieldViewGenerator_SparseHR() {}

  virtual lfError generate(void *raw_image, void *depth_image, lfCalibrationParameter_t* cparams, lfViewGeneratorParameter_t* vparams);

private:
  void* _lens_mask;
};
#endif // LIGHT_FIELD_VIEW_GENERATOR_ALLINFOCUS_H