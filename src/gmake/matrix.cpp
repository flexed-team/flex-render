//#include "matrix.h"
#include "geometry.h"

template<class t> Matrix<t>::~Matrix() {
	delete[] v;
}
template<class t> Matrix<t>::Matrix() : v{}, transposed(false) {};

template<class t> Matrix<t>::Matrix(const Matrix<t>& m) : width(m.width), height(m.height), transposed(m.transposed) {
	v = new t[m.g_length()];
	std::copy(m.v, m.v + m.g_length(), v);
}
template<class t> Matrix<t>::Matrix(const Matrix<t>& m, t _v[]) : width(m.width), height(m.height), transposed(m.transposed) {
	v = new t[m.g_length()];
	std::copy(_v, _v + m.g_length(), v);
}
template<class t> Matrix<t>::Matrix(const Matrix<t>& m, const std::vector<t>& _v) : width(m.width), height(m.height), transposed(m.transposed) {
	v = new t[m.g_length()];
	std::copy(_v.begin(), _v.end(), v);
}
template<class t> Matrix<t>::Matrix(const Matrix<t>& m, std::vector<t>* _v) : width(m.width), height(m.height), transposed(m.transposed) {
	v = new t[m.g_length()];
	std::copy(_v->begin(), _v->end(), v);
}

template<class t> Matrix<t>::Matrix(unsigned _w, unsigned _h, bool _t) : width(_w), height(_h), transposed(_t) {
	v = new t[_w * _h];
}
template<class t> Matrix<t>::Matrix(unsigned _w, unsigned _h, t _defv, bool _t) : width(_w), height(_h), transposed(_t) {
	v = new t[_w * _h];
	std::fill(v, v + _w * _h, _defv);
}
template<class t> Matrix<t>::Matrix(unsigned _w, unsigned _h, t _v[], bool _t) : width(_w), height(_h), transposed(_t) {
	v = new t[_w * _h];
	std::copy(_v, _v + _w * _h, v);
};
template<class t> Matrix<t>::Matrix(unsigned _w, unsigned _h, const std::vector<t>& _v, bool _t) : width(_w), height(_h), transposed(_t) {
	v = new t[_w * _h];
	std::copy(_v.begin(), _v.end(), v);
}
template<class t> Matrix<t>::Matrix(unsigned _w, unsigned _h, std::vector<t>* _v, bool _t) : width(_w), height(_h), transposed(_t) {
	v = new t[_w * _h];
	std::copy(_v->begin(), _v->end(), v);
}

template<class t> Matrix<t> Matrix<t>::identity(int dimensions)
{
	Matrix<t> M(dimensions, dimensions);
	for (int i = 0; i < dimensions; ++i)
	{
		for (int j = 0; j < dimensions; ++j)
			M(i, j) = (i == j ? 1.f : 0.f);
	}

	return M;
}

template<class t> inline Vec3f Matrix<t>::toVec3f()
{
	assert(g_w() == 1 && g_h() == 4);
	return Vec3f((*this)(0, 0) / (*this)(0, 3), (*this)(0, 1) / (*this)(0, 3), (*this)(0, 2) / (*this)(0, 3));
}


template<class t> void Matrix<t>::check_sizes(Matrix<t>& m) const
{
	assert(g_w() == m.g_w() && g_h() == m.g_h());
}

