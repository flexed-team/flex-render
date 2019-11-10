#include "matrix.h"
#include "geometry.h"

template<class t> Matrix<t>::Matrix(const Matrix<t>& m, bool _transpose) : w(m.w), h(m.h), v(m.v.begin(), m.v.end()), transposed(m.transposed) {}
template<class t> Matrix<t>::Matrix(unsigned _w, unsigned _h, t _defv, bool _transposed) : w(_w), h(_h), v(_w* _h, _defv), transposed(_transposed) {}
template<class t> Matrix<t>::Matrix(unsigned _w, unsigned _h, t* _v, bool _transposed) : w(_w), h(_h), v(_v, _v + _w * _h), transposed(_transposed) {};
template<class t> Matrix<t>::Matrix(unsigned _w, unsigned _h, std::vector<t> _v, bool _transposed) : w(_w), h(_h), v(_v.begin(), _v.end()), transposed(_transposed) {}
template<class t> Matrix<t>::Matrix(unsigned _w, unsigned _h, std::vector<t>* _v, bool _transposed) : w(_w), h(_h), v(_v->begin(), _v->end()), transposed(_transposed) {}


template<class t> void Matrix<t>::check_sizes(Matrix<t>& m) const
{
	assert(g_w() == m.g_w());
	assert(g_h() == m.g_h());
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


template<class t> void Matrix<t>::insert_row(t* rowv, int roww)
{
	assert(g_w() == roww && "Inappropriate row size");
	const unsigned add = g_length();
	for (int i = 0; i < g_w(); i++)
		v.push_back(rowv[i]);

	s_ih();
}

template<class t> void Matrix<t>::insert_col(t* colv, int colh)
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
	return Matrix<t>(g_w(), g_h(), _v, g_transposed());
}
template<class t> Matrix<t> Matrix<t>::operator +(float o) {
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		_v[i] += o;
	return Matrix<t>(g_w(), g_h(), _v, g_transposed());
}
template<class t> Matrix<t> Matrix<t>::operator +(Matrix<t>& o) {
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
template<class t> Matrix<t> Matrix<t>::operator -(Matrix<t>& o) {
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
template<class t> Vec2i Matrix<t>::operator *(Vec2i& o) const
{
	assert(g_w() == 2 && g_h() >= 2 && "Inappropriate matrix dimensions for vector multiplication - width must be 2 and heigth at least 2");
	Vec2i _v = Vec2i();
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned k = 0; k < 2; k++)
			_v.raw[i] += (*this)(i, k) * o.raw[k];

	return _v;
}
template<class t> Vec2f Matrix<t>::operator *(Vec2f& o) const
{
	assert(g_w() == 2 && g_h() >= 2 && "Inappropriate matrix dimensions for vector multiplication - width must be 2 and heigth at least 2");
	Vec2f _v = Vec2f();
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned k = 0; k < 2; k++)
			_v.raw[i] += (*this)(i, k) * o.raw[k];

	return _v;
}
template<class t> Vec3i Matrix<t>::operator *(Vec3i& o) const
{
	assert(g_w() == 3 && g_h() >= 3 && "Inappropriate matrix dimensions for vector multiplication - width must be 3 and heigth at least 3");
	Vec3i _v = Vec3i();
	for (unsigned i = 0; i < g_h(); i++)
		for (unsigned k = 0; k < 3; k++)
			_v.raw[i] += (*this)(i, k) * o.raw[k];

	return _v;
}
template<class t> Vec3f Matrix<t>::operator *(Vec3f& o) const
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
// TODO: rewrite == to support transpose
template<class t> bool Matrix<t>::operator ==(int o)
{
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		if (_v[i] != o) return false;
	return true;
}
template<class t> bool Matrix<t>::operator ==(float o)
{
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		if (_v[i] != o) return false;
	return true;
}
template<class t> bool Matrix<t>::operator ==(Matrix<t>& o)
{
	check_sizes(o);
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		if (_v[i] != o.v[i]) return false;
	return true;
}


// != != != != != != != != != != != != !=
template<class t> bool Matrix<t>::operator !=(int o)
{
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		if (_v[i] == o) return false;
	return true;
}
template<class t> bool Matrix<t> ::operator !=(float o)
{
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		if (_v[i] == o) return false;
	return true;
}
template<class t> bool Matrix<t> ::operator !=(Matrix<t>& o)
{
	check_sizes(o);
	t* _v = v.data();
	for (int i = 0; i < g_length(); i++)
		if (_v[i] == o.v[i]) return false;
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
template<class t> Matrix<t>& Matrix<t>::operator +=(Matrix<t> o) {
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
template<class t> Matrix<t>& Matrix<t>::operator -=(Matrix<t> o) {
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
template<class t> Matrix<t>& Matrix<t>::operator *=(Matrix<t> o) {
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
	++* this; // idk, just copied it from here https://docs.microsoft.com/ru-ru/cpp/cpp/increment-and-decrement-operator-overloading-cpp?view=vs-2019
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
{
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

template <class t> std::ostream& operator <<(std::ostream& s, Matrix<t>& m) {
	m.log();
	return s;
}
