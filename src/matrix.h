#ifndef __MATRIX_H__
#define __MATRIX_H__

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <cassert>
#include <iterator>
#include <iomanip>      // std::setw
#include <xmmintrin.h>
#include <functional>
#include "geometry.h"


class MatrixException : public std::runtime_error {
public:
	/** Default constructor */
	MatrixException() :runtime_error("Unexepcted error") {}
	/** Custom message constructor */
	MatrixException(std::string msg) :runtime_error(msg.c_str()) {}
};

template <class t>
class Matrix
{
protected:
	union {
		// height and width
		struct { unsigned int width, height; };
		// n is count of cols, m is count of rows
		struct { unsigned int n, m; };
		struct { int cols, rows; };
	};

	/** Changes flow of accessing `v` array members */
	bool transposed;

	/** Matrix values array */
	//std::vector<t> v;
	t* v;

	// Setters
	/** Increments matrix width */
	inline void s_inc_w() { transposed ? height++ : width++; }
	/** Increments matrix height */
	inline void s_inc_h() { transposed ? width++ : height++; }


	/**
	* Checks other matrix for size identity
	* Uses assert
	*/
	inline void check_sizes(Matrix<t>& m) const;

public:
	~Matrix();
	Matrix();

	/** Copy constructor */
	Matrix(const Matrix<t>& m);

	/** Skeleton constructor from array pointer */
	Matrix(const Matrix<t>& m, t _v[]);
	/** Skeleton constructor from vector */
	Matrix(const Matrix<t>& m, const std::vector<t>& _v);
	/** Skeleton constructor from vector pointer */
	Matrix(const Matrix<t>& m, std::vector<t>* _v);

	/** Size constructor */
	Matrix(unsigned _w, unsigned _h, bool _t = false);
	/** Default value constructor(zero) */
	Matrix(unsigned _w, unsigned _h, t _defv, bool _t = false);
	/** From array pointer */
	Matrix(unsigned _w, unsigned _h, t _v[], bool _t = false);
	/** From vector */
	Matrix(unsigned _w, unsigned _h, const std::vector<t>& _v, bool _t = false);
	/** From vector pointer */
	Matrix(unsigned _w, unsigned _h, std::vector<t>* _v, bool _t = false);


	// Getters
	/** Gets matrix values array size */
	inline unsigned g_length() const { return width * height; }
	/** Gets matrix width */
	inline unsigned g_w() const { return transposed ? height : width; }
	/** Gets matrix height */
	inline unsigned g_h() const { return transposed ? width : height; }
	/** Gets matrix array */
	inline t* g_v() const { return v; }
	/** Gets `transposed` flag */
	inline bool g_transposed() const { return transposed; }

	// Setters
	/** Switches `transposed` flag to an opposite */
	inline void transpose() { transposed = !transposed; }

	/** Inserts row to matrix */
	virtual void insert_row(t* rowv, unsigned roww);
	///** Inserts col to matrix */
	virtual void insert_col(t* colv, unsigned colh);

	/** Applies function for every element of matrix array */
	void for_each(std::function<void(t&)> callback);

	/**
	* Outputs matrix
	* Respects `transpose`
	*/
	void log();

	// + + + + + + + + + + + + + + + + + + + 
	Matrix<t> operator +(int o);
	Matrix<t> operator +(float o);
	Matrix<t> operator +(Matrix<t>& o);

	// - - - - - - - - - - - - - - - - 
	/** Unary minus */
	Matrix<t> operator -();
	Matrix<t> operator -(int o);
	Matrix<t> operator -(float o);
	Matrix<t> operator -(Matrix<t>& o);

	// * * * * * * * * * * * * * * * * * 
	Matrix<t> operator *(int o);
	Matrix<t> operator *(float o);
	/** Perfmorms matrix multiplication */
	//Matrix<t> operator *(Matrix<t> o);
	/** Perfmorms matrix multiplication */
	Matrix<t> operator *(Matrix<t>& o);