template<class t> void Matrix<t>::log()
{
	for (unsigned i = 0; i < g_h(); i++) {
		for (unsigned j = 0; j < g_w(); j++)
		{
			std::cout << std::setw(5) << (*this)(i, j) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


template<class t> void Matrix<t>::insert_row(t* row_values, unsigned row_width)
{
	assert(g_w() == row_width && "Inappropriate row size");

	const unsigned offset = g_length();

	t* _v = new t[offset + row_width];
	std::copy(v, v + offset, _v);
	delete[] v;
	v = _v;

	for (unsigned i = 0; i < row_width; i++)
		v[offset + i] = row_values[i];

	s_inc_h();
}

template<class t> void Matrix<t>::insert_col(t* col_values, unsigned col_height)
{
	assert(g_h() == col_height && "Inappropriate col size");
	s_inc_w();

	const unsigned new_length = g_length() + col_height;
	t* _v = new t[new_length];

	int c = 0;
	for (unsigned i = 0; i < new_length; i++)
		if (i % g_w() == g_w() - 1) {
			_v[i] = col_values[c];
			c++;
		}
		else  _v[i] = v[i - c];

	delete[] v;
	v = _v;
}

template<class t> void Matrix<t>::for_each(std::function<void(t&)> callback)
{
	for (int i = 0; i < g_length(); i++)
		callback(v[i]);
}

// = = = = = = = = = = = = = 
template<class t> Matrix<t>& Matrix<t>::operator =(const Matrix<t>& o) {
	width = o.width;
	height = o.height;
	transposed = o.transposed;

	delete[] v;
	v = new t[o.g_length()];
	std::copy(o.v, o.v + o.g_length(), v);

	return *this;
}

// + + + + + + + + + + + + + + + + + + + 
template<class t> Matrix<t> Matrix<t>::operator +(int o) {
	t* _v = new t[g_length()];
	std::copy(v, v + g_length(), _v);
	for (int i = 0; i < g_length(); i++)
		_v[i] += o;
	return Matrix<t>((*this), _v);
}
template<class t> Matrix<t> Matrix<t>::operator +(float o) {
	t* _v = new t[g_length()];
	std::copy(v, v + g_length(), _v);
	for (int i = 0; i < g_length(); i++)
		_v[i] += o;
	return Matrix<t>((*this), _v);
}
template<class t> Matrix<t> Matrix<t>::operator +(Matrix<t>& o) {
	check_sizes(o);
	t* _v = new t[g_length()];
	std::copy(v, v + g_length(), _v);
	for (int i = 0; i < g_length(); i++)
		_v[i] += o.v[i];
	return Matrix<t>((*this), _v);
}


// - - - - - - - - - - - - - - - - 
template<class t> Matrix<t> Matrix<t>::operator -() {
	t* _v = new t[g_length()];
	std::copy(v, v + g_length(), _v);
	for (int i = 0; i < g_length(); i++)
		_v[i] = -_v[i];
	return Matrix<t>((*this), _v);
}
template<class t> Matrix<t> Matrix<t>::operator -(int o) {
	t* _v = new t[g_length()];
	std::copy(v, v + g_length(), _v);
	for (int i = 0; i < g_length(); i++)
		_v[i] -= o;
	return Matrix<t>((*this), _v);
}
template<class t> Matrix<t> Matrix<t>::operator -(float o) {
	t* _v = new t[g_length()];
	std::copy(v, v + g_length(), _v);
	for (int i = 0; i < g_length(); i++)
		_v[i] -= o;
	return Matrix<t>((*this), _v);
}
template<class t> Matrix<t> Matrix<t>::operator -(Matrix<t>& o) {
	check_sizes(o);
	t* _v = new t[g_length()];
	std::copy(v, v + g_length(), _v);
	for (int i = 0; i < g_length(); i++)
		_v[i] -= o.v[i];
	return Matrix<t>((*this), _v);
}

// * * * * * * * * * * * * * * * * * 
template<class t> Matrix<t> Matrix<t>::operator *(int o)
{
	t* _v = new t[g_length()];
	std::copy(v, v + g_length(), _v);
	for (int i = 0; i < g_length(); i++)
		_v[i] *= o;
	return Matrix<t>((*this), _v);
}
template<class t>  Matrix<t> Matrix<t>::operator *(float o)
{
	t* _v = new t[g_length()];
	std::copy(v, v + g_length(), _v);
	for (int i = 0; i < g_length(); i++)
		_v[i] *= o;
	return Matrix<t>((*this), _v);
}
template<class t> Matrix<t> Matrix<t>::operator *(Matrix<t>& o)
{
	assert(g_w() == o.g_h() && "Multiply is impossible - origin matrix width is not equal other height");
	Matrix<t> _m = Matrix<t>(o.g_w(), g_h(), t());
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned j = 0; j < o.g_w(); j++)
			for (unsigned k = 0; k < g_w(); k++)
				_m(i, j) += (*this)(i, k) * o(k, j);

	return _m;
}
template<class t> Vec2i Matrix<t>::operator *(const Vec2i& o) 
{
	assert(g_w() == 2 && g_h() >= 2 && "Inappropriate matrix dimensions for vector multiplication - width must be 2 and heigth at least 2");
	Vec2i _v = Vec2i();
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned k = 0; k < 2; k++)
			_v.raw[i] += (*this)(i, k) * o.raw[k];

	return _v;
}
template<class t> Vec2f Matrix<t>::operator *(const Vec2f& o) const
{
	assert(g_w() == 2 && g_h() >= 2 && "Inappropriate matrix dimensions for vector multiplication - width must be 2 and heigth at least 2");
	Vec2f _v = Vec2f();
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned k = 0; k < 2; k++)
			_v.raw[i] += (*this)(i, k) * o.raw[k];

	return _v;
}
template<class t> Vec3i Matrix<t>::operator *(const Vec3i& o)
{
	assert(g_w() == 3 && g_h() >= 3 && "Inappropriate matrix dimensions for vector multiplication - width must be 3 and heigth at least 3");
	Vec3i _v = Vec3i();
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned k = 0; k < 3; k++)
			_v.raw[i] += (*this)(i, k) * o.raw[k];

	return _v;
}
template<class t> Vec3f Matrix<t>::operator *(const Vec3f& o)
{
	assert(g_w() == 3 && g_h() >= 3 && "Inappropriate matrix dimensions for vector multiplication - width must be 3 and heigth at least 3");
	Vec3f _v = Vec3f();
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned k = 0; k < 3; k++) {
			_v.raw[i] += (*this)(i, k) * o.raw[k];
		}

	return _v;
}



