/**
 * \file   lfTypes.h
 * \author Clemens Brummer
 * \date   02.11.2012
 */

#ifndef LIGHT_FIELD_TYPES_H
#define LIGHT_FIELD_TYPES_H

/**
 * Point type.
 */
typedef struct {
  double x;
  double y;
} lfPoint2Dd_t;

typedef struct {
  int x;
  int y;
} lfPoint2Di_t;

#ifdef __cplusplus

inline lfPoint2Dd_t lfPoint2D(double x, double y) {
  lfPoint2Dd_t p;
  p.x = x; p.y = y;
  return p;
}

inline lfPoint2Di_t lfPoint2Di(int x, int y) {
  lfPoint2Di_t p;
  p.x = x; p.y = y;
  return p;
}

inline lfPoint2Dd_t operator+(lfPoint2Dd_t p1, lfPoint2Dd_t p2) {
  lfPoint2Dd_t p;
  p.x = p1.x + p2.x;
  p.y = p1.y + p2.y;
  return p;
}

inline lfPoint2Di_t operator+(lfPoint2Di_t p1, lfPoint2Di_t p2) {
  lfPoint2Di_t p;
  p.x = p1.x + p2.x;
  p.y = p1.y + p2.y;
  return p;
}

inline lfPoint2Dd_t operator-(lfPoint2Dd_t p1, lfPoint2Dd_t p2) {
  lfPoint2Dd_t p;
  p.x = p1.x - p2.x;
  p.y = p1.y - p2.y;
  return p;
}

inline lfPoint2Di_t operator-(lfPoint2Di_t p1, lfPoint2Di_t p2) {
  lfPoint2Di_t p;
  p.x = p1.x - p2.x;
  p.y = p1.y - p2.y;
  return p;
}

inline void operator+=(lfPoint2Dd_t &p1, lfPoint2Dd_t p2) {
  p1.x = p1.x + p2.x;
  p1.y = p1.y + p2.y;
}

inline void operator+=(lfPoint2Di_t &p1, lfPoint2Di_t p2) {
  p1.x = p1.x + p2.x;
  p1.y = p1.y + p2.y;
}

inline lfPoint2Dd_t operator*(lfPoint2Dd_t p1, lfPoint2Dd_t p2) {
  lfPoint2Dd_t p;
  p.x = p1.x * p2.x;
  p.y = p1.y * p2.y;
  return p;
}

inline lfPoint2Dd_t operator*(double f, lfPoint2Dd_t p) {
  lfPoint2Dd_t po;
  po.x = f * p.x;
  po.y = f * p.y;
  return po;
}
#endif // __cplusplus

typedef double lfTransMat2D_t[9];

typedef enum {
  LF_CALIBRATION_FILE_RAYTRIX,
  LF_CALIBRATION_FILE_LYTRO,
  LF_CALIBRATION_FILE_UNKNOWN
} lfCalibrationFileType_t;

typedef enum {
  LF_DEPTH_GENERATOR_PLANE,
  LF_DEPTH_GENERATOR_FILE,
  LF_DEPTH_GENERATOR_UNKNOWN
} lfDepthGeneratorType_t;

typedef enum {
  LF_VIEW_GENERATOR_RAYTRIX,
  LF_VIEW_GENERATOR_SPARSE,
  LF_VIEW_GENERATOR_UNKNOWN
} lfViewGeneratorType_t;

#endif // LIGHT_FIELD_TYPES_H