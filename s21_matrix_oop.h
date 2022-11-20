#ifndef MATRIX_SRC_S21_MATRIX_OOP_H
#define MATRIX_SRC_S21_MATRIX_OOP_H

#include <cmath>
#include <exception>
#include <iostream>

class S21Matrix {
 private:
  int rows_, cols_;
  double** matrix_;

 public:
  // Конструкторы и деструктор
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other) noexcept;
  ~S21Matrix();

  // Базовые функции для работы с матрицами
  bool EqMatrix(const S21Matrix& other);
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose();
  S21Matrix CalcComplements();
  double Determinant();
  S21Matrix InverseMatrix();

  // Перегрузка операторов
  S21Matrix& operator=(const S21Matrix& x);
  S21Matrix& operator=(S21Matrix&& x) noexcept;
  bool operator==(const S21Matrix& x);
  S21Matrix operator+(const S21Matrix& x);
  S21Matrix operator-(const S21Matrix& x);
  S21Matrix operator*(const S21Matrix& x);
  friend S21Matrix operator*(const double x, const S21Matrix& y);
  S21Matrix operator*(double x);
  S21Matrix& operator+=(const S21Matrix& x);
  S21Matrix& operator-=(const S21Matrix& x);
  S21Matrix& operator*=(const S21Matrix& x);
  S21Matrix& operator*=(const double x);
  double& operator()(int i, int j);

  // Геттеры и сеттеры
  int GetRows();
  int GetCols();
  void SetRows(int rows);
  void SetCols(int cols);

 private:
  // Вспомогательные функции
  void CopyMatrix(double** sourse);
  void AllocateMemory();
  void FreeingMemory();
  double Minor(int x, int y);
  double Triangle(double** matrix, int size);
  int ChangeRows(double** matrix, int k, int size);
};

#endif  // MATRIX_SRC_S21_MATRIX_OOP_H
