#ifndef _VEC_3_HPP_
#define _VEC_3_HPP_

#include <cstdint>
#include <cmath>

namespace sh_vogl {
	namespace math {
		namespace R3 {

			template <typename T>
			struct vec3 {
				T x = 0, y = 0, z = 0;
				inline vec3() {}
				template <typename O>
				inline vec3(const vec3<O>& o) { x = (T)(o.x); y = (T)(o.y); z = (T)(o.z); }
				inline vec3(T x, T y, T z) { this->x = x; this->y = y; this->z = z; }
				template <typename O> inline vec3<T> operator=(const vec3<O>& o) { x = (T)(o.x); y = (T)(o.y); z = (T)(o.z); return *this; }
				//c
				inline T abs2() const { return x * x + y * y + z * z; }
				inline T abs() const { return (T)sqrt(abs2())); }
				template <typename O> inline T dot(vec3<O>& o) const { return x * o.x + y * o.y + z * o.z; }
				//logical
				inline uint32_t operator()() const { return x || y || z; }
				inline uint32_t operator!() const { return !x && !y && !z; }
				template <typename O> inline uint32_t operator==(const vec3<O>& o) const { return x == o.x && y == o.y && z == o.z; }
				template <typename O> inline uint32_t operator!=(const vec3<O>& o) const { return x != o.x || y != o.y || z != o.z; }
				//v-v
				inline vec3<T> norm(T r) { T n = abs(); if (n) { n = r / n; x *= n; y *= n; z *= n; } return *this; }
				inline vec3<T> get_norm(T r) { T n = abs(); if (n) n = r / n; return vec3(x * n, y * n, z * n); }
				inline vec3<T> operator-() const { return vec3<T>(-x, -y, -z); }
				template <typename O> inline vec3<T> operator+=(const vec3<O>& o) { return operator=((*this)+o); }
				template <typename O> inline vec3<T> operator-=(const vec3<O>& o) { return operator=((*this)-o); }
				template <typename O> inline vec3<T> operator*=(const O& o) { return operator=((*this)*o); }
				template <typename O> inline vec3<T> operator/=(const O& o) { return operator=((*this)/o); }
			};

			template<typename T>
			using vec = struct vec3<T>;

		};
	};
};
#endif//_VEC_3_HPP_