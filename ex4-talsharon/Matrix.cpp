#include "Matrix.h"
#include <cmath>

// constructors and destructor
Matrix::Matrix (int rows, int cols)
{
  if (rows <= 0 || cols <= 0)
  {
    throw std::length_error (ERROR_INIT_MATRIX_DIMS);
  }
  _dims.rows = rows;
  _dims.cols = cols;
  _matrix = new float*[rows];
  for (int i = 0; i < rows; i++)
  {
    _matrix[i] = new float[cols];
    for (int j = 0; j < cols; j++)
    {
      _matrix[i][j] = 0;
    }
  }
}

Matrix::Matrix ()
: Matrix(1, 1)
{
}

Matrix::~Matrix()
{
  for (int i = 0; i < _dims.rows; i++)
  {
    delete[] _matrix[i];
  }
  delete[] _matrix;
}

Matrix::Matrix (const Matrix &m)
{
  _dims.rows = m._dims.rows;
  _dims.cols = m._dims.cols;
  _matrix = new float*[_dims.rows];
  for (int i = 0; i < _dims.rows; i++)
  {
    _matrix[i] = new float[_dims.cols];
    for (int j = 0; j < _dims.cols; j++)
    {
      _matrix[i][j] = m._matrix[i][j];
    }
  }
}


//methods

Matrix &Matrix::transpose ()
{
  Matrix temp(_dims.cols, _dims.rows);
  for (int i = 0; i < _dims.rows; i++)
  {
    for (int j = 0; j < _dims.cols; j++)
    {
      temp._matrix[j][i] = _matrix[i][j];
    }
  }
  *this = temp;
  return *this;
}

Matrix &Matrix::vectorize ()
{
  Matrix temp(_dims.rows*_dims.cols, 1);
  for (int i = 0; i < _dims.rows; i++)
  {
    for (int j = 0; j < _dims.cols; j++)
    {
      temp._matrix[(i)*_dims.cols + j][0] = _matrix[i][j];
    }
  }
  *this = temp;
  return *this;
}

