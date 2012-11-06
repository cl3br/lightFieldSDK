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
   * \return LF_NO_ERR on success and LF_ERR on failure
   */
  lfError loadRawImage(const char* file_name);
  /**
   * Loads an light field image taken with a white filter from disc.
   * \param file_name Path of the image file
   * \return LF_NO_ERR on success and LF_ERR on failure
   */
  lfError loadWhiteImage(const char* file_name);
  /**
   * Loads an processed light field image from disc.
   * \param file_name Path of the image file
   * \return LF_NO_ERR on success and LF_ERR on failure
   */
  lfError loadProcImage(const char* file_name);
  /**
   * Loads a calibration file from disc.
   * \param file_name Path of the calibration file
   * \return LF_NO_ERR on success and LF_ERR on failure
   */
  lfError loadCalibration(const char* file_name, lfCalibrationFileType_t type);

private:
  void* loadImage(const char* file_name);
  lfError  filterImage();

  corePrivs* p; 
};
#endif // LIGHT_FIELD_CORE_H
