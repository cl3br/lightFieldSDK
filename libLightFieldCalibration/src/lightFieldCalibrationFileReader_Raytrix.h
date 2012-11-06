/**
 * \file   lightFieldCalibrationFileReader_Raytrix.h
 * \author Clemens Brummer
 * \date   21.10.2012
 */

#ifndef LIGHT_FIELD_CALIBRATION_FILE_READER_RAYTRIX_H
#define LIGHT_FIELD_CALIBRATION_FILE_READER_RAYTRIX_H

#include "lightFieldCalibrationFileReader.h"

class cLightFieldCalibrationFileReaderRaytrix : public cLightFieldCalibrationFileReader
{
public:
  lfError read(const char *file_name, lfCalibrationParameter_t* params);
};

#endif // LIGHT_FIELD_CALIBRATION_FILE_READER_RAYTRIX_H
