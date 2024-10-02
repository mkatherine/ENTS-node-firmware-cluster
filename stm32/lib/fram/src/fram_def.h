#ifndef LIB_FRAM_SRC_FRAM_DEF_H_
#define LIB_FRAM_SRC_FRAM_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "fram.h"

typedef FramStatus (*FramWritePtrType)(FramAddr addr, const uint8_t *data,
                                       size_t len);

typedef FramStatus (*FramReadPtrType)(FramAddr addr, size_t len, uint8_t *data);

typedef struct {
  /** Pointer to write function */
  FramWritePtrType WritePtr;
  /** Pointer to read function */
  FramReadPtrType ReadPtr;
  /** Size of FRAM */
  FramAddr size;
  /** Number of pages */
  unsigned int pages;
  /** Size of segments */
  unsigned int seg_size;
} FramInterfaceType;

#ifdef __cplusplus
}
#endif

#endif  // LIB_FRAM_SRC_FRAM_DEF_H_