void Matrix::plain_print () const
{
  for (int i = 0; i < _dims.rows; i++)
  {
    for (int j = 0; j < _dims.cols; j++)
    {
      std::cout << _matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

Matrix Matrix::dot (const Matrix& m) const
{
  if (_dims.rows != m._dims.rows || _dims.cols != m._dims.cols)
  {
    throw std::length_error (ERROR_DOT_DIM);
  }
  Matrix ret(_dims.rows, _dims.cols);
  for (int i = 0; i < _dims.rows; i++)
  {
    for (int j = 0; j < _dims.cols; j++)
    {
      ret._matrix[i][j] = _matrix[i][j] * m._matrix[i][j];
    }
  }
  return ret;
}

float Matrix::norm () const
{
  float norm = 0;
  for (int i = 0; i < _dims.rows; i++)
  {
    for (int j = 0; j < _dims.cols; j++)
    {
      norm += _matrix[i][j] * _matrix[i][j];
    }
  }
  return std::sqrt(norm);
}


// operators

Matrix operator+ (const Matrix& a, const Matrix& b)
{
  if (a._dims.rows != b._dims.rows || a._dims.cols != b._dims.cols)
  {
    throw std::length_error (ERROR_ADDITION_DIMS);
  }
  Matrix ret(a._dims.rows, a._dims.cols);
  for (int i = 0; i < a._dims.rows; i++)
  {
    for (int j = 0; j < a._dims.cols; j++)
    {
      ret._matrix[i][j] = a._matrix[i][j] + b._matrix[i][j];
    }
  }
  return ret;
}

Matrix &Matrix::operator= (const Matrix &m)
{
  // check if lhs and rhs are identically the same
  if (this == &m)
  {
    return *this;
  }

  // delete lhs Matrix's data
  for (int i = 0; i < _dims.rows; i++)
  {
    delete[] _matrix[i];
  }
  delete[] _matrix;

  // assign rhs data to lhs
  _dims.rows = m._dims.rows;
  _dims.cols = m._dims.cols;
  _matrix = new float*[_dims.rows];
  for (int i = 0; i < _dims.rows; i++)
  {
    _matrix[i] = new float[_dims.cols];
    for (int j = 0; j < _dims.cols; j++)
    {
      _matrix[i][j] = m._matrix[i][j];
    }
  }
  return *this;
}

Matrix &Matrix::operator+= (const Matrix &m)
{
  if (_dims.rows != m._dims.rows || _dims.cols != m._dims.cols)
  {
    throw std::length_error (ERROR_ADDITION_DIMS);
  }
  for (int i = 0; i < _dims.rows; i++)
  {
    for (int j = 0; j < _dims.cols; j++)
    {
      _matrix[i][j] += m._matrix[i][j];
    }
  }
  return *this;
}

float Matrix::operator() (int row, int col) const
{
  if (row >= _dims.rows || col >= _dims.cols ||
      row < 0 || col < 0)
  {
    throw std::out_of_range (ERROR_IND_OUT_OF_RANGE);
  }
  return _matrix[row][col];
}

float& Matrix::operator() (int row, int col)
{
  if (row >= _dims.rows || col >= _dims.cols ||
      row < 0 || col < 0)
  {
    throw std::out_of_range (ERROR_IND_OUT_OF_RANGE);
  }
  return _matrix[row][col];
}

float Matrix::operator[] (int n) const
{
  int row = n / _dims.cols;
  int col = n % _dims.cols;
  if (row >= _dims.rows || col >= _dims.cols ||
      row < 0 || col < 0)
  {
    throw std::out_of_range (ERROR_IND_OUT_OF_RANGE);
  }
  return _matrix[row][col];
}

float& Matrix::operator[] (int n)
{
  int row = n / _dims.cols;
  int col = n % _dims.cols;
  if (row >= _dims.rows || col >= _dims.cols ||
      row < 0 || col < 0)
  {
    throw std::out_of_range (ERROR_IND_OUT_OF_RANGE);
  }
  return _matrix[row][col];
}

Matrix operator* (const Matrix &a, const Matrix &b)
{
  if (a._dims.cols != b._dims.rows)
  {
    throw std::length_error (ERROR_MULTIPLICATION_DIMS);
  }
  int rows = a._dims.rows;
  int cols = b._dims.cols;
  Matrix ret(rows, cols);
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      for (int k = 0; k < a._dims.cols; k++)
      {
        ret._matrix[i][j] += a._matrix[i][k] * b._matrix[k][j];
      }
    }
  }
  return ret;
}

Matrix operator* (const Matrix &m, float c)
{
  Matrix ret(m._dims.rows, m._dims.cols);
  for (int i = 0; i < m._dims.rows; i++)
  {
    for (int j = 0; j < m._dims.cols; j++)
    {
      ret._matrix[i][j] = m._matrix[i][j] * c;
    }
  }
  return ret;
}

Matrix operator* (float c, const Matrix &m)
{
  return m*c;
}

std::ostream &operator<< (std::ostream &os, const Matrix &m)
{
  for (int i = 0; i < m._dims.rows; i++)
  {
    for (int j = 0; j < m._dims.cols; j++)
    {
      if (m(i,j) > MIN_PRINT_VAL)
      {
        os << "**";
      }
      else
      {
        os << "  ";
      }
    }
    os << std::endl;
  }
  return os;
}

std::istream &operator>> (std::istream &is, Matrix &m)
{
  is.seekg (0, std::ifstream::end);
  int length = (int) is.tellg();
  is.seekg (0, std::ifstream::beg);
  if (length/sizeof (float) != (long unsigned int) m.get_rows()*m.get_cols())
  {
    throw std::runtime_error (ERROR_INPUT_LENGTH);
  }
  for (int i = 0; i < m._dims.rows; i++)
  {
    for (int j = 0; j < m._dims.cols; j++)
    {
      if (is.good())
      {
        is.read ((char *) &m._matrix[i][j], sizeof (float));
      }
    }
  }
  return is;
}
