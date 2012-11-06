/**
 * \file   lightFieldCore.cpp
 * \author Clemens Brummer
 * \date   01.11.2012
 */

#include "cv.h"
#include "highgui.h"
#include "opencvHelpers.h"
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
  if(p->_raw_img && p->_white_img)
    RETURN_IF_FAILED(filterImage());
  
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
  RETURN_NO_ERR;
}

lfError  cLightFieldCore::filterImage()
{
  RETURN_IF_NULL(p->_white_img && p->_raw_img);
  p->deleteProcImage();
  p->_proc_img = CREATE_IMAGE(p->_raw_img);
  RETURN_IF_NULL(p->_proc_img);

  cvDiv(p->_raw_img, p->_white_img, p->_proc_img);
  showImage(p->_proc_img, "processed Image");
  RETURN_NO_ERR;
}

// TODO: generate processed image (e.g. rectified white filtered)
// TODO: light field type?
// TODO: 