// / / / / / / / / / / / / / / / / / / / / / / / / 
template<class t> Matrix<t> Matrix<t>::operator /(int o)
{
	t* _v = new t[g_length()];
	std::copy(v, v + g_length(), _v);
	for (unsigned i = 0; i < g_length(); i++)
		_v[i] /= o;
	return Matrix<t>((*this), _v);
}
template<class t> Matrix<t> Matrix<t>::operator /(float o)
{
	t* _v = new t[g_length()];
	std::copy(v, v + g_length(), _v);
	for (unsigned i = 0; i < g_length(); i++)
		_v[i] /= o;
	return Matrix<t>((*this), _v);
}
template<class t> Matrix<t> Matrix<t>::operator /(double o)
{
	t* _v = new t[g_length()];
	std::copy(v, v + g_length(), _v);
	for (unsigned i = 0; i < g_length(); i++)
		_v[i] /= o;
	return Matrix<t>((*this), _v);
}
template<class t> Matrix<t> Matrix<t>::operator /(long double o)
{
	t* _v = new t[g_length()];
	std::copy(v, v + g_length(), _v);
	for (unsigned i = 0; i < g_length(); i++)
		_v[i] /= o;
	return Matrix<t>((*this), _v);
}


// == == == == == == == == == == == 
template<class t> bool Matrix<t>::operator ==(int o)
{
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned j = 0; j < g_w(); j++)
			if ((*this)(i, j) != o) return false;
	return true;
}
template<class t> bool Matrix<t>::operator ==(float o)
{
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned j = 0; j < g_w(); j++)
			if ((*this)(i, j) != o) return false;
	return true;
}
template<class t> bool Matrix<t>::operator ==(Matrix<t>& o)
{
	check_sizes(o);
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned j = 0; j < g_w(); j++)
			if ((*this)(i, j) != o(i, j)) return false;
	return true;
}