	/** Perfmorms multiplication of matrix on 2d vector
	*	Matrix has to be width 2 and height at least 2 (all after 2nd row will be ignored)
	*/
	Vec2i operator *(const Vec2i& o) const;
	/** Perfmorms multiplication of matrix on 2d vector
	*	Matrix has to be width 2 and height at least 2 (all after 2nd row will be ignored)
	*/
	Vec2f operator *(const Vec2f& o) const;
	/** Perfmorms multiplication of matrix on 3d vector
	*	Matrix has to be width 3 and height at least 3 (all after 3nd row will be ignored)
	*/
	Vec3i operator *(const Vec3i& o) const;
	/** Perfmorms multiplication of matrix on 3d vector
	*	Matrix has to be width 3 and height at least 3 (all after 3nd row will be ignored)
	*/
	Vec3f operator *(const Vec3f& o) const;

	// / / / / / / / / / / / / / / / / / / / / / / / / 
	Matrix<t> operator /(int o);
	Matrix<t> operator /(float o);
	Matrix<t> operator /(double o);
	Matrix<t> operator /(long double o);

	// == == == == == == == == == == == ==
	bool operator ==(int o);
	bool operator ==(float o);
	bool operator ==(Matrix<t>& o);

	// != != != != != != != != != != != != != 
	bool operator !=(int o);
	bool operator !=(float o);
	bool operator !=(Matrix<t>& o);

	// += += += += += += += += += += += += += 
	Matrix<t>& operator +=(int o);
	Matrix<t>& operator +=(float o);
	Matrix<t>& operator +=(Matrix<t>& o);
	// -= -= -= -= -= -= -= -= -= -= -= -= -= 
	Matrix<t>& operator -=(int o);
	Matrix<t>& operator -=(float o);
	Matrix<t>& operator -=(Matrix<t>& o);
	// *= *= *= *= *= *= *= *= *= *= *= *= *= 
	Matrix<t>& operator *=(int o);
	Matrix<t>& operator *=(float o);
	Matrix<t>& operator *=(Matrix<t>& o);
	// /= /= /= /= /= /= /= /= /= /= /= /= 
	Matrix<t>& operator /=(int o);
	Matrix<t>& operator /=(float o);
	Matrix<t>& operator /=(double o);
	Matrix<t>& operator /=(long double o);

	// +++++++++++++++++++++++++++
	Matrix<t>& operator ++(); // Prefix increment operator.
	Matrix<t> operator ++(int);// Postfix increment ope rator.

	// ---------------------------
	Matrix<t>& operator --(); // Prefix increment operator.
	Matrix<t> operator --(int);// Postfix increment ope rator.

	/// << << << << << << << << <<
	template <class> friend std::ostream& operator<<(std::ostream& s, Matrix<t>& m);

	/**
	* Implementation of 2d array [] operator.
	* Returns row by index
	*/
	inline std::vector<t>  operator [](unsigned i) const;
	/**
	* Implementation of 1d array [] operator
	* DON'T MIX UP WITH [] OPERATOR, THAT GETS MATRIX ROW
	* This one returns the actual element from values array
	*/
	inline t& operator ()(unsigned i);

	/**
	* Implementation of 2d array [][] operator.
	* Returns matrix element
	*/
	inline t& operator () (unsigned i1, unsigned i2);
};

template <class t> std::ostream& operator <<(std::ostream& s, const Matrix<t>& m);

template<class t>
class SquareMatrix : public Matrix<t> {
public:
	SquareMatrix();

	/** Copy constructor */
	SquareMatrix(const Matrix<t>& m);

	/** Skeleton constructor from array pointer */
	SquareMatrix(const Matrix<t>& m, t _v[]);
	/** Skeleton constructor from vector */
	SquareMatrix(const Matrix<t>& m, const std::vector<t>& _v);
	/** Skeleton constructor from vector pointer */
	SquareMatrix(const Matrix<t>& m, std::vector<t>* _v);

