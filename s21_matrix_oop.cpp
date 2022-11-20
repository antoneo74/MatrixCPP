#include "s21_matrix_oop.h"

/////////////          Конструкторы и деструктор        /////////////////

S21Matrix::S21Matrix() {
  rows_ = 0;
  cols_ = 0;
  matrix_ = nullptr;
}

S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows_ <= 0 || cols_ <= 0) {
    rows_ = 0;
    cols_ = 0;
    matrix_ = nullptr;
  } else {
    AllocateMemory();
  }
}

S21Matrix::S21Matrix(const S21Matrix& other) {
  rows_ = other.rows_;
  cols_ = other.cols_;
  AllocateMemory();
  CopyMatrix(other.matrix_);
}

S21Matrix::S21Matrix(S21Matrix&& other) noexcept {
  std::swap(rows_, other.rows_);
  std::swap(cols_, other.cols_);
  std::swap(matrix_, other.matrix_);
  other.rows_ = 0;
  other.cols_ = 0;
  other.matrix_ = nullptr;
}

S21Matrix::~S21Matrix() {
  if (this->matrix_) {
    FreeingMemory();
    rows_ = 0;
    cols_ = 0;
    matrix_ = nullptr;
  }
}

/////////////    Базовые функции для работы с матрицами   /////////////////

bool S21Matrix::EqMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    return false;
  }

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      if (fabs(matrix_[i][j] - other.matrix_[i][j]) > 1E-07) {
        return false;
      }
    }
  }
  return true;
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::invalid_argument("Sizes of matrices are different");
  }

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::invalid_argument("Sizes of matrices are different");
  }

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

void S21Matrix::MulNumber(const double num) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] *= num;
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows_) {
    throw std::invalid_argument(
        "Count cols first matrix not equal count rows second matrix");
  }
  S21Matrix temp(rows_, other.cols_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      double sum = 0;
      for (int k = 0; k < cols_; k++) {
        sum += matrix_[i][k] * other.matrix_[k][j];
      }
      temp.matrix_[i][j] = sum;
    }
  }
  this->FreeingMemory();
  cols_ = other.cols_;
  matrix_ = temp.matrix_;
  temp.matrix_ = nullptr;
}

S21Matrix S21Matrix::Transpose() {
  S21Matrix result(cols_, rows_);
  for (int j = 0; j < cols_; j++) {
    for (int i = 0; i < rows_; i++) {
      result.matrix_[j][i] = matrix_[i][j];
    }
  }
  return result;
}

S21Matrix S21Matrix::CalcComplements() {
  S21Matrix result(rows_, cols_);
  if (rows_ != cols_ || rows_ <= 0) {
    throw std::invalid_argument("Matrix isn't square");
  }
  if (rows_ == 1) {
    result.matrix_[0][0] = 1;
  } else if (rows_ == 2) {
    result.matrix_[0][0] = matrix_[1][1];
    result.matrix_[0][1] = -matrix_[1][0];
    result.matrix_[1][0] = -matrix_[0][1];
    result.matrix_[1][1] = matrix_[0][0];
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        result.matrix_[i][j] = Minor(i, j) * pow(-1, (i + j));
      }
    }
  }
  return result;
}

double S21Matrix::Determinant() {
  double result = 0.0;
  if (rows_ != cols_ || rows_ <= 0) {
    throw std::invalid_argument("Matrix isn't square");
  }
  S21Matrix temp = CalcComplements();

  for (int i = 0; i < rows_; i++) {
    result += matrix_[0][i] * temp.matrix_[0][i];
  }
  return result;
}

S21Matrix S21Matrix::InverseMatrix() {
  if (rows_ != cols_ || rows_ <= 0) {
    throw std::invalid_argument("Matrix isn't square");
  }
  double det = Determinant();
  if (fabs(det) < 1E-07) {
    throw std::out_of_range("Determenant equal 0");
  }

  S21Matrix result = CalcComplements();
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      result.matrix_[i][j] /= det;
    }
  }
  result = result.Transpose();
  return result;
}

/////////////     Перегрузка операторов    /////////////////

S21Matrix& S21Matrix::operator=(const S21Matrix& x) {
  if (this == &x) return *this;
  if (matrix_) {
    FreeingMemory();
  }
  rows_ = x.rows_;
  cols_ = x.cols_;
  AllocateMemory();
  CopyMatrix(x.matrix_);
  return *this;
}

S21Matrix& S21Matrix::operator=(S21Matrix&& x) noexcept {
  if (this == &x) return *this;
  if (matrix_) {
    FreeingMemory();
  }
  std::swap(rows_, x.rows_);
  std::swap(cols_, x.cols_);
  std::swap(matrix_, x.matrix_);
  x.matrix_ = nullptr;
  return *this;
}

bool S21Matrix::operator==(const S21Matrix& x) { return EqMatrix(x); }