// != != != != != != != != != != != != !=
template<class t> bool Matrix<t>::operator !=(int o)
{
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned j = 0; j < g_w(); j++)
			if ((*this)(i, j) == o) return false;
	return true;
}
template<class t> bool Matrix<t> ::operator !=(float o)
{
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned j = 0; j < g_w(); j++)
			if ((*this)(i, j) == o) return false;
	return true;
}
template<class t> bool Matrix<t> ::operator !=(Matrix<t>& o)
{
	check_sizes(o);
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned j = 0; j < g_w(); j++)
			if ((*this)(i, j) == o(i, j)) return false;
	return true;
}


// += += += += += += += += += += += += += 
template<class t> Matrix<t>& Matrix<t>::operator +=(int o) {
	*this = *this + o;
	return *this;
}
template<class t> Matrix<t>& Matrix<t>::operator +=(float o) {
	*this = *this + o;
	return *this;
}
template<class t> Matrix<t>& Matrix<t>::operator +=(Matrix<t>& o) {
	*this = *this + o;
	return *this;
}

// -= -= -= -= -= -= -= -= -= -= -= -= -= 
template<class t> Matrix<t>& Matrix<t>::operator -=(int o) {
	*this = *this - o;
	return *this;
}
template<class t> Matrix<t>& Matrix<t>::operator -=(float o) {
	*this = *this - o;
	return *this;
}
template<class t> Matrix<t>& Matrix<t>::operator -=(Matrix<t>& o) {
	*this = *this - o;
	return *this;
}

// *= *= *= *= *= *= *= *= *= *= *= *= *= 
template<class t> Matrix<t>& Matrix<t>::operator *=(int o) {
	*this = *this * o;
	return *this;
}
template<class t> Matrix<t>& Matrix<t>::operator *=(float o) {
	*this = *this * o;
	return *this;
}
template<class t> Matrix<t>& Matrix<t>::operator *=(Matrix<t>& o) {
	*this = *this * o;
	return *this;
}
// /= /= /= /= /= /= /= /= /= /= /= /= 
template<class t> Matrix<t>& Matrix<t>::operator /=(int o) {
	*this = *this / o;
	return *this;
}
template<class t> Matrix<t>& Matrix<t>::operator /=(float o) {
	*this = *this / o;
	return *this;
}
template<class t> Matrix<t>& Matrix<t>::operator /=(double o) {
	*this = *this / o;
	return *this;
}
template<class t> Matrix<t>& Matrix<t>::operator /=(long double o) {
	*this = *this / o;
	return *this;
}

