#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

// Insert Dense class here...
class Dense
{
 private:
  Matrix _weights;
  Matrix _bias;
  Activation_func _func;

 public:
  // constructors and destructor
  Dense (Matrix &weights, Matrix &bias, Activation_func func);

  // methods
  Matrix get_weights() const;
  Matrix get_bias() const;
  Activation_func get_activation() const;

  // operator
  Matrix operator()(const Matrix &input) const;
};

#endif //DENSE_H
