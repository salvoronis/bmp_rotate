#ifndef ROTATION_H
#define ROTATION_H
#include <stdint.h>
struct image rotate_angle(struct image * img, double angle);
uint32_t rotated_height(struct image * origin, double angle);
uint32_t rotated_width(struct image * origin, double angle);
#endif
