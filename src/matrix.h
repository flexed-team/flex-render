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

	/**  Fills matrix with zeros */
	Matrix(unsigned int _w, unsigned int _h, bool _transpose = false) : w(_w), h(_h), v(_w* _h, 0), transpose(_transpose) {}
	/**  Matrix from pointer to array */
	Matrix(unsigned int _w, unsigned int _h, t* _v, bool _transpose = false) : w(_w), h(_h), v(_v, _v + _w * _h), transpose(_transpose) {}
	/**  Matrix from vector */
	Matrix(unsigned int _w, unsigned  int _h, std::vector<t> _v, bool _transpose = false) : w(_w), h(_h), v(_v.begin(), _v.end()), transpose(_transpose) {}
	/**  Matrix from pointer to vector */
	Matrix(unsigned int _w, unsigned  int _h, std::vector<t>* _v, bool _transpose = false) : w(_w), h(_h), v(_v->begin(), _v->end()), transpose(_transpose) {}

	// Methods ------------------------------------

	/** Gets matrix values array size */
	inline int length() const { return w * h; }

	/** Inserts row to matrix */
	void insert_row(t* rowv, int roww)
	{
		assert(w == roww);
		const unsigned int add = +w * h;
		for (int i = 0; i < w; i++)
		{
			v.insert(v.begin() + add + i, rowv[i]);
		}
		h++;
	}

	/** Inserts col to matrix */
	void insert_col(t* colv, int colh)
	{
		assert(h == colh);
		w++;
		const unsigned int add = w - 1;
		for (int i = 0; i < w; i++)
		{
			v.insert(v.begin() + add + i * w, colv[i]);
		}
	}

	void log()
	{
		if (transpose)
			for (unsigned int i = 0; i < w * h;)
			{
				std::cout << std::setw(5) << v[i];
				if (i / w == h - 1) {
					std::cout << "\n";
					if (i == w * h - 1) break;
					i = i % w + 1;
				}
				else
					i += w;
			}
		else
			for (unsigned int i = 0; i < w * h; i++)
			{
				std::cout << std::setw(5) << v[i];
				if (i % w == w - 1)
					std::cout << "\n";
			}
		std::cout << "\n";
	}

	/** Unary minus */
	Matrix<t> operator-()
	{
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] = -_v[i];
		return Matrix<t>(w, h, _v);
	}

	// Int ------------------------------------
	Matrix<t> operator+(int other)
	{
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] += other;
		return Matrix<t>(w, h, _v);
	}
	Matrix<t> operator-(int other)
	{
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] -= other;
		return Matrix<t>(w, h, _v);
	}
	Matrix<t> operator*(int other)
	{
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] *= other;
		return Matrix<t>(w, h, _v);
	}
	Matrix<t> operator/(int other)
	{
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] /= other;
		return Matrix<t>(w, h, _v);
	}

	// Float ------------------------------------
	Matrix<t> operator+(float other)
	{
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] += other;
		return Matrix<t>(w, h, _v);
	}
	Matrix<t> operator-(float other)
	{
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] -= other;
		return Matrix<t>(w, h, _v);
	}
	Matrix<t> operator*(float other)
	{
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] *= other;
		return Matrix<t>(w, h, _v);
	}
	Matrix<t> operator/(float other)
	{
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] /= other;
		return Matrix<t>(w, h, _v);
	}

	// Matrix operations ------------------------------------
	Matrix<t> operator+(Matrix<t>& other)
	{
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] += other.v[i];
		return Matrix<t>(w, h, _v);
	}
	Matrix<t> operator-(Matrix<t>& other)
	{
		check_sizes(other);
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] -= other.v[i];
		return Matrix<t>(w, h, _v);
	}
	/** Perfmorms matrix multiplication */
	Matrix<t> operator*(Matrix<t>& other)
	{
		assert(w == other.h);
		std::vector<t> _v = std::vector<t>(h * other.w);
		for (unsigned int i = 0; i < h; i++)
			for (unsigned int j = 0; j < other.w; j++)
				for (unsigned int k = 0; k < w; k++) {
					_v[j + i * other.w] += v[i * w + k] * other.v[k * other.w + j];
				}

		return Matrix<t>(other.w, h, &_v);
	}

private:
	/** 
	* Checks other matrix for size identity
	* Uses assert
	*/
	inline void check_sizes(Matrix<t>& m)
	{
		assert(w == m.w);
		assert(h == m.h);
	}
};

template <class t> std::ostream& operator<<(std::ostream& s, Matrix<t>& m) {
	m.log();
	return s;
}

typedef Matrix<int> Mati;
typedef Matrix<float> Matf;
typedef Matrix<Vec2i> Matv2i;
typedef Matrix<Vec3i> Matv3i;
typedef Matrix<Vec2f> Matv2f;
typedef Matrix<Vec3f> Matv3f;



#endif //__MATRIX_H__