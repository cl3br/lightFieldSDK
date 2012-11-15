/**
 * \file   lightFieldCalibrationPrivates.cpp
 * \author Clemens Brummer
 * \date   03.11.2012
 */
#include <vector>
#include "lightFieldCalibrationPrivates.h"

using namespace std;

calibPrivs::calibPrivs() {
  img      = NULL;
  imask    = NULL;
  lmask    = NULL;

  reader_factory = new cLightFieldCalibrationFileReaderFactory();

  //params.offset.x = 58.0;
  //params.offset.y = 24.0;
  //params.rotation = -0.0065;
  //params.diameter = 28.0;
  //params.scale_x = 1.0015;
  //params.scale_y = 0.996;
  //params.sub_grid_nums = 1;
  //params.tcp = 2.0;
  //params.numX = 58;
  //params.numY = 133;
  
  // default parameter
  params.diameter = 10;
  params.offset = lfPoint2D(0,0);
  params.rotation = 0;
  params.tcp = 1.0;
  lfLens_t lens;
  lens.depth_range_min = 1;
  lens.depth_range_max = 3;
  lens.offset = lfPoint2D(0,0);
  params.lens_types.clear();
  params.lens_types.push_back(lens);
  params.lens_type_nums = 1;
  params.lens_border = 0;
  params.grid_step = lfPoint2D(1.0,1.0);
  params.scale_x = 1.0;
  params.scale_y = 1.0;
  params.step_x = lfPoint2D(1.0,0);
  params.step_y = lfPoint2D(0,1.0);
  params.sub_grid_step = lfPoint2D(1.0,1.0);
  params.sub_grid_nums = 1;
  params.numLenses = 100;
  params.numX = 10;
  params.numY = 10;
  params.grid_min = lfPoint2Di(-4,-4);
  params.grid_max = lfPoint2Di(5,5);
  memset(&params.transMat,0,sizeof(lfTransMat2D_t));
  params.transMat[0]=1.0;
  params.transMat[4]=1.0;
  params.transMat[8]=1.0;

  isCalibNew = true;
}

calibPrivs::~calibPrivs() {
  this->cleanUp();
}

void calibPrivs::cleanUp() {
  deleteImage();
  deleteIMask();
  deleteLMask();
  deleteLensCenters();
  if(reader_factory) {
    delete reader_factory;
    reader_factory = NULL;
  }
}

void calibPrivs::deleteImage() {
  if (img) {
    cvReleaseImage(&img);
    img = NULL;
  }
}

void calibPrivs::deleteIMask() {
  if (imask) {
    cvReleaseImage(&imask);
    imask = NULL;
  }
}

void calibPrivs::deleteLMask() {
  if (lmask) {
    cvReleaseImage(&lmask);
    lmask = NULL;
  }
}

void calibPrivs::deleteLensCenters() {    
  for(vector<vector<lfPoint2Dd_t>>::iterator it = params.lens_centers.begin(); it != params.lens_centers.end(); it++)
  {
    (*it).clear();
  }
  params.lens_centers.clear();
}