S21Matrix S21Matrix::operator+(const S21Matrix& x) {
  S21Matrix result = *this;
  result.SumMatrix(x);
  return result;
}

S21Matrix S21Matrix::operator-(const S21Matrix& x) {
  S21Matrix result = *this;
  result.SubMatrix(x);
  return result;
}

S21Matrix S21Matrix::operator*(const S21Matrix& x) {
  S21Matrix result = *this;
  result.MulMatrix(x);
  return result;
}

S21Matrix S21Matrix::operator*(double x) {
  S21Matrix result = *this;
  result.MulNumber(x);
  return result;
}

S21Matrix operator*(const double x, const S21Matrix& y) {
  S21Matrix result = y;
  result.MulNumber(x);
  return result;
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& x) {
  this->SumMatrix(x);
  return *this;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& x) {
  this->SubMatrix(x);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& x) {
  this->MulMatrix(x);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const double x) {
  this->MulNumber(x);
  return *this;
}

double& S21Matrix::operator()(int i, int j) {
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
    throw std::out_of_range("Out of range. Incorrect input");
  }
  return matrix_[i][j];
}

/////////////     Геттеры и сеттеры    /////////////////

int S21Matrix::GetRows() { return rows_; }

int S21Matrix::GetCols() { return cols_; }

void S21Matrix::SetRows(int rows) {
  if (rows <= 0) {
    throw std::out_of_range("Incorrect value");
  } else if (rows_ != rows) {
    double** newmatrix = new double*[rows];
    for (int i = 0; i < rows; i++) {
      newmatrix[i] = new double[cols_]{};
    }
    int size = (rows_ > rows ? rows : rows_);

    for (int i = 0; i < size; i++) {
      for (int j = 0; j < cols_; j++) {
        newmatrix[i][j] = matrix_[i][j];
      }
    }
    FreeingMemory();
    rows_ = rows;
    matrix_ = newmatrix;
  }
}

void S21Matrix::SetCols(int cols) {
  if (cols <= 0) {
    throw std::out_of_range("Incorrect value");
  } else if (cols_ != cols) {
    double** newmatrix = new double*[rows_];
    for (int i = 0; i < rows_; i++) {
      newmatrix[i] = new double[cols]{};
    }
    int size = (cols_ > cols ? cols : cols_);

    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < size; j++) {
        newmatrix[i][j] = matrix_[i][j];
      }
    }
    FreeingMemory();
    cols_ = cols;
    matrix_ = newmatrix;
  }
}

///////////       Вспомогательные функции   //////////////////

void S21Matrix::AllocateMemory() {
  matrix_ = new double*[rows_];
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = new double[cols_]{};
  }
}

void S21Matrix::FreeingMemory() {
  for (int i = 0; i < rows_; i++) {
    delete[] matrix_[i];
  }
  delete[] matrix_;
}

void S21Matrix::CopyMatrix(double** sourse) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = sourse[i][j];
    }
  }
}

double S21Matrix::Minor(int x, int y) {
  double result = 0.0;
  S21Matrix temp(rows_ - 1, cols_ - 1);
  int a = 0, b = 0;
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      if (i != x && j != y) {
        temp.matrix_[a][b++] = matrix_[i][j];
        if (b == (rows_ - 1)) {
          a++;
          b = 0;
        }
      }
    }
  }
  if (temp.rows_ == 2) {
    result = temp.matrix_[0][0] * temp.matrix_[1][1] -
             temp.matrix_[0][1] * temp.matrix_[1][0];
  } else {
    result = Triangle(temp.matrix_, temp.rows_);
  }
  return result;
}

double S21Matrix::Triangle(double** matrix, int size) {
  int flag = 1;
  double result = 1;

  for (int k = 0; k < size - 1; k++) {
    if (!matrix[k][k]) {
      flag = ChangeRows(matrix, k, size);
    }
    if (!flag) {
      result = 0;
      break;
    } else {
      for (int i = k + 1; i < size; i++) {
        double kkk = matrix[i][k];
        if (kkk) {
          for (int j = k; j < size; j++) {
            matrix[i][j] = matrix[i][j] - matrix[k][j] * kkk / matrix[k][k];
          }
        }
      }
    }
  }
  if (result) {
    for (int i = 0; i < size; i++) {
      result *= matrix[i][i];
    }
  }
  return result;
}

int S21Matrix::ChangeRows(double** matrix, int k, int size) {
  int flag = 0;
  int i = k + 1;
  double temp;
  while (!matrix[k][k] && i < size) {
    for (int j = k; j < size; j++) {
      temp = matrix[k][j];
      matrix[k][j] = matrix[i][j];
      matrix[i][j] = -temp;
      if (matrix[k][k]) flag = 1;
    }
    if (flag) break;
    i++;
  }
  return flag;
}
