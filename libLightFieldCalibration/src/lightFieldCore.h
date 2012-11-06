/**
 * \file   lightFieldCore.h
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#ifndef LIGHT_FIELD_CORE_H
#define LIGHT_FIELD_CORE_H

#include "lfTypes.h"
#include "lfError.h"

struct corePrivs;

/**
 * Light Field Core.
 * This class handles ... TODO
 * 
 * 
 */
class cLightFieldCore
{
public:
  cLightFieldCore();
  ~cLightFieldCore();  

  /**
   * Loads a raw light field image from disc.
   * \param file_name Path of the image file
   * \return 0 on success and nonzero on failure
   */
  lfError loadImage(const char* img_file_name);
  /**
   * Loads a calibration file from disc.
   * \param file_name Path of the calibration file
   * \return 0 on success and nonzero on failure
   */
  lfError loadCalibration(const char* calib_file_name, lfCalibrationFileType_t type);

private:
  corePrivs* p; 
};
#endif // LIGHT_FIELD_CORE_H
