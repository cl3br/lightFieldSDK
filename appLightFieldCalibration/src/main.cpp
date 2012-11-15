#include <cstdio>
#include <iostream>

//#include "ConfigFile.h"

#include "lightFieldCore.h"
#include "main.h"

#define USAGE printf("Usage: %s <path/to/image.png> <path/to/calib_file.xml> [Options]\n\n" \
                     "Options:\n" \
                     "\t-w <path/to/white_image.png>" \
                     "\t-d <path/to/depth_image.png>" \
                     "\t-c (saves a calibration debug image with circles indicating the lenses)"\
                     "\t-i <path/to/output/> (stores all lens images in the specified directory)" \
                     ,argv[0]); return 1;

#define CHECK(func,err_txt) {                         \
                              lfError err = LF_NO_ERR;            \
                              if(LF_NO_ERR != (err = func)) { \
                                printf(err_txt);      \
    char c; \
    std::cin >> c; \
                                return err;           \
                              }                       \
                            }

#define MAX_FILE_NAME_LENGTH 260

int main(int argc, char **argv)
{
  bool has_white_image = false;
  bool has_depth_image = false;
  bool save_circle_image = false;
  bool store_lens_images = false;
  char white_image[MAX_FILE_NAME_LENGTH];
  char depth_image[MAX_FILE_NAME_LENGTH];
  char lens_image_folder[MAX_FILE_NAME_LENGTH];
  if (argc < 2) {
    USAGE;
  }

  for(int i=3; i < argc; i++)
  {
    if(argv[i][1] == 'w') {
      i++;
      if(i>argc) {
        USAGE;
      }
      strncpy(white_image, argv[i], sizeof(white_image)); white_image[sizeof(white_image) - 1] = '\0';
      has_white_image = true;
    } else if(argv[i][1] == 'd') {
      i++;
      if(i>argc){
        USAGE;
      }
      strncpy(depth_image, argv[i], sizeof(depth_image)); white_image[sizeof(depth_image) - 1] = '\0';
      has_depth_image = true;
    } else if(argv[i][1] == 'c') {
      save_circle_image=true;
    } else if(argv[i][1] == 'i') {
      i++;
      if(i>argc){
        USAGE;
      }
      strncpy(lens_image_folder, argv[i], sizeof(lens_image_folder)); white_image[sizeof(lens_image_folder) - 1] = '\0';
      store_lens_images = true;
    } else {
      USAGE;
    }
  }

  cLightFieldCore* lf = new cLightFieldCore();

  if(lf)
  {
    // set image
    if(has_white_image) {
      CHECK(lf->loadRawImage(argv[1]), "Unable to load the raw light field image.\n");
      CHECK(lf->loadWhiteImage(white_image), "Unable to load the white filtered light field image.\n");
    } else {
      CHECK(lf->loadProcImage(argv[1]), "Unable to load the processed light field image.\n");
    }
    CHECK(lf->loadCalibration(argv[2], LF_CALIBRATION_FILE_RAYTRIX), "Unable to load the config file.\n");

    if(has_depth_image) {
      CHECK(lf->loadDepthImage(depth_image), "unable to generate the depth image");
    } else {
      CHECK(lf->usePlaneDepthImage(-3.0), "unable to generate the depth image");
    }

    if(save_circle_image) {
      const char* name = createSaveName(argv[1], "Circles");
      CHECK(lf->saveCalibImage(name), "unable to generate calibration image!\n");
    }

    if(store_lens_images) {
      CHECK(lf->saveLensImages(lens_image_folder), "unable to generate calibration image!\n");
    }

    const char* new_name = createSaveName(argv[1], "View");
    //CHECK(lf->showRenderedView(LF_VIEW_GENERATOR_RAYTRIX, 1.0),"Unable to render view!\n");
    CHECK(lf->saveRenderedView(new_name,LF_VIEW_GENERATOR_RAYTRIX, 1.0),"Unable to render view!\n");


    //old functions
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
