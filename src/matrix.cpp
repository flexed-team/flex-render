#include "matrix.h"
#include "geometry.h"

template<class t> Matrix<t>::Matrix(const Matrix<t>& m) : w(m.w), h(m.h), v(m.v.begin(), m.v.end()), transposed(m.transposed) {}
template<class t> Matrix<t>::Matrix(const Matrix<t>& m, t _v[]) : w(m.w), h(m.h), v(_v, _v + m.w * m.h), transposed(m.transposed) {}
template<class t> Matrix<t>::Matrix(const Matrix<t>& m, std::vector<t> _v) : w(m.w), h(m.h), v(_v.begin(), _v.end()), transposed(m.transposed) {}
template<class t> Matrix<t>::Matrix(const Matrix<t>& m, std::vector<t>* _v) : w(m.w), h(m.h), v(_v->begin(), _v->end()), transposed(m.transposed) {}

template<class t> Matrix<t>::Matrix(unsigned _w, unsigned _h, t _defv, bool _transposed) : w(_w), h(_h), v(_w* _h, _defv), transposed(_transposed) {}
template<class t> Matrix<t>::Matrix(unsigned _w, unsigned _h, t _v[], bool _transposed) : w(_w), h(_h), v(_v, _v + _w * _h), transposed(_transposed) {};
template<class t> Matrix<t>::Matrix(unsigned _w, unsigned _h, std::vector<t> _v, bool _transposed) : w(_w), h(_h), v(_v.begin(), _v.end()), transposed(_transposed) {}
template<class t> Matrix<t>::Matrix(unsigned _w, unsigned _h, std::vector<t>* _v, bool _transposed) : w(_w), h(_h), v(_v->begin(), _v->end()), transposed(_transposed) {}


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


template<class t> void Matrix<t>::insert_row(t* rowv, unsigned roww)
{
	assert(g_w() == roww && "Inappropriate row size");
	const unsigned add = g_length();
	for (unsigned i = 0; i < g_w(); i++)
		v.push_back(rowv[i]);

	s_ih();
}

template<class t> void Matrix<t>::insert_col(t* colv, unsigned colh)
{
	assert(g_h() == colh && "Inappropriate col size");
	s_iw();
	const unsigned add = g_w() - 1;
	for (int i = 0; i < add; i++)
		v.insert(v.begin() + add + i * g_w(), colv[i]);
}

template<class t> void Matrix<t>::for_each(std::function<void(t&)> callback)
{
	for (int i = 0; i < g_length(); i++) {
		callback(v[i]);
	}
}


// + + + + + + + + + + + + + + + + + + + 
template<class t> Matrix<t> Matrix<t>::operator +(int o) {
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		_v[i] += o;
	//return Matrix<t>(g_w(), g_h(), _v, g_transposed());
	return Matrix<t>((*this), _v);
}
template<class t> Matrix<t> Matrix<t>::operator +(float o) {
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		_v[i] += o;
	return Matrix<t>(g_w(), g_h(), _v, g_transposed());
}
template<class t> Matrix<t> Matrix<t>::operator +(const Matrix<t>& o) {
	check_sizes(o);
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		_v[i] += o.v[i];
	return Matrix<t>(g_w(), g_h(), _v, g_transposed());
}


// - - - - - - - - - - - - - - - - 
template<class t> Matrix<t> Matrix<t>::operator -() {
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		_v[i] = -_v[i];
	return Matrix<t>(g_w(), g_h(), _v, g_transposed());
}
template<class t> Matrix<t> Matrix<t>::operator -(int o) {
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		_v[i] -= o;
	return Matrix<t>(g_w(), g_h(), _v, g_transposed());
}template<class t> Matrix<t> Matrix<t>::operator -(float o) {
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		_v[i] -= o;
	return Matrix<t>(g_w(), g_h(), _v, g_transposed());
}
template<class t> Matrix<t> Matrix<t>::operator -(const Matrix<t>& o) {
	check_sizes(o);
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		_v[i] -= o.v[i];
	return Matrix<t>(g_w(), g_h(), _v, g_transposed());
}

