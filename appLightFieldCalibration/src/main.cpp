#include <cstdio>
#include <iostream>

//#include "ConfigFile.h"

#include "lightFieldCore.h"
#include "main.h"

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
    CHECK(lf->loadRawImage(argv[1]), "Unable to load the raw light field image.\n");
    CHECK(lf->loadWhiteImage(argv[3]), "Unable to load the white filtered light field image.\n");
    CHECK(lf->loadCalibration(argv[2], LF_CALIBRATION_FILE_RAYTRIX), "Unable to load the config file.\n");
    CHECK(lf->showDepthImage(LF_DEPTH_GENERATOR_TCP), "unable to generate the depth image");
    const char* new_name = createSaveName(argv[1], "Circles");
    CHECK(lf->saveCalibImage(new_name), "unable to generate calibration image!\n");
    CHECK(lf->showRenderedView(LF_VIEW_GENERATOR_RAYTRIX, 1.0),"Unable to render view!\n");


    // puts an grid on the image for debug
    //CHECK(lf->showCalibImage(), "Unable to load light field raw image.\n");

    //CHECK(lf->saveRectifiedImage(argv[3]),"");
    
    //CHECK(lf->saveMaskImage(argv[3]), "Unable to save the mask image\n");
    //CHECK(lf->saveMaskedImage(argv[3]), "Unable to save the masked input image\n");

    //CHECK(lf->saveCenterImage(argv[3]),"Computing the center image failed.\n");
    
    //CHECK(lf->extractLensImages(),"Computing the lens images failed.\n");
    
  }

  return 0;
}

char* createSaveName(const char* base_name, const char* add_on)
{
  const char* pPoint = strrchr(base_name, '.');
  size_t base_len = pPoint - base_name;
  size_t ext_len = strlen(base_name) - base_len;
  size_t add_len = strlen(add_on);
  char* name = new char[base_len + add_len + ext_len];
  strncpy(name, base_name, base_len);
  name[base_len] = '\0';
  strcat(name, add_on);
  strcat(name, base_name + base_len);
  return name;
}