//  ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++ ++
template<class t> Matrix<t>& Matrix<t>::operator ++() {
	*this += 1;
	return *this;
}
template<class t> Matrix<t> Matrix<t>::operator ++(int) {
	Matrix<t> temp = *this;
	++* this; // copied it from here https://docs.microsoft.com/ru-ru/cpp/cpp/increment-and-decrement-operator-overloading-cpp?view=vs-2019
	return temp;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
template<class t> Matrix<t>& Matrix<t>::operator --() {
	*this -= 1;
	return *this;
}

template<class t> Matrix<t> Matrix<t>::operator --(int) {
	Matrix<t> temp = *this;
	--* this;
	return temp;
}


// Get elements operators
template<class t> std::vector<t> Matrix<t>::operator [](unsigned i) const
{ // TODO: maybe return iterator?
	return std::vector<t>(v.begin() + i * g_w(), v.begin() + (i + 1) * g_w());
}
template<class t> t& Matrix<t>::operator ()(unsigned i)
{
	return v[i];
}
template<class t> t& Matrix<t>::operator ()(unsigned r, unsigned c)
{
	if (transposed)
		return v[c * width + r];
	return v[r * width + c];
}

template <class t> std::ostream& operator <<(std::ostream& s, Matrix<t>& m) {
	m.log();
	return s;
}





template<class t> SquareMatrix<t>::SquareMatrix() : Matrix<t>() {};
template<class t> SquareMatrix<t>::SquareMatrix(const Matrix<t>& m) : Matrix<t>(m) {}

template<class t> SquareMatrix<t>::SquareMatrix(const Matrix<t>& m, t _v[]) : Matrix<t>(m, _v) {}
template<class t> SquareMatrix<t>::SquareMatrix(const Matrix<t>& m, const std::vector<t>& _v) : Matrix<t>(m, _v) {}
template<class t> SquareMatrix<t>::SquareMatrix(const Matrix<t>& m, std::vector<t>* _v) : Matrix<t>(m, _v) {}


template<class t> SquareMatrix<t>::SquareMatrix(unsigned _s, bool _t) : Matrix<t>(_s, _s, _t) {}
template<class t> SquareMatrix<t>::SquareMatrix(unsigned _s, t _defv, bool _t) : Matrix<t>(_s, _s, _defv, _t) {}
template<class t> SquareMatrix<t>::SquareMatrix(unsigned _s, t _v[], bool _t) : Matrix<t>(_s, _s, _v, _t) {}
template<class t> SquareMatrix<t>::SquareMatrix(unsigned _s, std::vector<t>& _v, bool _t) : Matrix<t>(_s, _s, _v, _t) {}
template<class t> SquareMatrix<t>::SquareMatrix(unsigned _s, std::vector<t>* _v, bool _t) : Matrix<t>(_s, _s, _v, _t) {}



template<class t> SquareMatrix<t> SquareMatrix<t>::cut_matrix(unsigned r, unsigned c) {
	std::vector<t> _v = std::vector<t>();
	for (unsigned i = 0; i < g_s(); i++) {
		if (i == r) continue;
		for (unsigned j = 0; j < g_s(); j++) {
			if (j == c) continue;
			_v.push_back((*this)(i, j));
		}
	}

	return SquareMatrix(g_s() - 1, _v, Matrix<t>::g_transposed());
}

template<class t> long double SquareMatrix<t>::determinant() {
	SquareMatrix<t> A = *this;
	const unsigned size = A.g_s();

	bool change_sign = false;
	bool swapped = false;
	for (unsigned i = 0; i < size - 1; i++) {
		//if(i)std::cout << "fwqfwq" << A(i, i) << " " << i << std::endl;
		if (i < g_s() - 2 && A(i, i) == 0) { // All inside this if condition is needed to fix zero-division
			if (A(i + 1, i) != 0) { // Swap with next row if the value under is non-zero
				A.swap_rows(i, i + 1);
				swapped = true;
			}
			else for (unsigned l = 0; l < A.height; l++) { // Else try to find non-zero elemnt in column and swap with it
				if (A(l, i) != 0) {
					A.swap_rows(i, l);
					swapped = true;
					break;
				}
			}

			if (!swapped)
				return 0;
			else swapped = false;

			// By determinate property: when you swap column or row with another the sign of determinate will change to an opposite
			change_sign = !change_sign;
		}

		for (unsigned j = i + 1; j < size; j++)
			for (unsigned k = i + 1; k < size; k++) {
				A(j, k) = (A(j, k) * A(i, i) - A(j, i) * A(i, k));
				if (i) A(j, k) /= A(i - 1, i - 1); // Don't divide on first iteration
			}
	}

	return change_sign ? -1 * A(size - 1, size - 1) : A(size - 1, size - 1);
}

template<class t> SquareMatrix<t> SquareMatrix<t>::inverse() {
	long double det = determinant();
	if (det == 0)
		throw MatrixException("Can't inverse: determinant equals zero");

	// Find adjoint matrix
	SquareMatrix<t> A = (*this);
	for (unsigned i = 0; i < g_s(); i++) {
		for (unsigned j = 0; j < g_s(); j++) {
			A(i, j) = complement(i, j);
		}
	}
	A.transpose();
	A /= det;

	return A;
}




template<class t> Matrix3<t>::Matrix3() : SquareMatrix<t>() {}
template<class t> Matrix3<t>::Matrix3(const Matrix<t>& m) : SquareMatrix<t>(m) {}

template<class t> Matrix3<t>::Matrix3(const Matrix<t>& m, t _v[9]) : SquareMatrix<t>(m, _v) {}
template<class t> Matrix3<t>::Matrix3(const Matrix<t>& m, const std::vector<t>& _v) : SquareMatrix<t>(m, _v) {}
template<class t> Matrix3<t>::Matrix3(const Matrix<t>& m, std::vector<t>* _v) : SquareMatrix<t>(m, _v) {}

template<class t> Matrix3<t>::Matrix3(t _defv, bool _t) : SquareMatrix<t>(3, _defv, _t) {};
template<class t> Matrix3<t>::Matrix3(t _v[9], bool _t) : SquareMatrix<t>(3, _v, _t) {};
template<class t> Matrix3<t>::Matrix3(std::vector<t>& _v, bool _t) : SquareMatrix<t>(3, _v, _t) {};
template<class t> Matrix3<t>::Matrix3(std::vector<t>* _v, bool _t) : SquareMatrix<t>(3, _v, _t) {};
template<class t> Matrix3<t>::Matrix3(
	t e0, t e1, t e2,
	t e3, t e4, t e5,
	t e6, t e7, t e8,
	bool _t
) {
	SquareMatrix<t>::width = 3;
	SquareMatrix<t>::height = 3;
	SquareMatrix<t>::transposed = _t;
	SquareMatrix<t>::v = new t[9];
	SquareMatrix<t>::v[0] = e0;
	SquareMatrix<t>::v[1] = e1;
	SquareMatrix<t>::v[2] = e2;
	SquareMatrix<t>::v[3] = e3;
	SquareMatrix<t>::v[4] = e4;
	SquareMatrix<t>::v[5] = e5;
	SquareMatrix<t>::v[6] = e6;
	SquareMatrix<t>::v[7] = e7;
	SquareMatrix<t>::v[8] = e8;
};


template<class t> Matrix4<t>::Matrix4() : SquareMatrix<t>() {}
template<class t> Matrix4<t>::Matrix4(const Matrix<t>& m) : SquareMatrix<t>(m) {}

template<class t> Matrix4<t>::Matrix4(const Matrix<t>& m, t _v[16]) : SquareMatrix<t>(m, _v) {}
template<class t> Matrix4<t>::Matrix4(const Matrix<t>& m, const std::vector<t>& _v) : SquareMatrix<t>(m, _v) {}
template<class t> Matrix4<t>::Matrix4(const Matrix<t>& m, std::vector<t>* _v) : SquareMatrix<t>(m, _v) {}

template<class t> Matrix4<t>::Matrix4(t _defv, bool _t) : SquareMatrix<t>(4, _defv, _t) {};
template<class t> Matrix4<t>::Matrix4(t _v[16], bool _t) : SquareMatrix<t>(4, _v, _t) {};
template<class t> Matrix4<t>::Matrix4(std::vector<t>& _v, bool _t) : SquareMatrix<t>(4, _v, _t) {};
template<class t> Matrix4<t>::Matrix4(std::vector<t>* _v, bool _t) : SquareMatrix<t>(4, _v, _t) {};
template<class t> Matrix4<t>::Matrix4(
	t e0, t e1, t e2, t e3,
	t e4, t e5, t e6, t e7,
	t e8, t e9, t e10, t e11,
	t e12, t e13, t e14, t e15,
	bool _t
) {
	SquareMatrix<t>::width = 4;
	SquareMatrix<t>::height = 4;
	SquareMatrix<t>::transposed = _t;
	SquareMatrix<t>::v = new t[16];
	SquareMatrix<t>::v[0] = e0;
	SquareMatrix<t>::v[1] = e1;
	SquareMatrix<t>::v[2] = e2;
	SquareMatrix<t>::v[3] = e3;
	SquareMatrix<t>::v[4] = e4;
	SquareMatrix<t>::v[5] = e5;
	SquareMatrix<t>::v[6] = e6;
	SquareMatrix<t>::v[7] = e7;
	SquareMatrix<t>::v[8] = e8;
	SquareMatrix<t>::v[9] = e9;
	SquareMatrix<t>::v[10] = e10;
	SquareMatrix<t>::v[11] = e11;
	SquareMatrix<t>::v[12] = e12;
	SquareMatrix<t>::v[13] = e13;
	SquareMatrix<t>::v[14] = e14;
	SquareMatrix<t>::v[15] = e15;
};