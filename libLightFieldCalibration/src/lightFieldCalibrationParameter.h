/**
 * \file   lfCalibrationParameter.h
 * \author Clemens Brummer
 * \date   21.10.2012
 */

#ifndef LF_CALIBRATION_PARAMETER_H
#define LF_CALIBRATION_PARAMETER_H

#include <vector>
#include "lfTypes.h"
#include "lfError.h"

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
  lfPoint2Dd_t offset;
  double depth_range_min;
  double depth_range_max;
} lfLens_t;

/**
 * Light field calibration parameter.
 * Stores all data neccessary for calibrating the micro-lenses.
 */
typedef struct lfCalibrationParameter {
  lfError findCloseLensCenters (lfPoint2Dd_t image_point, const int neighbors, const double depth, unsigned int *const  num_lenses, lfPoint2Dd_t* lens_centers_out);
  lfError fromCalibrationCoordToLens(lfPoint2Dd_t p_in, lfPoint2Di_t *p_out);
  lfError fromImageCoordToCalibrationCoord(lfPoint2Dd_t p_in, lfPoint2Dd_t *p_out);
  lfError fromImageCoordToLens(lfPoint2Dd_t p_in, lfPoint2Di_t *p_out);
  void* createLensMask();

  lfPoint2Dd_t offset;                ///< offset of the first lens type [pixel] in img coords
  double scale_x;                     ///< x scale factor
  double scale_y;                     ///< y scale factor
  double rotation;                    ///< rotation between image sensor and microlense array [rad]
  double diameter;                    ///< diameter of the microlenses [pixel]
  double lens_border;                 ///< [pixel]
  double tcp;                         ///< total covering plane [virtual depth]
  std::vector<lfLens_t> lens_types;   ///< array of lens types
  int lens_type_nums;                 ///< number of lens types
  lfPoint2Dd_t step_x;                ///< horizontal lens step [lens]
  lfPoint2Dd_t step_y;                ///< vertical lens step [lens]
  lfPoint2Dd_t grid_step;             ///< translation vector from grid (0,0) to grid (1,1) [lens]
  lfPoint2Dd_t sub_grid_step;         ///< translation vector to the sub pattern with in one grid [lens]
  int sub_grid_nums;                  ///< number of patterns wihtin one grid  
  int numX;                           ///> number of lens patterns in x direction
  int numY;                           ///> number of lens patterns in y direction
  int numLenses;                      ///> number of lenses from one type
  lfPoint2Di_t grid_min;              ///> defines the smallest point, which is visible in the image [lens] in lens coords
  lfPoint2Di_t grid_max;              ///> defines the greatest point, which is visible in the image [lens] in lens coords
  lfTransMat2D_t transMat;            ///> transforms from image to lens coordinate system
  //eLenseArrangement_t lenseOrder;
  std::vector<std::vector<lfPoint2Dd_t>> lens_centers; ///< all lens centers
} lfCalibrationParameter_t;

#endif //LIGHT_FIELD_CALIBRATION_H