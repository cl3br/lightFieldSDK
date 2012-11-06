/**
 * \file   lightFieldCalibrationFileReader.h
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#ifndef LIGHT_FIELD_CALIBRATION_FILE_READER_H
#define LIGHT_FIELD_CALIBRATION_FILE_READER_H

#include "lfError.h"
#include "lightFieldCalibrationParameter.h"
 
class cLightFieldCalibrationFileReader {
public:
  virtual lfError read(const char *file_name, lfCalibrationParameter_t* params) {return LF_ERR;};
};

#endif // LIGHT_FIELD_CALIBRATION_READER_H