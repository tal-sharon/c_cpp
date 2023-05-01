#include "Dense.h"

Dense::Dense (Matrix &weights, Matrix &bias, Activation_func func)
{
  _weights = weights;
  _bias = bias;
  _func = *func;
}

Matrix Dense::get_weights () const
{
  return _weights;
}
Matrix Dense::get_bias () const
{
  return _bias;
}
Activation_func Dense::get_activation () const
{
  return *_func;
}
Matrix Dense::operator() (const Matrix &input) const
{
  return Matrix(_func ((_weights*input) + _bias));
}