// * * * * * * * * * * * * * * * * * 
template<class t> Matrix<t> Matrix<t>::operator *(int o)
{
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		_v[i] *= o;
	return Matrix<t>(g_w(), g_h(), _v, g_transposed());
}
template<class t>  Matrix<t> Matrix<t>::operator *(float o)
{
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		_v[i] *= o;
	return Matrix<t>(g_w(), g_h(), _v, g_transposed());
}
template<class t> Matrix<t> Matrix<t>::operator *(Matrix<t>& o)
{
	assert(g_w() == o.g_h() && "Multiply is impossible - origin matrix width is not equal other height");
	Matrix<t> _m = Matrix<t>(o.g_w(), g_h(), std::vector<t>(g_h() * o.g_w()));
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned j = 0; j < o.g_w(); j++)
			for (unsigned k = 0; k < g_w(); k++)
				_m(i, j) += (*this)(i, k) * o(k, j);

	return _m;
}
template<class t> Vec2i Matrix<t>::operator *(const Vec2i& o) const
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
template<class t> Vec3i Matrix<t>::operator *(const Vec3i& o) const
{
	assert(g_w() == 3 && g_h() >= 3 && "Inappropriate matrix dimensions for vector multiplication - width must be 3 and heigth at least 3");
	Vec3i _v = Vec3i();
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned k = 0; k < 3; k++)
			_v.raw[i] += (*this)(i, k) * o.raw[k];

	return _v;
}
template<class t> Vec3f Matrix<t>::operator *(const Vec3f& o) const
{
	assert(g_w() == 3 && g_h() >= 3 && "Inappropriate matrix dimensions for vector multiplication - width must be 3 and heigth at least 3");
	Vec3f _v = Vec3f();
	for (unsigned int i = 0; i < g_h(); i++)
		for (unsigned int k = 0; k < 3; k++) {
			_v.raw[i] += (*this)(i, k) * o.raw[k];
		}

	return _v;
}



