/**
 * \file   lightFieldCalibReader.cpp
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#include "lightFieldCalibrationFileReaderFactory.h"
#include "lightFieldCalibrationFileReader_Raytrix.h"
#include "lightFieldCalibrationFileReader_Lytro.h"

cLightFieldCalibrationFileReader*
cLightFieldCalibrationFileReaderFactory::create(lfCalibrationFileType_t type)
{
  switch(type) {
  case LF_CALIBRATION_FILE_RAYTRIX:
    return new cLightFieldCalibrationFileReaderRaytrix();
  case LF_CALIBRATION_FILE_LYTRO:
    return new cLightFieldCalibrationFileReaderLytro();
  case LF_CALIBRATION_FILE_UNKNOWN:
  default:
    return NULL;
  }
}