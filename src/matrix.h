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
private:
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

protected:
	/** Changes flow of accessing `v` array members */
	bool transposed;

	/** Matrix values array */
	std::vector<t> v;

	// Setters
	/** Increments matrix width */
	inline void s_iw() { if (transposed) h++; else w++; };
	/** Increments matrix height */
	inline void s_ih() { if (transposed) w++; else h++; };


	/**
	* Checks other matrix for size identity
	* Uses assert
	*/
	inline void check_sizes(Matrix<t>& m) const;

public:

	/** Default value constructor */
	Matrix(unsigned int _w, unsigned int _h, t _defv, bool _transposed = false);
	/** From array pointer */
	Matrix(unsigned int _w, unsigned int _h, t* _v, bool _transposed = false);
	/** From vector */
	Matrix(unsigned int _w, unsigned  int _h, std::vector<t> _v, bool _transposed = false);
	/** From vector pointer */
	Matrix(unsigned int _w, unsigned  int _h, std::vector<t>* _v, bool _transposed = false);


	// Getters
	/** Gets matrix values array size */
	inline unsigned int g_length() const { return w * h; }
	/** Gets matrix width */
	inline unsigned int g_w() const { if (transposed) return h; return w; }
	/** Gets matrix height */
	inline unsigned int g_h() const { if (transposed) return w; return h; }
	/** Gets matrix array values */
	inline unsigned int g_v() const { return h; }

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
	Matrix<t> operator +(int o) ;
	Matrix<t> operator +(float o) ;
	Matrix<t> operator +(Matrix<t>& o) ;

	// - - - - - - - - - - - - - - - - 
	/** Unary minus */
	Matrix<t> operator -() ;
	Matrix<t> operator -(int o) ;
	Matrix<t> operator -(float o) ;
	Matrix<t> operator -(Matrix<t>& o) ;

	// * * * * * * * * * * * * * * * * * 
	Matrix<t> operator *(int o);
	Matrix<t> operator *(float o);
	/** Perfmorms matrix multiplication */
	Matrix<t> operator *(Matrix<t>& o) const;

	/** Perfmorms multiplication of matrix on 2d vector
	*	Matrix has to be width 2 and height at least 2 (all after 2nd row will be ignored)
	*/
	Vec2i operator *(Vec2i& o) const;
	/** Perfmorms multiplication of matrix on 2d vector
	*	Matrix has to be width 2 and height at least 2 (all after 2nd row will be ignored)
	*/
	Vec2f operator *(Vec2f& o) const;
	/** Perfmorms multiplication of matrix on 3d vector
	*	Matrix has to be width 3 and height at least 3 (all after 2nd row will be ignored)
	*/
	Vec3i operator *(Vec3i& o) const;
	/** Perfmorms multiplication of matrix on 3d vector
	*	Matrix has to be width 3 and height at least 3 (all after 2nd row will be ignored)
	*/
	Vec3f operator *(Vec3f& o) const;

	// / / / / / / / / / / / / / / / / / / / / / / / / 
	Matrix<t> operator /(int o) ;
	Matrix<t>& operator /(float o) ;

	// == == == == == == == == == == == ==
	bool operator ==(int o) ;
	bool operator ==(float o) ;
	bool operator ==(Matrix<t>& o) ;

	// != != != != != != != != != != != != != 
	bool operator !=(int o) ;
	bool operator !=(float o) ;
	bool operator !=(Matrix<t>& o) ;

	// += += += += += += += += += += += += += 
	Matrix<t>& operator +=(int o);
	Matrix<t>& operator +=(float o);
	Matrix<t>& operator +=(Matrix<t> o);
	// -= -= -= -= -= -= -= -= -= -= -= -= -= 
	Matrix<t>& operator -=(int o);
	Matrix<t>& operator -=(float o);
	Matrix<t>& operator -=(Matrix<t> o);
	// *= *= *= *= *= *= *= *= *= *= *= *= *= 
	Matrix<t>& operator *=(int o);
	Matrix<t>& operator *=(float o);
	Matrix<t>& operator *=(Matrix<t> o);

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
	inline std::vector<t>  operator [](unsigned int i) const;
	/**
	* Implementation of 1d array [] operator
	* DON'T MIX UP WITH [] OPERATOR, THAT GETS MATRIX ROW
	* This one returns the actual element from values array
	*/
	inline  t operator ()(unsigned int i) const;
	/**
	* Implementation of 2d array [][] operator.
	* Returns matrix element
	*/
	inline t operator () (unsigned int i1, unsigned int i2) const;
};

template <class t> std::ostream& operator <<(std::ostream& s, Matrix<t>& m);

typedef Matrix<int> Mati;
typedef Matrix<float> Matf;
typedef Matrix<Vec2i> Matv2i;
typedef Matrix<Vec3i> Matv3i;
typedef Matrix<Vec2f> Matv2f;
typedef Matrix<Vec3f> Matv3f;


template<class t>
class SquareMatrix : public Matrix<t> {
public:
	SquareMatrix(unsigned int _s, t* _v, bool _transpose = false) : Matrix<t>(_s, _s, _v, _transpose) {};

};


class MatrixMath {
public:
	template<class t> static Matrix<t> concat(Matrix<t>& m1, Matrix<t> m2) {
		for (int i = 0; i < m2.length(); i++) {
			//v.insert(v.begin() + add + i * w, colv[i]);
		}
	}
};

#endif //__MATRIX_H__