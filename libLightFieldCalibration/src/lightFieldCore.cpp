/**
 * \file   lightFieldCore.cpp
 * \author Clemens Brummer
 * \date   01.11.2012
 */

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
  showImage(p->_proc_img, "processed Image");
  RETURN_NO_ERR;
}

lfError cLightFieldCore::showDepthImage(lfDepthGeneratorType_t type)
{
  RETURN_IF_FAILED(createDepthImage(type));
  showImage((IplImage*) p->_depth_generator->getDepthImage(), "Depth Image");
  RETURN_NO_ERR;
}

lfError cLightFieldCore::showRenderedView(lfViewGeneratorType_t type, double virtual_depth)
{
  RETURN_IF_FAILED(renderView(type));
  showImage((IplImage*) p->_view_generator->getView(), "Rendered Image View");
  RETURN_NO_ERR;
}


lfError cLightFieldCore::createDepthImage(lfDepthGeneratorType_t type)
{  
  RETURN_IF_NULL(p->_img);

  p->_depth_generator = p->_depth_generator_factory->create(type);
  RETURN_IF_NULL(p->_depth_generator);
  RETURN_IF_FAILED(p->_depth_generator->generate(p->_img, p->_calib->getParameters()));
  RETURN_NO_ERR;
}

lfError cLightFieldCore::renderView(lfViewGeneratorType_t type)
{
  RETURN_IF_NULL(p->_img);

  LF_DELETE(p->_view_generator);

  p->_view_generator = p->_view_generator_factory->create(type);
  RETURN_IF_NULL(p->_depth_generator);
  RETURN_IF_FAILED(p->_view_generator->generate(p->_img, p->_depth_generator->getDepthImage(), p->_calib->getParameters()));
  RETURN_NO_ERR;
}

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
  int ret = cvSaveImage(file_name, img);
  cvReleaseImage(&img);

  if(ret == 0)
    return LF_ERR;
  else
    RETURN_NO_ERR;
}

void* cLightFieldCore::createCalibImage()
{
  if(!p->_img)
    return NULL;
  void* img = cvCloneImage(p->_img);
  if(!img)
    return NULL;
  if(LF_NO_ERR != p->_calib->drawLenses(img, 1, p->_calib->getParameters().lensTypes))
    return NULL;

  return img;
}
