#include "float_comparison.h"

bool is_float_leq(float a, float b) {
  return (a < b) || are_floats_eq(a, b);
}

bool is_float_geq(float a, float b) {
  return (a > b) || are_floats_eq(a, b);
}

bool are_floats_eq(float a, float b) {

  const float epsilon = 0.5;

  if(a < b)
    return ((b-a) < epsilon);

  return ((a-b) < epsilon);
}