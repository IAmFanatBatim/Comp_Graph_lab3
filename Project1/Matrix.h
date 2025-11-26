#pragma once
#pragma once
#include <memory>
#include <iostream>
#include <math.h>

using namespace std;

class Matrix
{
	double** ptr;
	int n, m;
public:
	Matrix(int nRows, int nCols);
	friend const Matrix operator* (const Matrix& mat1, const Matrix& mat2);
	Matrix& operator=(const Matrix& mat);
	double* operator[] (const int row);
	const double* operator[](const int row) const;
	~Matrix();
};

class Affine : public Matrix
{
public:
	Affine();
	friend const Affine operator* (const Affine& mat1, const Affine& mat2);
	void addTransfer(double byX, double byY);
	void addScaling(double byX, double byY);
	void addRotation(double alpha);
	void addReflection(bool byX, bool byY);
};

class Point : public Matrix
{
public:
	Point(double x, double y);
	friend const Point operator* (const Affine& mat1, const Point& mat2);
	double getX();
	double getY();
};

Matrix::Matrix(int nRows, int nCols)
{
	ptr = (double**)malloc(sizeof(double*) * nRows);
	for (int row_ind = 0; row_ind < nRows; row_ind++) {
		ptr[row_ind] = (double*)malloc(sizeof(double) * nCols);
		for (int col_ind = 0; col_ind < nCols; col_ind++) {
			ptr[row_ind][col_ind] = 0;
		}
	}
	n = nRows;
	m = nCols;
}

const Matrix operator* (const Matrix& mat1, const Matrix& mat2)
{
	Matrix multed(mat1.n, mat2.m);
	for (int row_ind = 0; row_ind < multed.n; row_ind++) {
		for (int col_ind = 0; col_ind < multed.m; col_ind++) {
			multed.ptr[row_ind][col_ind] = 0;
			for (int summand_ind = 0; summand_ind < multed.m; summand_ind++) {
				multed.ptr[row_ind][col_ind] += mat1.ptr[row_ind][summand_ind] * mat2.ptr[summand_ind][col_ind];
			}
		}
	}
	return multed;
}

Matrix& Matrix::operator= (const Matrix& mat)
{
	this->n = mat.n;
	this->m = mat.m;
	for (int row_ind = 0; row_ind < n; row_ind++) {
		for (int col_ind = 0; col_ind < m; col_ind++) {
			(*this)[row_ind][col_ind] = mat[row_ind][col_ind];
		}
	}
	return *this;
}

double* Matrix::operator[](const int row)
{
	return ptr[row];
}

const double* Matrix::operator[](const int row) const
{
	return ptr[row];
}

Matrix::~Matrix() {
	for (int row_ind = 0; row_ind < n; row_ind++) {
		free(ptr[row_ind]);
	}
	free(ptr);
	ptr = NULL;
	n = 0;
	m = 0;
}


Point::Point(double x=0, double y=0) : Matrix(3, 1) {
	(*this)[0][0] = x;
	(*this)[1][0] = y;
	(*this)[2][0] = 1;
};

const Point operator* (const Affine& mat1, const Point& mat2) {
	Point multed(0, 0);
	for (int row_ind = 0; row_ind < 3; row_ind++) {
		multed[row_ind][0] = 0;
		for (int summand_ind = 0; summand_ind < 3; summand_ind++) {
			multed[row_ind][0] += mat1[row_ind][summand_ind] * mat2[summand_ind][0];
		}
	}
	return multed;
}

double Point::getX() {
	return (*this)[0][0];
};

double Point::getY() {
	return (*this)[1][0];
}

Affine::Affine() : Matrix(3, 3) {
	(*this)[0][0] = 1;
	(*this)[1][1] = 1;
	(*this)[2][2] = 1;
};

const Affine operator* (const Affine& mat1, const Affine& mat2) {
	Affine multed;
	for (int row_ind = 0; row_ind < 3; row_ind++) {
		for (int col_ind = 0; col_ind < 3; col_ind++) {
			multed[row_ind][col_ind] = 0;
			for (int summand_ind = 0; summand_ind < 3; summand_ind++) {
				multed[row_ind][col_ind] += mat1[row_ind][summand_ind] * mat2[summand_ind][col_ind];
			}
		}
	}
	return multed;
}

void Affine::addTransfer(double byX, double byY) {
	(*this)[0][2] = byX;
	(*this)[1][2] = byY;
};

void Affine::addScaling(double byX, double byY) {
	(*this)[0][0] = byX;
	(*this)[1][1] = byY;
};

void Affine::addRotation(double alpha)
{
	(*this)[0][0] = cos(alpha);
	(*this)[0][1] = -sin(alpha);
	(*this)[1][0] = sin(alpha);
	(*this)[1][1] = cos(alpha);
};

void Affine::addReflection(bool byX, bool byY)
{
	if (byX) {
		(*this)[1][1] = -1;
	};
	if (byY) {
		(*this)[0][0] = -1;
	};
};

