#include "Matrix.h"

#ifndef ACTIVATION_H
#define ACTIVATION_H

// Insert Activation class here...
typedef Matrix (*Activation_func)(const Matrix& m);

namespace activation
{
  extern Activation_func relu;
  extern Activation_func softmax;
}

#endif //ACTIVATION_H