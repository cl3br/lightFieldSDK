/**
 * \file   lightFieldDepthGenerator.cpp
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#include "lightFieldDepthGenerator.h"

lfError cLightFieldDepthGenerator::generate(void* raw_image, lfCalibrationParameter_t* params, void* depthmap)
{
  // default parameter
  params->diameter = 10;
  params->grid_step = lfPoint2D(1.0,1.0);
  lfLens_t lens;
  lens.depth_range_min = 1;
  lens.depth_range_max = 3;
  lens.offset = lfPoint2D(0,0);
  params->lenses.clear();
  params->lenses.push_back(lens);
  params->lensTypes = 1;
  params->lens_border = 0;
  params->offset = lfPoint2D(0,0);
  params->rotation = 0;
  params->scale_x = 1.0;
  params->scale_y = 1.0;
  params->step_x = lfPoint2D(1.0,0);
  params->step_y = lfPoint2D(0,1.0);
  params->sub_grid_nums = 1;
  params->tcp = 1.0;
  RETURN_NO_ERR;
}