// / / / / / / / / / / / / / / / / / / / / / / / / 
template<class t> Matrix<t> Matrix<t>::operator /(int o)
{
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		_v[i] /= o;
	return Matrix<t>(g_w(), g_h(), _v, g_transposed());
}
template<class t> Matrix<t> Matrix<t>::operator /(float o)
{
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		_v[i] /= o;
	return Matrix<t>(g_w(), g_h(), _v, g_transposed());
}
template<class t> Matrix<t> Matrix<t>::operator /(double o)
{
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		_v[i] /= o;
	return Matrix<t>(g_w(), g_h(), _v, g_transposed());
}
template<class t> Matrix<t> Matrix<t>::operator /(long double o)
{
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		_v[i] /= o;
	return Matrix<t>(g_w(), g_h(), _v, g_transposed());
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
template<class t> bool Matrix<t>::operator ==(const Matrix<t>& o)
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
template<class t> bool Matrix<t> ::operator !=(const Matrix<t>& o)
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
template<class t> Matrix<t>& Matrix<t>::operator +=(const Matrix<t>& o) {
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
template<class t> Matrix<t>& Matrix<t>::operator -=(const Matrix<t>& o) {
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
template<class t> Matrix<t>& Matrix<t>::operator *=(const Matrix<t>& o) {
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
		return v[c * w + r];
	return v[r * w + c];
}

template <class t> std::ostream& operator <<(std::ostream& s, const Matrix<t>& m) {
	m.log();
	return s;
}





template<class t> SquareMatrix<t>::SquareMatrix(const Matrix<t>& m) : Matrix<t>(m) {}

template<class t> SquareMatrix<t>::SquareMatrix(const Matrix<t>& m, t _v[]) : Matrix<t>(m, _v) {}
template<class t> SquareMatrix<t>::SquareMatrix(const Matrix<t>& m, std::vector<t> _v) : Matrix<t>(m, _v) {}
template<class t> SquareMatrix<t>::SquareMatrix(const Matrix<t>& m, std::vector<t>* _v) : Matrix<t>(m, _v) {}


template<class t> SquareMatrix<t>::SquareMatrix(unsigned _s, bool _transpose) : Matrix<t>(_s, _s, _transpose) {};
template<class t> SquareMatrix<t>::SquareMatrix(unsigned _s, t _v[], bool _transpose) : Matrix<t>(_s, _s, _v, _transpose) {};
template<class t> SquareMatrix<t>::SquareMatrix(unsigned _s, std::vector<t> _v, bool _transpose) : Matrix<t>(_s, _s, _v, _transpose) {};
template<class t> SquareMatrix<t>::SquareMatrix(unsigned _s, std::vector<t>* _v, bool _transpose) : Matrix<t>(_s, _s, _v, _transpose) {};



template<class t> SquareMatrix<t> SquareMatrix<t>::cut_matrix(unsigned r, unsigned c) {
	std::vector<t> _v = std::vector<t>();
	for (unsigned i = 0; i < g_s(); i++) {
		if (i == r) continue;
		for (unsigned j = 0; j < g_s(); j++) {
			if (j == c) continue;
			_v.push_back((*this)(i, j));
		}
	}
	//return SquareMatrix(g_s() - 1, _v, false);
	return SquareMatrix(g_s() - 1, _v, Matrix<t>::g_transposed());
}

template<class t> long double SquareMatrix<t>::determinant() {
	SquareMatrix<t> A = *this;
	const unsigned size = A.g_s();

	for (unsigned i = 0; i < size - 1; i++) {
		for (unsigned j = i + 1; j < size; j++) {
			for (unsigned k = i + 1; k < size; k++) {
				A(j, k) = (A(j, k) * A(i, i) - A(j, i) * A(i, k));
				if (i) A(j, k) /= A(i - 1, i - 1);
			}
		}
	}

	return A(size - 1, size - 1);
}

template<class t> SquareMatrix<t> SquareMatrix<t>::inverse() {
	long double det = determinant();
	if (det == 0)
		throw MatrixException("Can't inverse: determinant equals zero");

	// Find adjoint matrix
	SquareMatrix<t> _m = (*this);
	for (unsigned i = 0; i < g_s(); i++) {
		for (unsigned j = 0; j < g_s(); j++) {
			_m(i, j) = complement(i, j);
		}
	}
	_m.transpose();
	_m /= det;

	return _m;
}




template<class t> Matrix3<t>::Matrix3(const Matrix<t>& m) : SquareMatrix<t>(m) {}

template<class t> Matrix3<t>::Matrix3(const Matrix<t>& m, t _v[9]) : SquareMatrix<t>(m, _v) {}
template<class t> Matrix3<t>::Matrix3(const Matrix<t>& m, std::vector<t> _v) : SquareMatrix<t>(m, _v) {}
template<class t> Matrix3<t>::Matrix3(const Matrix<t>& m, std::vector<t>* _v) : SquareMatrix<t>(m, _v) {}

template<class t> Matrix3<t>::Matrix3(bool _transpose) : SquareMatrix<t>(3, _transpose) {};
template<class t> Matrix3<t>::Matrix3(t _v[9], bool _transpose) : SquareMatrix<t>(3, _v, _transpose) {};
template<class t> Matrix3<t>::Matrix3(std::vector<t> _v, bool _transpose) : SquareMatrix<t>(3, _v, _transpose) {};
template<class t> Matrix3<t>::Matrix3(std::vector<t>* _v, bool _transpose) : SquareMatrix<t>(3, _v, _transpose) {};



template<class t> Matrix4<t>::Matrix4(const Matrix<t>& m) : SquareMatrix<t>(m) {}

template<class t> Matrix4<t>::Matrix4(const Matrix<t>& m, t _v[16]) : SquareMatrix<t>(m, _v) {}
template<class t> Matrix4<t>::Matrix4(const Matrix<t>& m, std::vector<t> _v) : SquareMatrix<t>(m, _v) {}
template<class t> Matrix4<t>::Matrix4(const Matrix<t>& m, std::vector<t>* _v) : SquareMatrix<t>(m, _v) {}

template<class t> Matrix4<t>::Matrix4(bool _transpose) : SquareMatrix<t>(4, _transpose) {};
template<class t> Matrix4<t>::Matrix4(t _v[16], bool _transpose) : SquareMatrix<t>(4, _v, _transpose) {};
template<class t> Matrix4<t>::Matrix4(std::vector<t> _v, bool _transpose) : SquareMatrix<t>(4, _v, _transpose) {};
template<class t> Matrix4<t>::Matrix4(std::vector<t>* _v, bool _transpose) : SquareMatrix<t>(4, _v, _transpose) {};




template<class t> Vector<t>::Vector(const Matrix<t>& m) : Matrix<t>(m) {}

template<class t> Vector<t>::Vector(const Matrix<t>& m, t _v[]) : Matrix<t>(m, _v) {}
template<class t> Vector<t>::Vector(const Matrix<t>& m, std::vector<t> _v) : Matrix<t>(m, _v) {}
template<class t> Vector<t>::Vector(const Matrix<t>& m, std::vector<t>* _v) : Matrix<t>(m, _v) {}

template<class t> Vector<t>::Vector(unsigned _dim, bool _transpose) : Matrix<t>(1, _dim, _transpose) {};
template<class t> Vector<t>::Vector(unsigned _dim, t _v[], bool _transpose) : Matrix<t>(1, _dim, _v, _transpose) {};
template<class t> Vector<t>::Vector(unsigned _dim, std::vector<t> _v, bool _transpose) : Matrix<t>(1, _dim, _v, _transpose) {};
template<class t> Vector<t>::Vector(unsigned _dim, std::vector<t>* _v, bool _transpose) : Matrix<t>(1, _dim, _v, _transpose) {};


template<class t> Vector3<t>::Vector3(const Matrix<t>& m) : Vector<t>(m) {}

template<class t> Vector3<t>::Vector3(const Matrix<t>& m, t _v[3]) : Vector<t>(m, _v) {}
template<class t> Vector3<t>::Vector3(const Matrix<t>& m, std::vector<t> _v) : Vector<t>(m, _v) {}
template<class t> Vector3<t>::Vector3(const Matrix<t>& m, std::vector<t>* _v) : Vector<t>(m, _v) {}

template<class t> Vector3<t>::Vector3(bool _transpose) : Vector<t>(3, _transpose) {};
template<class t> Vector3<t>::Vector3(t _v[3], bool _transpose) : Vector<t>(3, _v, _transpose) {};
template<class t> Vector3<t>::Vector3(std::vector<t> _v, bool _transpose) : Vector<t>(3, _v, _transpose) {};
template<class t> Vector3<t>::Vector3(std::vector<t>* _v, bool _transpose) : Vector<t>(3, _v, _transpose) {};


template<class t> Vector4<t>::Vector4(const Matrix<t>& m) : Vector<t>(m) {}

template<class t> Vector4<t>::Vector4(const Matrix<t>& m, t _v[4]) : Vector<t>(m, _v) {}
template<class t> Vector4<t>::Vector4(const Matrix<t>& m, std::vector<t> _v) : Vector<t>(m, _v) {}
template<class t> Vector4<t>::Vector4(const Matrix<t>& m, std::vector<t>* _v) : Vector<t>(m, _v) {}

template<class t> Vector4<t>::Vector4(bool _transpose) : Vector<t>(4, _transpose) {};
template<class t> Vector4<t>::Vector4(t _v[4], bool _transpose) : Vector<t>(4, _v, _transpose) {};
template<class t> Vector4<t>::Vector4(std::vector<t> _v, bool _transpose) : Vector<t>(4, _v, _transpose) {};
template<class t> Vector4<t>::Vector4(std::vector<t>* _v, bool _transpose) : Vector<t>(4, _v, _transpose) {};
