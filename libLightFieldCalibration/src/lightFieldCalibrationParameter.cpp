/**
 * \file   lfCalibrationParameter.cpp
 * \author Clemens Brummer
 * \date   08.11.2012
 */

#include <assert.h>
#include "cv.h"
#include "opencvHelpers.h"
#include "lightFieldUtils.h"
#include "lightFieldCalibrationParameter.h"

#define LENS_MASK_HEXAGON
//#define LENS_MASK_CIRCLE
//#define LENS_MASK_SQUARE

using namespace std;

lfError lfCalibrationParameter::findCloseLensCenters(lfPoint2Dd_t image_point, const int neighbors, const double depth, unsigned int *const  num_lenses, lfPoint2Dd_t* lens_centers_out)
{
  size_t len=0;
  lfPoint2Di_t corresponding_lens;
  fromImageCoordToLens(image_point, &corresponding_lens);

  lfPoint2Di_t first_lens = lfPoint2Di(max(corresponding_lens.x-neighbors, grid_min.x), max(corresponding_lens.y-neighbors, grid_min.y));
  lfPoint2Di_t last_lens = lfPoint2Di(min(corresponding_lens.x+neighbors, grid_max.x), min(corresponding_lens.y+neighbors, grid_max.y));
  int row_begin, cols;
  const double abs_depth = abs(depth);

  for(int y=first_lens.y; y <= last_lens.y; y++) {
    // compute lens number
    row_begin = (y - grid_min.y) * numX * sub_grid_nums;
    for(int x=first_lens.x; x <= last_lens.x; x++) {
      cols = (x - grid_min.x) * sub_grid_nums;
      for(int l=0; l<this->lens_type_nums; l++) {
        // only if lens is appropriate for the specified depth
        if(abs_depth >= lens_types[l].depth_range_min && abs_depth <= lens_types[l].depth_range_max) {
          assert(numLenses > row_begin + cols);
          lens_centers_out[len] = lens_centers[l][row_begin + cols];
          len++;
          // also add the sub grid lens
          lens_centers_out[len] = lens_centers[l][row_begin + cols + 1];
          len++;
        }
      }
    }
  }

  // find close lenses
  //double diff;
  //for(int l=0; l<this->lens_type_nums; l++) {
  //  for(int i=0; i<this->numLenses; i++) {
  //    diff = abs(image_point - lens_centers[l][i]);
  //    if(region_size > diff) {
  //      if(len>=*num_lenses)
  //        return LF_ERR;
  //      lens_centers_out[len] = lens_centers[l][i];
  //      len++;
  //    }
  //  }
  //}
  *num_lenses = len;
  RETURN_NO_ERR;
}

lfError lfCalibrationParameter::fromCalibrationCoordToLens(lfPoint2Dd_t p_in, lfPoint2Di_t *p_out)
{
  if(this->diameter == 0)
    return LF_ERR;

  p_out->x = (int) floor (1/(this->diameter * this->grid_step.x) * p_in.x);
  p_out->y = (int) floor (1/(this->diameter * this->grid_step.y) * p_in.y);
  RETURN_NO_ERR;
}

lfError lfCalibrationParameter::fromImageCoordToCalibrationCoord(lfPoint2Dd_t p_in, lfPoint2Dd_t *p_out)
{
  RETURN_IF_NULL(p_out);
  *p_out = lfTransformPoint2D_inv(transMat, p_in);
  RETURN_NO_ERR;
}

lfError lfCalibrationParameter::fromImageCoordToLens(lfPoint2Dd_t p_in, lfPoint2Di_t *p_out)
{
  lfPoint2Dd_t p_temp;
  RETURN_IF_FAILED(fromImageCoordToCalibrationCoord(p_in, &p_temp));
  RETURN_IF_FAILED(fromCalibrationCoordToLens(p_temp, p_out));
  RETURN_NO_ERR;
}

void* lfCalibrationParameter::createLensMask()
{
  const int d = cvRound(diameter);
  const int mask_size = (int) (d * 1.42);
  IplImage* lens_mask = cvCreateImage(cvSize(mask_size, mask_size), IPL_DEPTH_8U, 1);
  if(lens_mask == NULL)
    return NULL;

  cvZero(lens_mask);

#ifdef LENS_MASK_HEXAGON
  const int npts = 6;
  CvPoint pts[npts];
  const int x0 = (int) ((mask_size - d) * 0.5);
  const int x1 = (int) (mask_size * 0.5) - 1;
  const int x2 = (int) ((mask_size + d) * 0.5 - 1);
  const int y0 = x0;
  const int y3 = x2;
  const int y1 = (int) (y0 + d * 0.25);
  const int y2 = (int) (y0 + d * 0.75);

  pts[0].x = x1; pts[0].y = y0;
  pts[1].x = x2; pts[1].y = y1;
  pts[2].x = x2; pts[2].y = y2;
  pts[3].x = x1; pts[3].y = y3;
  pts[4].x = x0; pts[4].y = y2;
  pts[5].x = x0; pts[5].y = y1;

  // TODO: rotate points

  cvFillConvexPoly(lens_mask, pts, 6, cvScalar(MY_CV_WHITE));
#elif defined LENS_MASK_CIRCLE
  cvCircle(lens_mask, cvPoint(lens_mask->width/2, lens_mask->height/2), (int) diameter/2, cvScalar(MY_CV_WHITE), -1);
#elif defined LENS_MASK_SQUARE
  int r = (int) diameter/2;
  cvRectangle(lens_mask, cvPoint(lens_mask->width/2-r, lens_mask->height/2-r), cvPoint(lens_mask->width/2+r, lens_mask->height/2+r), cvScalar(MY_CV_WHITE),-1);
#else
  cvSet(lens_mask, cvScalar(MY_CV_WHITE));
#endif

  return lens_mask;
}