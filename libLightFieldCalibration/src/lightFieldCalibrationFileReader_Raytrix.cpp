/**
 * \file   lightFieldCalibrationFileReader_Raytrix.cpp
 * \author Clemens Brummer
 * \date   21.10.2012
 */

#include <cstdio>
#include <assert.h>

#include "tinyxml2.h"
#include "lightFieldCalibrationFileReader_Raytrix.h"

#define XML2DOUBLE(node, name, var) {const char* val = node->FirstChildElement(name)->FirstChild()->Value(); sscanf(val, "%lf%", &var); }
#define XML2POINT(node, node_name, point) { XMLElement* n = node->FirstChildElement(node_name); XML2DOUBLE(n, "x", point.x) XML2DOUBLE(n, "y", point.y) }

using namespace tinyxml2;

lfError cLightFieldCalibrationFileReaderRaytrix::read(const char *file_name, lfCalibrationParameter_t* params)
{
  XMLDocument *xml = new XMLDocument();
  xml->LoadFile(file_name);

  XMLElement *root = xml->RootElement();
  assert(strcmp(root->Name(), "RayCalibData") == 0);

  XML2DOUBLE(root, "diameter", params->diameter);
  XML2DOUBLE(root, "rotation", params->rotation);
  XML2DOUBLE(root, "lens_border", params->lens_border);
  XML2DOUBLE(root, "tcp", params->tcp);
    
  XML2POINT(root, "offset", params->offset);
  params->offset.y *= -1;

  XML2POINT(root, "lens_base_x", params->step_x);
  XML2POINT(root, "lens_base_y", params->step_y);
  XML2POINT(root, "sub_grid_base", params->grid_step);

  // raytrix patterns have only to patterns in one grid
  params->sub_grid_step.x = params->grid_step.x / 2;
  params->sub_grid_step.y = params->grid_step.y / 2;

  params->scale_x = 1.0;
  params->scale_y = -1.0;
  params->sub_grid_nums = 2;

  params->lens_types.clear();

  XMLElement* lens_type = root->FirstChildElement("lens_type");
  XMLElement* depth_range = NULL;
  int i=0;
  for(; lens_type != NULL; i++, lens_type = lens_type->NextSiblingElement("lens_type") )
  {
    int id = lens_type->IntAttribute("id");
    assert(id == i);

    lfLens_t l;
    XML2POINT(lens_type, "offset", l.offset);

    depth_range = lens_type->FirstChildElement("depth_range");
    XML2DOUBLE(depth_range, "min", l.depth_range_min);
    XML2DOUBLE(depth_range, "max", l.depth_range_max);

    params->lens_types.push_back(l);
  }
  
  params->lens_type_nums = params->lens_types.size();

  delete xml;
  RETURN_NO_ERR;
}
