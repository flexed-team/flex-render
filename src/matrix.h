#ifndef __MATRIX_H__
#define __MATRIX_H__

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <cassert>
#include <iterator>
#include <xmmintrin.h>

template <class t>
class Matrix
{
public:
	union {
		/// height and width
		struct { int w, h; };
		/// n is count of cols, m is count of rows, 
		struct { int n, m; };
	};

	/// Matrix values array
	std::vector<t> v = std::vector<t>();

	// Fills matrix with zeros
	Matrix(int _w, int _h) : w(_w), h(_h), v(_w * _h, 0) {}
	Matrix(int _w, int _h, t* _v) : w(_w), h(_h), v(_v, _v + _w * _h) {}

	// Methods ------------------------------------

	/// Gets matrix values array size
	inline int length() const { return w * h; }


	/// Inserts row to matrix
	void insert_row(t* rowv, int roww) {
		assert(w == roww);
		const int add = +w * h;
		for (int i = 0; i < w; i++) {
			v.insert(v.begin() + add + i, rowv[i]);
		}
		h++;
	}

	/// Inserts col to matrix
	void insert_col(t* colv, int colh) {
		assert(h == colh);
		w++;
		const int add = w - 1;
		for (int i = 0; i < w; i++) {
			v.insert(v.begin() + add + i * w, colv[i]);
		}
	}

	void log()
	{
		for (int i = 0; i < w * h; i++)
		{
			printf("%5d,", v[i]);
			if (i % w == w - 1) std::cout << "\n";
		}
		std::cout << "\n";
	}


	// Unary minus
	Matrix<t> operator -() {
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] = -_v[i];
		return Matrix<t>(w, h, _v);
	}

	// Int ------------------------------------
	Matrix<t> operator +(int other) {
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] += other;
		return Matrix<t>(w, h, _v);
	}
	Matrix<t> operator -(int other) {
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] -= other;
		return Matrix<t>(w, h, _v);
	}
	Matrix<t> operator *(int other) {
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] *= other;
		return Matrix<t>(w, h, _v);
	}
	Matrix<t> operator /(int other) {
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] /= other;
		return Matrix<t>(w, h, _v);
	}


	// Float ------------------------------------
	Matrix<t> operator +(float other) {
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] += other;
		return Matrix<t>(w, h, _v);
	}
	Matrix<t> operator -(float other) {
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] -= other;
		return Matrix<t>(w, h, _v);
	}
	Matrix<t> operator *(float other) {
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] *= other;
		return Matrix<t>(w, h, _v);
	}
	Matrix<t> operator /(float other) {
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] /= other;
		return Matrix<t>(w, h, _v);
	}

	// Matrix operations ------------------------------------
	Matrix<t> operator +(Matrix<t>& other) {
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] += other.v[i];
		return Matrix<t>(w, h, _v);
	}
	Matrix<t> operator -(Matrix<t>& other) {
		check_sizes(other);
		t* _v = v.data();
		for (int i = 0; i < w * h; i++)
			_v[i] -= other.v[i];
		return Matrix<t>(w, h, _v);
	}
	Matrix<t> operator *(Matrix<t>& other) {
		//check_sizes(other);
		t* _v = new t[h * other.w];
		for (int i = 0; i < h; i++)
			for (int j = 0; j < other.w; j++)
				for (int k = 0; k < w; k++) {
					_v[j + i * other.w] += v[k + w * i] * other.v[j + k * other.w];
				}

		return Matrix<t>(other.w, h, _v);
	}

private:
	/// Checks other matrix for size identity
	/// Uses assert
	inline void check_sizes(Matrix<t>& m) { assert(w == m.w); assert(h == m.h); }

};


typedef  Matrix<int> Mati;
typedef  Matrix<float> Matf;



<<<<<<< HEAD
#endif //__MATRIX_H__
=======
#endif //__MATRIX_H__
>>>>>>> a279524f8a9356dc2746b2c12a68e5c95c1e7ae5
