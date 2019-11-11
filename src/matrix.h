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
		struct
		{
			unsigned int w, h;
		};
		// n is count of cols, m is count of rows
		struct
		{
			unsigned int n, m;
		};
	};

	/** Changes flow of accessing `v` array members */
	bool transposed;

	/** Matrix values array */
	std::vector<t> v;

	// Setters
	/** Increments matrix width */
	virtual inline void s_iw() { if (transposed) h++; else w++; }
	/** Increments matrix height */
	virtual inline void s_ih() { if (transposed) w++; else h++; }


	/**
	* Checks other matrix for size identity
	* Uses assert
	*/
	inline void check_sizes(Matrix<t>& m) const;

public:

	/** Copy constructor */
	Matrix(const Matrix<t>& m);

	/** Skeleton constructor from array pointer */
	Matrix(const Matrix<t>& m, t _v[]);
	/** Skeleton constructor from vector */
	Matrix(const Matrix<t>& m, std::vector<t> _v);
	/** Skeleton constructor from vector pointer */
	Matrix(const Matrix<t>& m, std::vector<t>* _v);

	/** Default value constructor(zero) */
	Matrix(unsigned _w, unsigned _h, t _defv, bool _transposed = false);
	/** From array pointer */
	Matrix(unsigned _w, unsigned _h, t _v[], bool _transposed = false);
	/** From vector */
	Matrix(unsigned _w, unsigned _h, std::vector<t> _v, bool _transposed = false);
	/** From vector pointer */
	Matrix(unsigned _w, unsigned _h, std::vector<t>* _v, bool _transposed = false);


	// Getters
	/** Gets matrix values array size */
	inline unsigned int g_length() const { return w * h; }
	/** Gets matrix width */
	virtual inline unsigned int g_w() const { if (transposed) return h; return w; }
	/** Gets matrix height */
	virtual inline unsigned int g_h() const { if (transposed) return w; return h; }
	/** Gets matrix array */
	inline std::vector<t> g_v() const { return v; }
	/** Gets `transposed` flag */
	inline bool g_transposed() const { return transposed; }

	// Setters
	/** Switches `transposed` flag to an opposite */
	inline void transpose() { transposed = !transposed; }

	/** Inserts row to matrix */
	virtual void insert_row(t* rowv, unsigned roww);
	/** Inserts col to matrix */
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
	Matrix<t> operator +(const Matrix<t>& o);

	// - - - - - - - - - - - - - - - - 
	/** Unary minus */
	Matrix<t> operator -();
	Matrix<t> operator -(int o);
	Matrix<t> operator -(float o);
	Matrix<t> operator -(const Matrix<t>& o);

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
	bool operator ==(const Matrix<t>& o);

	// != != != != != != != != != != != != != 
	bool operator !=(int o);
	bool operator !=(float o);
	bool operator !=(const Matrix<t>& o);

	// += += += += += += += += += += += += += 
	Matrix<t>& operator +=(int o);
	Matrix<t>& operator +=(float o);
	Matrix<t>& operator +=(const Matrix<t>& o);
	// -= -= -= -= -= -= -= -= -= -= -= -= -= 
	Matrix<t>& operator -=(int o);
	Matrix<t>& operator -=(float o);
	Matrix<t>& operator -=(const Matrix<t>& o);
	// *= *= *= *= *= *= *= *= *= *= *= *= *= 
	Matrix<t>& operator *=(int o);
	Matrix<t>& operator *=(float o);
	Matrix<t>& operator *=(const Matrix<t>& o);
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
	template <class> friend std::ostream& operator<<(std::ostream& s, const Matrix<t>& m);

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
	/** Copy constructor */
	SquareMatrix(const Matrix<t>& m);

	/** Skeleton constructor from array pointer */
	SquareMatrix(const Matrix<t>& m, t _v[]);
	/** Skeleton constructor from vector */
	SquareMatrix(const Matrix<t>& m, std::vector<t> _v);
	/** Skeleton constructor from vector pointer */
	SquareMatrix(const Matrix<t>& m, std::vector<t>* _v);

	/** Default value constructor(zero) */
	SquareMatrix(unsigned _s, bool _transpose = false);
	/** From array pointer */
	SquareMatrix(unsigned _s, t _v[], bool _transpose = false);
	/** From vector */
	SquareMatrix(unsigned _s, std::vector<t> _v, bool _transpose = false);
	/** From vector pointer */
	SquareMatrix(unsigned _s, std::vector<t>* _v, bool _transpose = false);

	// Getters
	void insert_row() = delete;
	void insert_col() = delete;
	// Replace width and height getters with single side getter
	inline unsigned g_w() = delete;
	inline unsigned g_h() = delete;
	/** Gets matrix side */
	inline unsigned g_s() const { return Matrix<t>::g_w(); }

	/** Gets a matrix of a lower order by cutting down row and column */
	SquareMatrix<t> cut_matrix(unsigned r, unsigned c);

	/** Implementation of the Bareiss algorithm
	*	Copied from https://cs.nyu.edu/exact/core/download/core_v1.4/core_v1.4/progs/bareiss/bareiss.cpp
	*/
	long double determinant();

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

class MatrixMath {
public:
	template<class t> static Matrix<t> concat(Matrix<t>& m1, Matrix<t> m2) {
		for (int i = 0; i < m2.length(); i++) {
			//v.insert(v.begin() + add + i * w, colv[i]);
		}
	}
};




template<class t>
class Matrix3 : public SquareMatrix<t>
{
public:
	/** Copy constructor */
	Matrix3(const Matrix<t>& m);

