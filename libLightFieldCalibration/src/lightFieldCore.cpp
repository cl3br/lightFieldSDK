/**
 * \file   lightFieldCore.cpp
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#if defined(_WIN32)
  #include <direct.h>
  #define MKDIR(s) _mkdir(s);
#elif defined(__linux__)
  #include <sys/types.h> 
  #include <sys/stat.h> 
  #define MKDIR(s) mkdir(s, 777);
#else
 #error No plattform specific mkdir specified!
#endif

#include "cv.h"
#include "highgui.h"
#include "opencvHelpers.h"
#include "lightFieldUtils.h"
#include "lightFieldCore.h"
#include "lightFieldCorePrivates.h"


cLightFieldCore::cLightFieldCore()
{
  p = new corePrivs();
}

cLightFieldCore::~cLightFieldCore()
{
  if(p) {
    p->cleanUp();
    delete p;
  }
}

lfError cLightFieldCore::loadCalibration(const char* file_name, lfCalibrationFileType_t type)
{
  RETURN_IF_FAILED(p->_calib->loadCalibration(file_name, type));
  RETURN_NO_ERR;
}

void* cLightFieldCore::loadImage(const char* file_name)
{
  if(!file_name)
    return NULL;

  IplImage *temp = NULL;
  temp = cvLoadImage(file_name);
  if(!temp) return NULL;

  void *img = cvCreateImage(cvSize(temp->width,temp->height), IPL_DEPTH_32F, temp->nChannels);
  cvConvert(temp, img);

  return img;
}

lfError cLightFieldCore::loadRawImage(const char* file_name)
{
  p->deleteRawImage();
  p->_raw_img = (IplImage*) loadImage(file_name);

  if(p->_raw_img) {
    p->_img = p->_raw_img;
    if(p->_white_img) {
      RETURN_IF_FAILED(filterImage());
      p->_img = p->_proc_img;
    }
  }
  
  p->isImageNew = true;
  RETURN_NO_ERR;
}

lfError cLightFieldCore::loadWhiteImage(const char* file_name)
{
  p->deleteWhiteImage();
  p->_white_img = (IplImage*) loadImage(file_name);
  if(p->_raw_img && p->_white_img)
    RETURN_IF_FAILED(filterImage());

  RETURN_NO_ERR;
}
lfError cLightFieldCore::loadProcImage(const char* file_name)
{
  p->deleteProcImage();
  p->_proc_img = (IplImage*) loadImage(file_name);
  RETURN_IF_NULL(p->_proc_img);
  p->_img = p->_proc_img;

  RETURN_NO_ERR;
}

lfError  cLightFieldCore::filterImage()
{
  RETURN_IF_NULL(p->_white_img && p->_raw_img);
  p->deleteProcImage();
  p->_proc_img = CREATE_IMAGE(p->_raw_img);
  RETURN_IF_NULL(p->_proc_img);

  cvDiv(p->_raw_img, p->_white_img, p->_proc_img);
  cvScale(p->_proc_img, p->_proc_img, 255);
  RETURN_NO_ERR;
}

lfError cLightFieldCore::usePlaneDepthImage(double plane)
{
  lfDepthGeneratorParameter params;
  params.plane = plane;
  RETURN_IF_FAILED(createDepthImage(LF_DEPTH_GENERATOR_PLANE, &params));
  showImage((IplImage*) p->_depth_generator->getDepthImage(), "Depth Image");
  RETURN_NO_ERR;
}

lfError cLightFieldCore::loadDepthImage(const char* file_name)
{
  lfDepthGeneratorParameter params;
  params.file_name = file_name;
  RETURN_IF_FAILED(createDepthImage(LF_DEPTH_GENERATOR_FILE, &params));
  showImage((IplImage*) p->_depth_generator->getDepthImage(), "Depth Image");
  RETURN_NO_ERR;
}

lfError cLightFieldCore::createDepthImage(lfDepthGeneratorType_t type, lfDepthGeneratorParameter* params)
{  
  RETURN_IF_NULL(p->_img);

  p->_depth_generator = p->_depth_generator_factory->create(type);
  RETURN_IF_NULL(p->_depth_generator);
  RETURN_IF_FAILED(p->_depth_generator->generate(p->_img, params));
  RETURN_NO_ERR;
}

/////////////////////////// VIEW HANDLING //////////////////////

lfError cLightFieldCore::showRenderedView(lfViewGeneratorType_t type, double virtual_depth)
{
  RETURN_IF_FAILED(renderView(type));
  showImage((IplImage*) p->_view_generator->getView(), "Rendered Image View");
  RETURN_NO_ERR;
}

lfError cLightFieldCore::saveRenderedView(const char* file_name, lfViewGeneratorType_t type, double virtual_depth)
{
  RETURN_IF_FAILED(renderView(type));
  RETURN_IF_FAILED(saveImage(file_name, p->_view_generator->getView()));
  RETURN_NO_ERR;
}

lfError cLightFieldCore::renderView(lfViewGeneratorType_t type)
{
  RETURN_IF_NULL(p->_img);

  RETURN_IF_NULL(p->_depth_generator);
  RETURN_IF_NULL(p->_depth_generator->getDepthImage())
    //RETURN_IF_FAILED(p->_depth_generator->generate(p->_img, &p->_calib->getParameters(p->_img)));
  
  LF_DELETE(p->_view_generator);
  if(p->_view_generator == NULL)
    p->_view_generator = p->_view_generator_factory->create(type);  
  RETURN_IF_NULL(p->_view_generator);
  if(NULL == p->_view_generator->getView())
    RETURN_IF_FAILED(p->_view_generator->generate(p->_img, p->_depth_generator->getDepthImage(), &p->_calib->getParameters(p->_img)));
  RETURN_NO_ERR;
}

/////////////////////////// CIRCLE IMAGE HANDLING /////////////////////////////

lfError cLightFieldCore::showCalibImage()
{
  IplImage* img = (IplImage*) createCalibImage();
  RETURN_IF_NULL(img);
  showImage(img, "Calibration Image");
  cvReleaseImage(&img);
  RETURN_NO_ERR;
}

lfError cLightFieldCore::saveCalibImage(const char* file_name)
{
  IplImage* img = (IplImage*) createCalibImage();
  RETURN_IF_NULL(img);
  RETURN_IF_FAILED(saveImage(file_name, img));
  RETURN_NO_ERR;
}

void* cLightFieldCore::createCalibImage()
{
  if(!p->_img)
    return NULL;
  void* img = cvCloneImage(p->_img);
  if(!img)
    return NULL;
  if(LF_NO_ERR != p->_calib->drawLenses(img, 1, p->_calib->getParameters(p->_img).lens_type_nums))
    return NULL;

  return img;
}



lfError cLightFieldCore::saveImage(const char* file_name, void* img)
{
  int ret = cvSaveImage(file_name, img);
  cvReleaseImage((IplImage**)&img);

  if(ret == 0)
    return LF_ERR;
  else
    RETURN_NO_ERR;
}

////////////////////// LENS IMAGE HANDLING /////////////////////////
#define EXTRACT_FROM_DEPTH

lfError cLightFieldCore::saveLensImages(const char* folder_name)
{
  RETURN_IF_NULL(folder_name);
#ifdef EXTRACT_FROM_DEPTH
  IplImage* depth = (IplImage*) p->_depth_generator->getDepthImage();
  IplImage* depth_converted = cvCreateImage(cvSize(depth->width, depth->height), IPL_DEPTH_16U, 1);
  CvScalar vd, vd_new;
  for(int y=0; y<depth->height; y++) {
    for(int x=0; x<depth->width; x++) {
      vd = cvGet2D(depth, y, x);
      vd_new.val[0] = (1 + 1/vd.val[0]) * 65535;
      cvSet2D(depth_converted, y, x, vd_new);
    }
  }
  showImage(depth_converted, "Depth converted");
  RETURN_IF_FAILED(extractLensImages(depth_converted));
  const char* type = "depth";
#else
  const char* type = "img";
  RETURN_IF_FAILED(extractLensImages(p->_img));
#endif

  int lens_num = 1;
  int lens_type_num = 1;
  char my_folder_name[260];
  char dir_path[260];
  char file_path[260];
  const size_t folder_name_size = sizeof(my_folder_name);

  //add last file seperator
  strncpy(my_folder_name, folder_name, folder_name_size);
  my_folder_name[folder_name_size - 1] = '\0';
  const size_t folder_name_length = strlen(my_folder_name);
  if(my_folder_name[folder_name_length-1] != '/' && my_folder_name[folder_name_length-1] != '\\' && folder_name_length < folder_name_size) {
    my_folder_name[folder_name_length] = '/';
    my_folder_name[folder_name_length + 1] = '\0';
  }

  // save all images (example format: lenses1/img_30.png)
  for(lfImg_t::iterator l = p->_lensImages.begin(); l != p->_lensImages.end(); l++)
  {    
    for(vector<IplImage*>::iterator it = (*l).begin(); it != (*l).end(); it++)
    {
      sprintf(dir_path, "%slenses%d", my_folder_name ,lens_type_num);
      MKDIR(dir_path);
      sprintf(file_path, "%s/%s_%d.png", dir_path, type, lens_num);
      RETURN_IF_FAILED(saveImage(file_path, (*it)));
      lens_num++;
    }
    lens_type_num++;
    lens_num=1;
  }
  RETURN_NO_ERR;
}


lfError cLightFieldCore::extractLensImages(void *img)
{
  RETURN_IF_NULL(p->_img && img);
  lfCalibrationParameter params = p->_calib->getParameters(p->_img);

  IplImage *mask = (IplImage*) params.createLensMask();
  RETURN_IF_NULL(mask);
  showImage(mask,"Mask");

  p->deleteLensImages();

  // create new lens images
  IplImage* lens_img = NULL;
  p->_lensImages.reserve(params.lens_type_nums);
  for(int l = 0; l < params.lens_type_nums; l++) {
    vector<IplImage*> v(params.numLenses);
    p->_lensImages.push_back(v);
    for(int i = 0; i < params.numLenses; i++) {
      lens_img = cvCreateImage(cvSize(mask->width, mask->height),((IplImage*) img)->depth, ((IplImage*) img)->nChannels);
      if(!lens_img) {
        printf("Not enought memory to create all lens images!\n");
        p->deleteLensImages();
        return LF_ERR;
      }
      p->_lensImages[l][i] = lens_img;
    }
  }

  for (int l=0; l < params.lens_type_nums; l++) {
    for (int i=0; i < params.numLenses; i++) {
      getLensImage(i, l, &params, img, mask, p->_lensImages[l][i]);
    }
  }

  //for (int i = 500; i<620; i+=2) 
  //  showImage(p->_lensImages[1][i], "lens image");

  RETURN_NO_ERR;
}

lfError cLightFieldCore::getLensImage(int lens_num, int lens_type, const void* calib_params, void* full_img, void* mask_img, void* lens_img)
{
  RETURN_IF_NULL(full_img && mask_img && calib_params);

  //cast of the void pointers
  IplImage* mask = (IplImage*) mask_img;
  IplImage* full = (IplImage*) full_img;
  IplImage* img  = (IplImage*) lens_img;
  lfCalibrationParameter_t *params = (lfCalibrationParameter_t*) calib_params;
  
  if (params->lens_centers.empty())
    return LF_ERR;

  lfPoint2Dd_t lc;
  CvRect roi;
  const int shift_x = cvRound(mask->width/2);
  const int shift_y = cvRound(mask->height/2);

  lc = params->lens_centers[lens_type][lens_num];
  roi.x = (int) lc.x - shift_x;
  roi.y = (int) lc.y - shift_y;
  roi.height = mask->height;
  roi.width = mask->width;
  cvZero(img);

  if(roi.x < 0 || roi.y < 0 || roi.x + roi.width >= full->width || roi.y + roi.height >= full->height)
    return LF_ERR;

  cvSetImageROI(full, roi);
  cvCopy(full, img, mask);

  RETURN_NO_ERR;
}
