#ifndef __MATRIX_H__
#define __MATRIX_H__

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <cassert>
#include <iterator>
#include <iomanip>      // std::setw
#include <xmmintrin.h>
#include "geometry.h"

template <class t>
class Matrix
{
public:
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
	bool transpose;

	/** Matrix values array */
	std::vector<t> v;

	/** Zero constructor */
	Matrix(unsigned int _w, unsigned int _h, bool _transpose = false);
	/** From array pointer */
	Matrix(unsigned int _w, unsigned int _h, t* _v, bool _transpose = false) : w(_w), h(_h), v(_v, _v + _w * _h), transpose(_transpose) {};
	/** From vector */
	Matrix(unsigned int _w, unsigned  int _h, std::vector<t> _v, bool _transpose = false) : w(_w), h(_h), v(_v.begin(), _v.end()), transpose(_transpose) {}
	/** From vector pointer */
	Matrix(unsigned int _w, unsigned  int _h, std::vector<t>* _v, bool _transpose = false) : w(_w), h(_h), v(_v->begin(), _v->end()), transpose(_transpose) {}


	/** Gets matrix values array size */
	inline int length() const { return w * h; }

	/** Inserts row to matrix */
	void insert_row(t* rowv, int roww);

	/** Inserts col to matrix */
	void insert_col(t* colv, int colh);

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
	Matrix<t> operator -(int other);
	Matrix<t> operator -(float other);
	Matrix<t> operator -(Matrix<t>& other);

	// * * * * * * * * * * * * * * * * * 
	Matrix<t> operator *(int other);
	Matrix<t> operator *(float other);
	/** Perfmorms matrix multiplication */
	Matrix<t> operator *(Matrix<t>& other);

	// / / / / / / / / / / / / / / / / / / / / / / / / 

	Matrix<t> operator /(int other);
	Matrix<t>& operator /(float other);

	// = = = = = = = = = = = = = = = = = = = = = 

	bool operator ==(int other);
	bool operator ==(float other);
	bool operator ==(Matrix<t>& other);

	// != != != != != != != != != != != != != 

	bool operator !=(int other);
	bool operator !=(float other);
	bool operator !=(Matrix<t>& other);

	/**
	* Implementation of 1d array [] operator
	* DON'T MIX UP WITH () OPERATOR, THAT GETS MATRIX ROW
	* This one returns the actual `v` array element
	*/
	t operator [](int i);
	/**
	* Implementation of 2d array [] operator.
	* Returns row by index
	*/
	std::vector<t> operator ()(int i);
	/**
	* Implementation of 2d array [][] operator.
	* Returns matrix element
	*/
	t operator ()(int i1, int i2);

private:
	/**
	* Checks other matrix for size identity
	* Uses assert
	*/
	inline void check_sizes(Matrix<t>& m);
};

typedef Matrix<int> Mati;
typedef Matrix<float> Matf;
typedef Matrix<Vec2i> Matv2i;
typedef Matrix<Vec3i> Matv3i;
typedef Matrix<Vec2f> Matv2f;
typedef Matrix<Vec3f> Matv3f;

#endif //__MATRIX_H__