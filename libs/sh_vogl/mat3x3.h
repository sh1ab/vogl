#ifndef VAR_O_VOGL_MAT3X3_H
#define VAR_O_VOGL_MAT3X3_H

#include <cassert>
#include <cinttypes>

template <typename T>
struct mat3x3 {
	T A[9];

	T& operator() (uint32_t i, uint32_t j) { return A[i * 3 + j]; }
	mat3x3<T>* operator=(mat3x3<T> g) { 
		A[0] = g.A[0];
		A[1] = g.A[1];
		A[2] = g.A[2];
		A[3] = g.A[3];
		A[4] = g.A[4];
		A[5] = g.A[5];
		A[6] = g.A[6];
		A[7] = g.A[7];
		A[8] = g.A[8];
		return this;
	}
};

typedef struct mat3x3<float> fmat3x3;

template <typename T>
float det(mat3x3<T>& m) {
	return
		m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) -
		m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
		m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));
}

template <typename T>
mat3x3<T> invert(mat3x3<T> m) {
	mat3x3<T> m0;
	float inv_det = det(m);
	assert(inv_det*inv_det> 0.00001f*0.00001f);
	inv_det = 1.0f / inv_det;

	m0(0, 0) = (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) * inv_det;
	m0(0, 1) = (m(0, 2) * m(2, 1) - m(0, 1) * m(2, 2)) * inv_det;
	m0(0, 2) = (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * inv_det;
	m0(1, 0) = (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) * inv_det;
	m0(1, 1) = (m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0)) * inv_det;
	m0(1, 2) = (m(1, 0) * m(0, 2) - m(0, 0) * m(1, 2)) * inv_det;
	m0(2, 0) = (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)) * inv_det;
	m0(2, 1) = (m(2, 0) * m(0, 1) - m(0, 0) * m(2, 1)) * inv_det;
	m0(2, 2) = (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) * inv_det;
	return m0;
}

#endif //VAR_O_VOGL_MAT3X3_H