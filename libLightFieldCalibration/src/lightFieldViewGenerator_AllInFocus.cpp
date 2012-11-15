/**
 * \file   lightFieldViewGenerator.cpp
 * \author Clemens Brummer
 * \date   01.11.2012
 */
#include <vector>

#include "cv.h"
#include "opencvHelpers.h"
#include "lightFieldViewGenerator_AllInFocus.h"

lfError cLightFieldViewGenerator_AllInFocus::generate(void *raw_image, void *depth_image, lfCalibrationParameter_t* params)
{
  IplImage* img = static_cast<IplImage*> (raw_image);
  IplImage* depthImg = static_cast<IplImage*> (depth_image);
  IplImage* view = CREATE_IMAGE(img);
  _lens_mask = params->createLensMask();  
  IplImage* mask = (IplImage*) _lens_mask;
  RETURN_IF_NULL(img && view && depthImg);

  cvZero(view);

  unsigned int numLenses; // number of lenses in the neighborhood
  unsigned int numUsedLenses = 0; // number of lenses used for the projection
  int xLens, yLens; // projected point in lens coordinates
  double depth; // virtual depth at current image point
  lfPoint2Dd_t lens_centers[100]; // storage for the lens centers of the neighboring lenses
  lfPoint2Dd_t image_point; // projected point in image coordinates
  lfPoint2Dd_t lens_center; // current used lens center
  CvScalar s = cvScalar(0); // color value of the final pixel
  const int r = (int) (params->diameter / 2 - params->lens_border); // radius with integer resolution

  //showImage(mask,"Lens mask");

  for (int y=0; y < img->height; y++) {
    for (int x=0; x < img->width; x++) {
      depth = cvGetReal2D(depthImg, y, x);
      numLenses = sizeof(lens_centers) / sizeof(*lens_centers);
      params->findCloseLensCenters(lfPoint2D(x, y), 2, depth, &numLenses, lens_centers);

        //IplImage *temp_img = cvCloneImage(img);

        //for(unsigned int l = 0; l < numLenses; l++) {
        //  lens_center = lens_centers[l];
        //  cvCircle(temp_img, cvPoint((int) lens_center.x, (int) lens_center.y), r, gColors[0], 1);
        //}
        //cvCircle(temp_img, cvPoint(x,y), 5, gColors[1], 2);
        //showImage(temp_img, "Debug Image");
        //cvReleaseImage(&temp_img);
      
      for(unsigned int l = 0; l < numLenses; l++) {
        lens_center = lens_centers[l];
        image_point = lens_center - (1/depth * (lfPoint2D(x,y) - lens_center));
        // only if projected image point is at a valid lense position
        xLens = (int) (mask->width / 2 - lens_center.x + image_point.x);
        yLens = (int) (mask->height / 2 - lens_center.y + image_point.y);
        if(IS_POINT_IN_IMAGE(mask, xLens, yLens)) {
          if(cvGetReal2D(mask,yLens,xLens)) {
            if(IS_POINT_IN_IMAGE(img, (int) image_point.x, (int) image_point.y)) {
              s = s + cvGet2D(img, (int) image_point.y, (int) image_point.x);
              numUsedLenses++;
            }
          }
        }
      }
      if(numUsedLenses == 0)
        continue;
      // compute mean value over all found pixels
      s =  (1/(double)numUsedLenses) * s;
      cvSet2D(view, y, x, s);
      // reset variables
      numUsedLenses = 0;
      s = cvScalar(0);
    }
  }

  _view = view;

  RETURN_NO_ERR;
}