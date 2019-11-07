#include "matrix.h"

template<class t> Matrix<t>::Matrix(unsigned int _w, unsigned int _h, t _defv, bool _transpose) : w(_w), h(_h), v(_w* _h, _defv), transpose(_transpose) {}
template<class t> Matrix<t>::Matrix(unsigned int _w, unsigned int _h, t* _v, bool _transpose) : w(_w), h(_h), v(_v, _v + _w * _h), transpose(_transpose) {};
template<class t> Matrix<t>::Matrix(unsigned int _w, unsigned  int _h, std::vector<t> _v, bool _transpose) : w(_w), h(_h), v(_v.begin(), _v.end()), transpose(_transpose) {}
template<class t> Matrix<t>::Matrix(unsigned int _w, unsigned  int _h, std::vector<t>* _v, bool _transpose) : w(_w), h(_h), v(_v->begin(), _v->end()), transpose(_transpose) {}



template<class t> void Matrix<t>::log()
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


template<class t> void Matrix<t>::insert_row(t* rowv, int roww)
{
	assert(w == roww && "Inappropriate row size");
	const unsigned int add = +w * h;
	for (int i = 0; i < w; i++)
	{
		v.insert(v.begin() + add + i, rowv[i]);
	}
	h++;
}

template<class t> void Matrix<t>::insert_col(t* colv, int colh)
{
	assert(h == colh && "Inappropriate col size");
	w++;
	const unsigned int add = w - 1;
	for (int i = 0; i < w; i++)
	{
		v.insert(v.begin() + add + i * w, colv[i]);
	}
}


// + + + + + + + + + + + + + + + + + + + 

template<class t> Matrix<t> Matrix<t>::operator +(int o) {
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		_v[i] += o;
	return Matrix<t>(w, h, _v);
}
template<class t> Matrix<t> Matrix<t>::operator +(float o) {
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		_v[i] += o;
	return Matrix<t>(w, h, _v);
}
template<class t> Matrix<t> Matrix<t>::operator +(Matrix<t>& o) {
	check_sizes(o);
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		_v[i] += o.v[i];
	return Matrix<t>(w, h, _v);
}


// - - - - - - - - - - - - - - - - 

template<class t> Matrix<t> Matrix<t>::operator -() {
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		_v[i] = -_v[i];
	return Matrix<t>(w, h, _v);
}
template<class t> Matrix<t> Matrix<t>::operator -(int o) {
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		_v[i] -= o;
	return Matrix<t>(w, h, _v);
}template<class t> Matrix<t> Matrix<t>::operator -(float o) {
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		_v[i] -= o;
	return Matrix<t>(w, h, _v);
}
template<class t> Matrix<t> Matrix<t>::operator -(Matrix<t>& o) {
	check_sizes(o);
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		_v[i] -= o.v[i];
	return Matrix<t>(w, h, _v);
}

// * * * * * * * * * * * * * * * * * 
template<class t> Matrix<t> Matrix<t>::operator *(int o)
{
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		_v[i] *= o;
	return Matrix<t>(w, h, _v);
}
template<class t>  Matrix<t> Matrix<t>::operator *(float o)
{
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		_v[i] *= o;
	return Matrix<t>(w, h, _v);
}
template<class t> Matrix<t> Matrix<t>::operator *(Matrix<t>& o)
{
	assert(w == o.h && "Multiply is impossible - origin matrix width is not equal other height");
	std::vector<t> _v = std::vector<t>(h * o.w);
	for (unsigned int i = 0; i < h; i++)
		for (unsigned int j = 0; j < o.w; j++)
			for (unsigned int k = 0; k < w; k++) {
				_v[j + i * o.w] += v[i * w + k] * o.v[k * o.w + j];
			}

	return Matrix<t>(o.w, h, &_v);
}


// / / / / / / / / / / / / / / / / / / / / / / / / 

template<class t> Matrix<t> Matrix<t>::operator /(int o)
{
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		_v[i] /= o;
	return Matrix<t>(w, h, _v);
}
template<class t> Matrix<t>& Matrix<t>::operator /(float o)
{
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		_v[i] /= o;
	return Matrix<t>(w, h, _v);
}


// = = = = = = = = = = = = = = = = = = = = = 

template<class t> bool Matrix<t>::operator ==(int o)
{
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		if (_v[i] != o) return false;
	return true;
}
template<class t> bool Matrix<t>::operator ==(float o)
{
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		if (_v[i] != o) return false;
	return true;
}
template<class t> bool Matrix<t>::operator ==(Matrix<t>& o)
{
	check_sizes(o);
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		if (_v[i] != o.v[i]) return false;
	return true;
}


// != != != != != != != != != != != != != 

template<class t> bool Matrix<t>::operator !=(int o)
{
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		if (_v[i] == o) return false;
	return true;
}
template<class t> bool Matrix<t> ::operator !=(float o)
{
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
		if (_v[i] == o) return false;
	return true;
}
template<class t> bool Matrix<t> ::operator !=(Matrix<t>& o)
{
	check_sizes(o);
	t* _v = v.data();
	for (int i = 0; i < w * h; i++)
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


template<class t> t Matrix<t>::operator [](int i)
{
	assert(i < w * h && "Out of bounds");
	return v[i];
}
template<class t> std::vector<t> Matrix<t>::operator ()(int i)
{
	assert(i < h && "Out of bounds");
	return std::vector<t>(v.begin() + i * w, v.begin() + (i + 1) * w);
}
template<class t> t Matrix<t>::operator ()(int i1, int i2)
{
	assert(i1 < h && i2 < w && "Out of bounds");
	return v[i2 + i1 * w];
}

template<class t> void Matrix<t>::check_sizes(Matrix<t>& m)
{
	assert(w == m.w);
	assert(h == m.h);
}


template <class t> std::ostream& operator <<(std::ostream& s, Matrix<t>& m) {
	m.log();
	return s;
}