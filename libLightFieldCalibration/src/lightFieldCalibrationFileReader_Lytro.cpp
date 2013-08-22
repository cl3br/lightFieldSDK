/**
 * \file   lightFieldCalibrationFileReader_Raytrix.cpp
 * \author Clemens Brummer
 * \date   21.10.2012
 */

#include <cstdio>
#include <assert.h>
#include <malloc.h>
#include <math.h>

#include "rapidjson/document.h"
#include "lightFieldCalibrationFileReader_Lytro.h"

using namespace rapidjson;

lfError cLightFieldCalibrationFileReaderLytro::read(const char *file_name, lfCalibrationParameter_t* params)
{
  int w, h;
  Document document;

  // Datei einlesen
  FILE *f = fopen(file_name,"r");
  fseek(f, 0, SEEK_END);
  long file_size = ftell(f);
  rewind(f);
  file_size++;
  char *file_buffer = (char*) malloc(file_size * (sizeof(char)));
  fread(file_buffer, sizeof(char), file_size, f);
  fclose(f);
  file_buffer[file_size-1] = '\0';

  document.Parse<0>(file_buffer);
  if(document.HasParseError())
    return LF_ERR;

  //const Value& img = document["image"];

  //w = img["width"].GetInt();
  //h = img["height"].GetInt();

  const Value& devices = document["devices"];
  const Value& mla = devices["mla"];

  double pixelPitch = devices["sensor"]["pixelPitch"].GetDouble();

  params->diameter = mla["lensPitch"].GetDouble() / pixelPitch;
  params->rotation = mla["rotation"].GetDouble();

  const Value& scale = mla["scaleFactor"];
  params->scale_x = scale["x"].GetDouble();
  params->scale_y = scale["y"].GetDouble();

  const Value& offset = mla["sensorOffset"];
  params->offset.x = offset["x"].GetDouble() / pixelPitch;
  params->offset.y = offset["y"].GetDouble() / pixelPitch;

  params->lens_border = 0.0;

  lfLens_t l;
  l.offset = lfPoint2D(0.0, 0.0);
  l.depth_range_min = 0.0;
  l.depth_range_max = 100.0;

  params->lens_types.clear();
  params->lens_types.push_back(l);

  params->lens_type_nums = params->lens_types.size();

  params->sub_grid_nums = 2;
  params->grid_step = lfPoint2D(1.0, sin(60.0) * 2);
  params->sub_grid_step.x = params->grid_step.x / 2;
  params->sub_grid_step.y = params->grid_step.y / 2;

  params->tcp = 1,0;
  
  free(file_buffer);

  RETURN_NO_ERR;
}
