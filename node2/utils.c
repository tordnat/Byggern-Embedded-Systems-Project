#include "utils.h"
int32_t map(int32_t val_to_map, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
  return (val_to_map - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}