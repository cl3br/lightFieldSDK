/**
 * \file   lightFieldCalibrationPrivates.h
 * \author Clemens Brummer
 * \date   03.11.2012
 */


#ifndef LIGHT_FIELD_CALIBRATION_PRIVS_H
#define LIGHT_FIELD_CALIBRATION_PRIVS_H

#include "cv.h"
#include <vector>
#include "lightFieldCalibrationFileReaderFactory.h"

//      lens_type < number < lens_img > > >
typedef vector< vector< IplImage* > > lfImg_t;

struct calibPrivs
{
  calibPrivs();
  ~calibPrivs();

  void cleanUp();
  void deleteImage();
  void deleteIMask();
  void deleteLMask();
  void deleteLensImages();
  void deleteLensCenters();


  // struct members
  IplImage* img;                    ///> raw light field image
  IplImage* imask;                  ///> image mask to determine lens pixels (1 - valid, 0 - invalid)
  IplImage* lmask;                  ///> lens mask to extract a lens image
  vector<CvMat*> lensCenters;       ///> stores all the lens centers (channel 1: x, channel 2: y) [pixels] in img coords
  lfImg_t   lensImages;             ///> all lense images
  CvMat*    transMat;               ///> transforms from image to lens coordinate system
  lfCalibrationParameter_t params;  ///> stores all calibration parameter \sa lfCalibrationParameter_t
  cLightFieldCalibrationFileReaderFactory* reader_factory;
  
  CvPoint grid_min;                 ///> defines the smallest point, which is visible in the image [lens] in lens coords
  CvPoint grid_max;                 ///> defines the greatest point, which is visible in the image [lens] in lens coords

  int numX;                         ///> number of lens patterns in x direction
  int numY;                         ///> number of lens patterns in y direction
  int numLenses;                    ///> number of lenses from one type
  //unsigned int lenseArrStyle;
  //unsigned int imgRectStyle;
  bool isCalibNew;
};

#endif // LIGHT_FIELD_CALIBRATION_PRIVS_H