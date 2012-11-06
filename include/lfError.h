/**
 * \file   lfError.h
 * \author Clemens Brummer
 * \date   03.11.2012
 */

#ifndef LIGHT_FIELD_ERROR_H
#define LIGHT_FIELD_ERROR_H

typedef enum {
  LF_NO_ERR = 0,
  LF_ERR
}lfError;

#define RETURN_NO_ERR return LF_NO_ERR

#define RETURN_IF_FAILED(f) {lfError err; if(LF_NO_ERR != (err = f) ) { printf("The function %s returned an error!\n", #f); return err;}}
#define RETURN_IF_NULL(var) if(var == NULL) { printf("pointer is NULL\n"); return LF_ERR; }

#endif // LIGHT_FIELD_ERROR_H
