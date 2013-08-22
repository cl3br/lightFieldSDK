#include <cstdio>
#include <iostream>

//#include "ConfigFile.h"

#include "lightFieldCore.h"
#include "main.h"

#define USAGE {printf("Usage: %s <path/to/image.png> <path/to/calib_file.xml> [Options]\n\n" \
                     "Options:\n" \
                     "\t-w    \t<path/to/white_image.png>\n" \
                     "\t-m    \t<path/to/16_bit_depth_map.png>\n" \
                     "\t-c    \t(saves a calibration debug image with circles indicating the lenses)\n"\
                     "\t-v    \t(save a rendered view)\n" \
                     "\t-i/-d \t<path/to/output/> (stores all lens images / depth maps into the specified directory)\n" \
                     "\t-p    \t<value> (use a planar depth image)\n" \
                     ,argv[0]); return 1;}

#define ONLY_ONE_DEPTH {printf("Use only one depth image generator!\n"); USAGE}

#define CHECK(func,err_txt) {                                 \
                              lfError err = LF_NO_ERR;        \
                              if(LF_NO_ERR != (err = func)) { \
                                printf(err_txt);              \
                                return err;                   \
                              }                               \
                            }

#define MAX_FILE_NAME_LENGTH 260

int main(int argc, char **argv)
{
  bool has_white_image            = false;
  bool has_depth_image            = false;
  bool save_circle_image          = false;
  bool save_view_image            = false;
  bool store_lens_images          = false;
  bool store_lens_images_as_depth = false;
  bool use_plane                  = false;

  double plane = 0;
  char white_image[MAX_FILE_NAME_LENGTH];
  char depth_image[MAX_FILE_NAME_LENGTH];
  char lens_image_folder[MAX_FILE_NAME_LENGTH];

  // minimum params requiered (see usage)
  if (argc < 2) {
    USAGE;
  }

  // parse parameters
  for(int i=3; i < argc; i++)
  {
    if(argv[i][1] == 'w') {
      i++;
      if(i>argc) {
        USAGE;
      }
      strncpy(white_image, argv[i], sizeof(white_image)); white_image[sizeof(white_image) - 1] = '\0';
      has_white_image = true;
    } else if(argv[i][1] == 'm') {
      if(use_plane)
        ONLY_ONE_DEPTH;
      i++;
      if(i>argc){
        USAGE;
      }
      strncpy(depth_image, argv[i], sizeof(depth_image)); white_image[sizeof(depth_image) - 1] = '\0';
      has_depth_image = true;
    } else if(argv[i][1] == 'c') {
      save_circle_image=true;
    } else if(argv[i][1] == 'v') {
      save_view_image=true;
    } else if(argv[i][1] == 'p') {
      if(has_depth_image)
        ONLY_ONE_DEPTH;
      i++;
      if(i>argc){
        USAGE;
      }
      plane = strtod(argv[i], NULL);
      use_plane=true;
    } else if(argv[i][1] == 'i' || argv[i][1] == 'd') {
      store_lens_images_as_depth = (argv[i][1] == 'd');
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
      const char* proc_name = createSaveName(argv[1], "MyProc");
      CHECK(lf->loadWhiteImage(white_image, proc_name), "Unable to load the white filtered light field image.\n");
    } else {
      CHECK(lf->loadProcImage(argv[1]), "Unable to load the processed light field image.\n");
    }

    CHECK(lf->loadCalibration(argv[2], LF_CALIBRATION_FILE_LYTRO), "Unable to load the config file.\n");
    if(has_depth_image) {
      CHECK(lf->loadDepthImage(depth_image), "unable to generate the depth image");
    } else if (use_plane) {
      CHECK(lf->usePlaneDepthImage(plane), "unable to generate the depth image");
    }
    
    if(save_circle_image) {
      const char* name = createSaveName(argv[1], "Circles");
      CHECK(lf->saveCalibImage(name), "unable to generate calibration image!\n");
    }

    if(store_lens_images) {
      CHECK(lf->saveLensImages(lens_image_folder, store_lens_images_as_depth), "unable to generate calibration image!\n");
    }

    if(save_view_image && (has_depth_image || use_plane)) {
      const char* new_name = createSaveName(argv[1], "View");
      //CHECK(lf->showRenderedView(LF_VIEW_GENERATOR_RAYTRIX, 1.0),"Unable to render view!\n");
      CHECK(lf->saveRenderedView(new_name,LF_VIEW_GENERATOR_RAYTRIX, 1.0),"Unable to render view!\n");
    }


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
