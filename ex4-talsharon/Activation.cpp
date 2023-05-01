#include "Activation.h"
#include <cmath>

Matrix relu_func (const Matrix& m)
{
  Matrix ret(m);
  int n = m.get_rows()*m.get_cols();
  for (int k = 0; k < n; k++)
  {
    if (m[k] < 0)
    {
      ret[k] = 0;
    }
  }
  return ret;
}

Matrix softmax_func (const Matrix& m)
{
  Matrix ret(m);
  int n = m.get_rows()*m.get_cols();
  float sum = 0;
  for (int k = 0; k < n; k++)
  {
    sum += std::exp(m[k]);
  }
  for (int k = 0; k < n; k++)
  {
    ret[k] = (std::exp(m[k]) / sum);
  }
  return ret;
}

namespace activation
{
    Activation_func relu = relu_func;
    Activation_func softmax = softmax_func;
}
