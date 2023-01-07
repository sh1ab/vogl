#ifndef _MAT_3_HPP_
#define _MAT_3_HPP_

#include <cassert>
#include <cinttypes>
#include <iostream>

namespace sh_vogl {
	namespace math {
		namespace R3 {
			template <typename T>
			struct mat3 {
				T A[9];
				mat3(T aii = 0) {
					A[0] = aii;
					A[1] = 0;
					A[2] = 0;
					A[3] = 0;
					A[4] = aii;
					A[5] = 0;
					A[6] = 0;
					A[7] = 0;
					A[8] = aii;
				}
				mat3(T a00, T a01, T a02, T a10, T a11, T a12, T a20, T a21, T a22) {
					A[0] = a00;
					A[1] = a01;
					A[2] = a02;
					A[3] = a10;
					A[4] = a11;
					A[5] = a12;
					A[6] = a20;
					A[7] = a21;
					A[8] = a22;
				}

				T& operator() (uint32_t i, uint32_t j) { return A[i * 3 + j]; }
				mat3<T> operator=(mat3<T> g) { 
					A[0] = g.A[0];
					A[1] = g.A[1];
					A[2] = g.A[2];
					A[3] = g.A[3];
					A[4] = g.A[4];
					A[5] = g.A[5];
					A[6] = g.A[6];
					A[7] = g.A[7];
					A[8] = g.A[8];
					return *this;
				}

				template <typename O>
				mat3<T> operator*=(const mat3<O>& m) {
					return operator=((*this) * m);
				}

				mat3<T> invert() {
					mat3<T> m;
					float inv_det = det(*this);
					if (inv_det*inv_det*1000000 < 0.00001f) {
						std::cout << "\nno, peeeen\n";
						return m;
					}
					inv_det = 1.0f / inv_det;

					m(0, 0) = (operator()(1, 1) * operator()(2, 2) - operator()(2, 1) * operator()(1, 2)) * inv_det;
					m(0, 1) = (operator()(0, 2) * operator()(2, 1) - operator()(0, 1) * operator()(2, 2)) * inv_det;
					m(0, 2) = (operator()(0, 1) * operator()(1, 2) - operator()(0, 2) * operator()(1, 1)) * inv_det;
					m(1, 0) = (operator()(1, 2) * operator()(2, 0) - operator()(1, 0) * operator()(2, 2)) * inv_det;
					m(1, 1) = (operator()(0, 0) * operator()(2, 2) - operator()(0, 2) * operator()(2, 0)) * inv_det;
					m(1, 2) = (operator()(1, 0) * operator()(0, 2) - operator()(0, 0) * operator()(1, 2)) * inv_det;
					m(2, 0) = (operator()(1, 0) * operator()(2, 1) - operator()(2, 0) * operator()(1, 1)) * inv_det;
					m(2, 1) = (operator()(2, 0) * operator()(0, 1) - operator()(0, 0) * operator()(2, 1)) * inv_det;
					m(2, 2) = (operator()(0, 0) * operator()(1, 1) - operator()(1, 0) * operator()(0, 1)) * inv_det;
					return m;
				}
			};

			template<typename T>
			using mat = struct mat3<T>;

			template <typename T>
			float det(mat<T>& m) {
				return
					m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) -
					m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
					m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));
			}
		};
	};
};
#endif //_MAT_3_HPP_