//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"

#define MLP_SIZE 4
#define ERROR_INVALID_MLP_INPUT \
"Error: MlpNetwork Constructor input is not valid."
#define SOFTMAX_IND 3
#define RELU_INDXS 3
#define INIT_DIGIT_VALUE 0
#define INIT_DIGIT_PROBABILITY -1

/**
 * @struct digit
 * @brief Identified (by Mlp network) digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct digit {
	unsigned int value;
	float probability;
} digit;

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
									{64,  128},
									{20,  64},
									{10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
								 {64,  1},
								 {20,  1},
								 {10,  1}};

// Insert MlpNetwork class here...
class MlpNetwork
{
 private:
  Matrix* _weights;
  Matrix* _biases;
 public:
  /**
   * constructs MlpNetwork object
   * @param weights a Matrix array of size 4 (MLP_SIZE)
   *                with the dimensions mentioned above (weights_dims[])
   * @param biases a Matrix array of size 4 (MLP_SIZE)
   *               with the dimensions mentioned above (biases_dims[])
   * @throw srd:runtime_error in case dimensions of a Matrix does not match
   *                          with the mentioned above
   */
  MlpNetwork (Matrix weights[MLP_SIZE], Matrix biases[MLP_SIZE]);

  /**
   * implements all MlpNetworks layers on the input img
   * and returns the identified result
   * @param img Matrix by reference: the input image,
   *            should be a "vector" - Matrix with one column only
   * @return digit: with the result value and it's probability
   */
  digit operator()(Matrix& img);

};
#endif // MLPNETWORK_H