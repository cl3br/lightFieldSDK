#include <cstdio>
#include <iostream>

//#include "ConfigFile.h"

#include "lightFieldCore.h"

#define USAGE printf("Usage: %s <path/to/image.jpg> <path/to/calib_file.xml>",argv[0])
#define CHECK(func,err_txt) {                         \
                              lfError err = LF_NO_ERR;            \
                              if(LF_NO_ERR != (err = func)) { \
                                printf(err_txt);      \
    char c; \
    std::cin >> c; \
                                return err;           \
                              }                       \
                            }

int main(int argc, char **argv)
{
  if (argc != 4) {
    USAGE;
    return 1;
  }

  cLightFieldCore* lf = new cLightFieldCore();

  if(lf)
  {
    // set image with calibration file
    CHECK(lf->loadImage(argv[1]), "Unable to load the raw light field image.\n");
    CHECK(lf->loadCalibration(argv[2], LF_CALIBRATION_FILE_RAYTRIX), "Unable to load the config file.\n");

    // puts an grid on the image for debug
    //CHECK(lf->showCalibImage(), "Unable to load light field raw image.\n");

    //CHECK(lf->saveRectifiedImage(argv[3]),"");
    
    //CHECK(lf->saveCalibImage(argv[3]), "Unable to save the calibration image.\n");
    //CHECK(lf->saveMaskImage(argv[3]), "Unable to save the mask image\n");
    //CHECK(lf->saveMaskedImage(argv[3]), "Unable to save the masked input image\n");

    //CHECK(lf->saveCenterImage(argv[3]),"Computing the center image failed.\n");
    
    //CHECK(lf->extractLensImages(),"Computing the lens images failed.\n");
    
  }

  return 0;
}


