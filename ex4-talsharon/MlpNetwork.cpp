#include "MlpNetwork.h"
#include <iostream>

MlpNetwork::MlpNetwork (Matrix weights[MLP_SIZE], Matrix biases[MLP_SIZE])
{
  for (int i = 0; i < MLP_SIZE; i++)
  {
    if (weights[i].get_rows() != weights_dims[i].rows ||
        weights[i].get_cols() != weights_dims[i].cols ||
        biases[i].get_rows() != bias_dims[i].rows ||
        biases[i].get_cols() != bias_dims[i].cols)
    {
      throw std::runtime_error (ERROR_INVALID_MLP_INPUT);
    }
  }
  _weights = weights;
  _biases = biases;
}

digit MlpNetwork::operator() (Matrix &img)
{
  Matrix vec = Matrix(img);
  for (int i = 0; i < RELU_INDXS; i++)
  {
    Dense layer = Dense (_weights[i], _biases[i],
                                     activation::relu);

    vec = layer (vec);
  }
  Dense layer = Dense (_weights[SOFTMAX_IND], _biases[SOFTMAX_IND],
                       activation::softmax);
  vec = layer (vec);
  digit res {INIT_DIGIT_VALUE, INIT_DIGIT_PROBABILITY};
  for (int i = 0; i < vec.get_rows(); i++)
  {
    if (vec[i] > res.probability)
    {
      res.value = i;
      res.probability = vec[i];
    }
  }
  return res;
}

