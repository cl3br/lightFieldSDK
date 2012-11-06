/**
 * \file   lightFieldCalibrationPrivates.cpp
 * \author Clemens Brummer
 * \date   03.11.2012
 */

#include "lightFieldCalibrationPrivates.h"

calibPrivs::calibPrivs() {
  img      = NULL;
  imask    = NULL;
  lmask    = NULL;
  transMat = NULL;

  reader_factory = new cLightFieldCalibrationFileReaderFactory();

  params.offset.x = 58.0;
  params.offset.y = 24.0;
  params.rotation = -0.0065;
  params.diameter = 28.0;
  params.scale_x = 1.0015;
  params.scale_y = 0.996;
  params.sub_grid_nums = 1;

  numX = 58;
  numY = 133;
  //lenseArrStyle = LENSE_ARR_HEXA;

  isCalibNew = false;
}

calibPrivs::~calibPrivs() {
  this->cleanUp();
}

void calibPrivs::cleanUp() {
  deleteImage();
  deleteIMask();
  deleteLMask();
  deleteLensImages();
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
  for(vector<CvMat*>::iterator it = lensCenters.begin(); it != lensCenters.end(); it++)
  {
    if (*it)
      cvReleaseMat(&(*it)); *it = NULL;
  }
  lensCenters.clear();
}

void calibPrivs::deleteLensImages() {
  for(lfImg_t::iterator l = lensImages.begin(); l != lensImages.end(); l++)
  {    
    for(vector<IplImage*>::iterator it = (*l).begin(); it != (*l).end(); it++)
    {
      if (*it)
        cvReleaseImage(&(*it)); *it = NULL;
    }
    (*l).clear();
  }
  lensImages.clear();
}