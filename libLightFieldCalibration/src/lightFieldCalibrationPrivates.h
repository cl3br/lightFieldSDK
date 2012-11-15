/**
 * \file   lightFieldCalibrationPrivates.h
 * \author Clemens Brummer
 * \date   03.11.2012
 */


#ifndef LIGHT_FIELD_CALIBRATION_PRIVS_H
#define LIGHT_FIELD_CALIBRATION_PRIVS_H

#include "cv.h"
#include "lfTypes.h"
#include "lightFieldCalibrationFileReaderFactory.h"

struct calibPrivs
{
  calibPrivs();
  ~calibPrivs();

  void cleanUp();
  void deleteImage();
  void deleteIMask();
  void deleteLMask();
  void deleteLensCenters();


  // struct members
  IplImage* img;                    ///> raw light field image
  IplImage* imask;                  ///> image mask to determine lens pixels (1 - valid, 0 - invalid)
  IplImage* lmask;                  ///> lens mask to extract a lens image
  lfCalibrationParameter_t params;  ///> stores all calibration parameter \sa lfCalibrationParameter_t
  cLightFieldCalibrationFileReaderFactory* reader_factory;
  bool isCalibNew;
};

#endif // LIGHT_FIELD_CALIBRATION_PRIVS_H