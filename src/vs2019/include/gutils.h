#pragma once
#include "geometry.h"

Matrix<float> viewport(int x, int y, int w, int h, const int d)
{
	std::cout << x << " " << y << " " << w << " " << h << " " << d << std::endl;
	Matrix<float> m = Matrix<float>::identity(4);
	m(0, 3) = x + w / 2.f;
	m(1, 3) = y + h / 2.f;
	m(2, 3) = d / 2.f;

	m(0, 0) = w / 2.f;
	m(1, 1) = h / 2.f;
	m(2, 2) = d / 2.f;

	return m;
}