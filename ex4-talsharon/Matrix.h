// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <fstream>

#define ERROR_IND_OUT_OF_RANGE "Error: index is out of range."
#define ERROR_DOT_DIM \
"Error: rhs Matrix and lhs Matrix should have the same dimensions."
#define ERROR_ADDITION_DIMS \
"Error: rhs Matrix and lhs Matrix should have the same dimensions."
#define ERROR_MULTIPLICATION_DIMS \
"Error: rhs Matrix's columns should be the same size of lhs Matrix's rows."
#define ERROR_INPUT_LENGTH \
"Error: Input length is not compatible with Matrix's dimensions."
#define ERROR_INIT_MATRIX_DIMS "Error: Matrix dimensions should be positive."
#define MIN_PRINT_VAL 0.1

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
	int rows, cols;
} matrix_dims;

class Matrix
{
 private:
  // data
  matrix_dims _dims{};
  float **_matrix;

 public:
  // constructors and destructor
  /**
   *
   * @param rows int: number of rows
   * @param cols int: number of columns
   * @throw std::length_error if rows or cols aren't positive integers.
   */
  Matrix(int rows, int cols);
  Matrix();
  Matrix(const Matrix& m);
  ~Matrix();

  // methods
  int get_rows() const
  {
    return _dims.rows;
  }
  int get_cols() const
  {
    return _dims.cols;
  }
  Matrix& transpose();
  Matrix& vectorize();

  /**
   * prints Matrix's entries by it's float values
   */
  void plain_print() const;

  /**
   *
   * @param m rhs Matrix by reference
   * @throw std::length_error in case rhs Matrix does not have the same
   *                          dimensions as lhs Matrix
   * @return lhs Matrix by reference
   */
  Matrix dot(const Matrix& m) const;

  float norm() const;

  // operators
  /**
   * initiates new Matrix of addition of two Matrices
   * @param a lhs Matrix by reference
   * @param b lhs Matrix by reference
   * @throw std::length_error in case rhs Matrix does not have the same
   *                          dimensions as lhs Matrix
   * @return Matrix by value: the result of the addition
   */
  friend Matrix operator+(const Matrix& a, const Matrix& b);
  Matrix& operator=(const Matrix& m);

  /**
   * multiplication of two Matrices
   * @param a lhs Matrix by reference
   * @param b lhs Matrix by reference
   * @throw std::length_error in case rhs Matrix does not have the same
   *                          dimensions as lhs Matrix
   * @return
   */
  friend Matrix operator*(const Matrix& a, const Matrix& b);
  friend Matrix operator*(const Matrix& m, float c);
  friend Matrix operator*(float c, const Matrix& m);

  /**
   * adds rhs Matrix to lhs Matrix
   * @param m rhs Matrix by reference
   * @throw std::length_error in case rhs Matrix does not have the same
   *                          dimensions as lhs Matrix
   * @return updated lhs Matrix by reference
   */
  Matrix& operator+=(const Matrix& m);

  /**
   * gets value of Matrix's entry in row and column given
   * @param row int: row of entry
   * @param col int: column of entry
   * @throw std::out_of_range in case row or col are out of range
   * @return float: value of Matrix's entry in row and column given
   */
  float& operator()(int row, int col);

  /**
 * gets value of const Matrix's entry in row and column given
 * @param row int: row of entry
 * @param col int: column of entry
 * @throw std::out_of_range in case row or col are out of range
 * @return float: value of const Matrix's entry in row and column given
 */
  float operator()(int row, int col) const;

  /**
   * gets the value of th i'th entry of a matrix
   * @param n int: wanted index of the Matrix
   * @throw std::out_of_range in case given index is out of range
   * @return float: value of th i'th entry of a matrix
   */
  float& operator[](int n);

  /**
 * gets the value of th i'th entry of a const Matrix
 * @param n int: wanted index of the Matrix
 * @throw std::out_of_range in case given index is out of range
 * @return float: value of th i'th entry of a const Matrix
 */
  float operator[](int n) const;

  /**
   * gets visualized output stream of matrix
   * @param os output stream
   * @param m Matrix
   * @return std::ostream by reference: updated with the Matrix
   */
  friend std::ostream& operator<<(std::ostream& os, const Matrix& m);

  /**
   * fills a Matrix with binary input stream of floats
   * @param is std::istream binary input stream
   * @param m Matrix by reference
   * @throw std::runtime_error in case stream's length is not compatible with
   *                           Matrix's dimensions
   * @return std::istream
   */
  friend std::istream& operator>>(std::istream& is, Matrix& m);
};

#endif //MATRIX_H