	/** Size constructor */
	SquareMatrix(unsigned _s, bool _t = false);
	/** Default value constructor(zero) */
	SquareMatrix(unsigned _s, t _defv, bool _t = false);
	/** From array pointer */
	SquareMatrix(unsigned _s, t _v[], bool _t = false);
	/** From vector */
	SquareMatrix(unsigned _s, std::vector<t>& _v, bool _t = false);
	/** From vector pointer */
	SquareMatrix(unsigned _s, std::vector<t>* _v, bool _t = false);

	// Getters
	void insert_row() = delete;
	void insert_col() = delete;
	/** Gets matrix side */
	inline unsigned g_s() const { return Matrix<t>::g_w(); }

	/** Gets a matrix of a lower order by cutting down row and column */
	SquareMatrix<t> cut_matrix(unsigned r, unsigned c);


	/** Implementation of the Bareiss algorithm
	*	Copied from https://cs.nyu.edu/exact/core/download/core_v1.4/core_v1.4/progs/bareiss/bareiss.cpp
	*
	*   Note that this implementation can include divisions by zero which will return nan
	*
	*   To fix this you have to swap rows with 0 on iteration change,
	*   like here https://www.math.usm.edu/perry/Research/Thesis_DRL.pdf (type "Division by Zero in Bareiss" while on page to find solution)
	*	This is very expensive solution, but even it won't give acceptable results as after swap 0 can still be on its place
	*   so wee need to look up a whole col to find non-zero element, which is even more expensive
	*
	*	But as far as I know every algorithm that has complexity O(n^3) and lower encounters this problem
	*/
	long double determinant();


	void swap_rows(unsigned r1, unsigned r2) {
		for (unsigned i = 0; i < Matrix<t>::g_w(); i++) {
			std::swap((*this)(r1, i), (*this)(r2, i));
		}
	}

	/** Finds minor of matrix */
	inline long double minor(int r, int c) {
		return cut_matrix(r, c).determinant();
	}

	/** Finds algebraic */
	inline long double complement(int r, int c) {
		return (((r + c) % 2) ? -1 : 1) * minor(r, c);
	}

	/** Returns inverse matrix */
	SquareMatrix<t> inverse();
};

template<class t>
class Matrix3 : public SquareMatrix<t>
{
public:
	Matrix3();

	/** Copy constructor */
	Matrix3(const Matrix<t>& m);

	/** Skeleton constructor from array pointer */
	Matrix3(const Matrix<t>& m, t _v[9]);
	/** Skeleton constructor from vector */
	Matrix3(const Matrix<t>& m, const std::vector<t>& _v);
	/** Skeleton constructor from vector pointer */
	Matrix3(const Matrix<t>& m, std::vector<t>* _v);

	/** Default value constructor(zero) */
	Matrix3(t _defv = t(), bool _t = false);
	/** From array pointer */
	Matrix3(t _v[9], bool _t = false);
	/** From vector */
	Matrix3(std::vector<t>& _v, bool _t = false);
	/** From vector pointer */
	Matrix3(std::vector<t>* _v, bool _t = false);
};

template<class t>
class Matrix4 : public SquareMatrix<t>
{
public:
	Matrix4();

	/** Copy constructor */
	Matrix4(const Matrix<t>& m);

	/** Skeleton constructor from array pointer */
	Matrix4(const Matrix<t>& m, t _v[16]);
	/** Skeleton constructor from vector */
	Matrix4(const Matrix<t>& m, const std::vector<t>& _v);
	/** Skeleton constructor from vector pointer */
	Matrix4(const Matrix<t>& m, std::vector<t>* _v);

	/** Default value constructor(zero) */
	Matrix4(t _defv, bool _t = false);
	/** From array pointer */
	Matrix4(t _v[16], bool _t = false);
	/** From vector */
	Matrix4(std::vector<t>& _v, bool _t = false);
	/** From vector pointer */
	Matrix4(std::vector<t>* _v, bool _t = false);
};

// Typedefs

typedef Matrix<int> Mati;
typedef Matrix<float> Matf;


#endif //__MATRIX_H__