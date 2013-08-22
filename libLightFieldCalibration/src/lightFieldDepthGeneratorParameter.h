/**
 * \file   lightFieldDepthParameter.h
 * \author Clemens Brummer
 * \date   13.11.2012
 */

#ifndef LF_DEPTH_GENERATOR_PARAMETER_H
#define LF_DEPTH_GENERATOR_PARAMETER_H

#include "lfTypes.h"

/**
 * Light field calibration parameter.
 * Stores all data neccessary for calibrating the micro-lenses.
 */
typedef struct lfDepthGeneratorParameter {
  double plane; ///> plane of the depth plane [virtual depth]
  const char* file_name;
} lfDepthGeneratorParameter_t;

#endif //LF_DEPTH_GENERATOR_PARAMETER_H