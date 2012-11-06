/**
 * \file   lightFieldCalibrationFileReaderFactory.h
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#ifndef LIGHT_FIELD_CALIBRATION_FILE_READER_FACTORY_H
#define LIGHT_FIELD_CALIBRATION_FILE_READER_FACTORY_H

#include "lfTypes.h"
#include "lightFieldCalibrationFileReader.h"
 
class cLightFieldCalibrationFileReaderFactory {
public:
  virtual cLightFieldCalibrationFileReader* create(lfCalibrationFileType_t type);
};

#endif // LIGHT_FIELD_CALIBRATION_READER_H