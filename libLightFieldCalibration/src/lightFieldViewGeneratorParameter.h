/**
 * \file   lightFieldViewGeneratorParameter.h
 * \author Clemens Brummer
 * \date   20.11.2012
 */

#ifndef LF_VIEW_GENERATOR_PARAMETER_H
#define LF_VIEW_GENERATOR_PARAMETER_H

#include "lfTypes.h"

/**
 * Light field calibration parameter.
 * Stores all data neccessary for calibrating the micro-lenses.
 */
typedef struct lfViewGeneratorParameter {
  double scale;
} lfViewGeneratorParameter_t;

#endif //LF_VIEW_GENERATOR_PARAMETER_H