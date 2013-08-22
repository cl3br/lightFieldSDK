/**
 * \file   lightFieldCalibrationFileReader_Lytro.h
 * \author Clemens Brummer
 * \date   14.07.2012
 */

#ifndef LIGHT_FIELD_CALIBRATION_FILE_READER_LYTRO_H
#define LIGHT_FIELD_CALIBRATION_FILE_READER_LYTRO_H

#include "lightFieldCalibrationFileReader.h"

class cLightFieldCalibrationFileReaderLytro : public cLightFieldCalibrationFileReader
{
public:
  lfError read(const char *file_name, lfCalibrationParameter_t* params);
};

#endif // LIGHT_FIELD_CALIBRATION_FILE_READER_RAYTRIX_H