	/** Skeleton constructor from array pointer */
	Matrix3(const Matrix<t>& m, t _v[9]);
	/** Skeleton constructor from vector */
	Matrix3(const Matrix<t>& m, std::vector<t> _v);
	/** Skeleton constructor from vector pointer */
	Matrix3(const Matrix<t>& m, std::vector<t>* _v);

	/** Default value constructor(zero) */
	Matrix3(bool _transpose = false);
	/** From array pointer */
	Matrix3(t _v[9], bool _transpose = false);
	/** From vector */
	Matrix3(std::vector<t> _v, bool _transpose = false);
	/** From vector pointer */
	Matrix3(std::vector<t>* _v, bool _transpose = false);
};

template<class t>
class Matrix4 : public SquareMatrix<t>
{
public:
	/** Copy constructor */
	Matrix4(const Matrix<t>& m);

	/** Skeleton constructor from array pointer */
	Matrix4(const Matrix<t>& m, t _v[16]);
	/** Skeleton constructor from vector */
	Matrix4(const Matrix<t>& m, std::vector<t> _v);
	/** Skeleton constructor from vector pointer */
	Matrix4(const Matrix<t>& m, std::vector<t>* _v);

	/** Default value constructor(zero) */
	Matrix4(bool _transpose = false);
	/** From array pointer */
	Matrix4(t _v[16], bool _transpose = false);
	/** From vector */
	Matrix4(std::vector<t> _v, bool _transpose = false);
	/** From vector pointer */
	Matrix4(std::vector<t>* _v, bool _transpose = false);
};


// Vector implementation based on matrices
template<class t>
class Vector : public Matrix<t>
{
public:
	/** Copy constructor */
	Vector(const Matrix<t>& v);

	/** Skeleton constructor from array pointer */
	Vector(const Matrix<t>& v, t _v[]);
	/** Skeleton constructor from vector */
	Vector(const Matrix<t>& v, std::vector<t> _v);
	/** Skeleton constructor from vector pointer */
	Vector(const Matrix<t>& v, std::vector<t>* _v);

	/** Default value constructor(zero) */
	Vector(unsigned _dim, bool _transpose = false);
	/** From array pointer */
	Vector(unsigned _dim, t _v[], bool _transpose = false);
	/** From vector */
	Vector(unsigned _dim, std::vector<t> _v, bool _transpose = false);
	/** From vector pointer */
	Vector(unsigned _dim, std::vector<t>* _v, bool _transpose = false);
};

// Vector implementation based on matrices
template<class t>
class Vector3 : public Vector<t>
{
public:
	/** Copy constructor */
	Vector3(const Matrix<t>& v);

	/** Skeleton constructor from array pointer */
	Vector3(const Matrix<t>& v, t _v[3]);
	/** Skeleton constructor from vector */
	Vector3(const Matrix<t>& v, std::vector<t> _v);
	/** Skeleton constructor from vector pointer */
	Vector3(const Matrix<t>& v, std::vector<t>* _v);

	/** Default value constructor(zero) */
	Vector3(bool _transpose = false);
	/** From array pointer */
	Vector3(t _v[3], bool _transpose = false);
	/** From vector */
	Vector3(std::vector<t> _v, bool _transpose = false);
	/** From vector pointer */
	Vector3(std::vector<t>* _v, bool _transpose = false);

	/** Gets x */
	inline unsigned int g_x() const { return (*this)(0); }
	/** Gets y */
	inline unsigned int g_y() const { return (*this)(1); }
	/** Gets additional coordinate in homogenous space */
	inline unsigned int g_a() const { return (*this)(2); }

	//TODO: ^ operator
	/** Scalar mult */
	inline t operator %(const Vector3<t>& v) const { return g_x() * v.g_x() + g_y() * v.g_y() + g_a() * v.g_a(); } // Scalar mult
	//inline t operator %=(const Vector3<t>& v) const { return g_x() * v.g_x() + g_y() * v.g_y() + g_a() * v.g_a(); } // Scalar mult
};

// Vector implementation based on matrices
template<class t>
class Vector4 : public Vector<t>
{
public:
	/** Copy constructor */
	Vector4(const Matrix<t>& m);

	/** Skeleton constructor from array pointer */
	Vector4(const Matrix<t>& m, t _v[4]);
	/** Skeleton constructor from vector */
	Vector4(const Matrix<t>& m, std::vector<t> _v);
	/** Skeleton constructor from vector pointer */
	Vector4(const Matrix<t>& m, std::vector<t>* _v);

	/** Default value constructor(zero) */
	Vector4(bool _transpose = false);
	/** From array pointer */
	Vector4(t _v[4], bool _transpose = false);
	/** From vector */
	Vector4(std::vector<t> _v, bool _transpose = false);
	/** From vector pointer */
	Vector4(std::vector<t>* _v, bool _transpose = false);

	/** Gets x */
	inline unsigned int g_x() const { return (*this)(0); }
	/** Gets y */
	inline unsigned int g_y() const { return (*this)(1); }	
	/** Gets z */
	inline unsigned int g_z() const { return (*this)(2); }
	/** Gets additional coordinate in homogenous space */
	inline unsigned int g_a() const { return (*this)(3); }
	

	//TODO: ^ operator
	/** Scalar mult */
	inline t operator %(const Vector4<t>& v) const { return g_x() * v.g_x() + g_y() * v.g_y() + g_z() * v.g_z() + g_a() * v.g_a(); } // Scalar mult
	//inline t operator %=(const Vector4<t>& v) const { v. g_x() * v.g_x() + g_y() * v.g_y() + g_z() * v.g_z() + g_a() * v.g_a(); } // Scalar mult
};


// Typedefs

typedef Matrix<int> Mati;
typedef Matrix<float> Matf;


#endif //__MATRIX_H__