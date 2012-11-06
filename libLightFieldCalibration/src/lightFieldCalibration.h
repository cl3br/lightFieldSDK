/**
 * \file   lightFieldCalibration.h
 * \author Clemens Brummer
 * \date   18.10.2012
 */

#ifndef LIGHT_FIELD_CALIBRATION_H
#define LIGHT_FIELD_CALIBRATION_H

#include "lfError.h"

struct calibPrivs;

/**
 * Light Field Calibration.
 * This class handles calibration tasks. Following features are available:
 * -# Calculates the center of every micro lens in pixel.
 * -# Shows/ saves an image for visualization.
 */
class cLightFieldCalibration
{
public:
  cLightFieldCalibration();
  ~cLightFieldCalibration();
  /**
   * Loads a calibration file from disc.
   * \param file_name Path of the calibration file
   * \return 0 on success and nonzero on failure
   */
  lfError loadCalibration(const char* file_name, lfCalibrationFileType_t type);
  /**
   * Shows the input image with little circles to vizualize the micro lenses.
   * \return 0 on success and nonzero on failure
   */
  lfError showCalibImage();
  /**
   * Saves the calibration image to disc.
   * \param file_name Path to store the image on disc. Format will be recognized automatically.
   * \return 0 on success and nonzero on failure
   */
  lfError saveCalibImage(const char* file_name);
  /**
   * Shows the rectified image.
   * \return 0 on success and nonzero on failure
   */
  lfError showRectifiedImage();
  /**
   * Saves the rectified image to disc.
   * \param file_name Path to store the image on disc. Format will be recognized automatically.
   * \return 0 on success and nonzero on failure
   */
  lfError saveRectifiedImage(const char* file_name);

  lfError saveMaskImage(const char* file_name);
  
  lfError saveMaskedImage(const char* file_name);

  lfError saveCenterImage(const char* file_name);
  lfError showCenterImage();
  lfError extractLensImages();

  //enum eImageRectification {
  //  RECTIFY_AFFINE=2,        ///> use affine transformation for image rectification
  //  RECTIFY_PERSPECTIVE    ///> use affine transformation for image rectification
  //};

private:
  lfError computeLenseCenters();  // computes all lens centers and store them into a matrix
  lfError computeTransMat();      // compute the transformation matrix according to eImageRectification
  lfError computeImageSpecificParams(); // should be called after image and calibration files are loaded
  lfError computeCenterImage(void* img);
  lfError drawLenses(void* img, int line_width, int colors);  // draws lines / circles as lens visualization
  lfError rectify(void* img);
  lfError createImageMask();       // creates a mask containing the valid lens pixels
  lfError createLensMask();       // creates a mask containing the valid lens pixels
  lfError getLensImage(int lens_type, int lens_num, void* lens_img, bool useMask=true);

  calibPrivs* p;              // struct containing all private variables
};

#endif //LIGHT_FIELD_CALIBRATION_H