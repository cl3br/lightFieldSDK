/**
 * \file   lfCalibrationParameter.h
 * \author Clemens Brummer
 * \date   21.10.2012
 */

#ifndef LF_CALIBRATION_PARAMETER_H
#define LF_CALIBRATION_PARAMETER_H

#include <vector>
#include "lfTypes.h"

/// Specifies the arrangement of the micro lenses.
typedef enum {
  LENSE_ARR_RECT,        ///< lenses are rectangular arranged
  LENSE_ARR_HEXA         ///< lenses are hexagonal arranged
} lfLenseArrangement_t;

/**
 * Lense type.
 * Stores lens specific data.
 */
typedef struct {
  lfPoint2D_t offset;
  double depth_range_min;
  double depth_range_max;
} lfLens_t;

/**
 * Light field calibration parameter.
 * Stores all data neccessary for calibrating the micro-lenses.
 */
typedef struct lfCalibrationParameter {
  lfPoint2D_t offset;             ///< offset of the first lens type [pixel] in img coords
  double scale_x;                 ///< x scale factor
  double scale_y;                 ///< y scale factor
  double rotation;                ///< rotation between image sensor and microlense array [rad]
  double diameter;                ///< radius of the microlenses [pixel]
  double lens_border;             ///< [pixel]
  double tcp;                     ///< total covering plane [virtual depth]
  std::vector<lfLens_t> lenses;   ///< array of lens types
  int lensTypes;                  ///< number of lens types
  lfPoint2D_t step_x;             ///< horizontal lens step [lens]
  lfPoint2D_t step_y;             ///< vertical lens step [lens]
  lfPoint2D_t grid_step;          ///< translation vector from grid (0,0) to grid (1,1) [lens]
  lfPoint2D_t sub_grid_step;      ///< translation vector to the sub pattern with in one grid [lens]
  int sub_grid_nums;              ///< number of patterns wihtin one grid
  //eLenseArrangement_t lenseOrder;
  std::vector<std::vector<lfPoint2D_t>> lens_centers; ///< all lens centers
} lfCalibrationParameter_t;

#endif //LIGHT_FIELD_CALIBRATION_H