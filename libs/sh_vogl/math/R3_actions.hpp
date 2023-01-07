#ifndef _R3_ACTIONS_HPP_
#define _R3_ACTIONS_HPP_

#include "vec3.hpp"
#include "mat3.hpp"

namespace sh_vogl {
	namespace math {
		namespace R3 {
            template <typename V, typename K> 
            inline vec<V> operator*(const vec<V>& v, const K& k) { return vec<V>(v.x * k, v.y * k, v.z * k); }
            template <typename V, typename K> 
            inline vec<V> operator*(const K& k, const vec<V>& v) { return vec<V>(k * v.x, k * v.y, k * v.z); }
            template <typename V, typename K> 
            inline vec<V> operator/(const vec<V>& v, const K& k) { return vec<V>(v.x / k, v.y / k, v.z / k); }
            
            template <typename V, typename W> 
            inline vec<V> operator*(const vec<V>& v, const vec<W>& w) { return vec<V>(v.x * w.x, v.y * w.y, v.z * w.z); }
            template <typename V, typename W> 
            inline vec<V> operator/(const vec<V>& v, const vec<W>& w) { return vec<V>(v.x / w.x, v.y / w.y, v.z / w.z); }

            
            template <typename V, typename W> 
            inline vec<V> operator+(const vec<V>& v, const vec<W>& w) { return vec<V>(v.x + w.x, v.y + w.y, v.z + w.z); }
            template <typename V, typename W> 
            inline vec<V> operator-(const vec<V>& v, const vec<W>& w) { return vec<V>(v.x - w.x, v.y - w.y, v.z - w.z); }

            template <typename M, typename V> 
            inline vec<M> operator*(mat<M> m, vec<V> v) { return vec<M>(
                (m(0, 0))*(v.x) + (m(0, 1))*(v.y) + (m(0, 2))*(v.z), 
                (m(1, 0))*(v.x) + (m(1, 1))*(v.y) + (m(1, 2))*(v.z), 
                (m(2, 0))*(v.x) + (m(2, 1))*(v.y) + (m(2, 2))*(v.z)); }
            template <typename M, typename C> 
            inline mat<M> operator*(mat<M> m, mat<C> c) { return mat<M>(
                m(0, 0)*c(0, 0) + m(0, 1)*c(1, 0) + m(0, 2)*c(2, 0),        m(1, 0)*c(0 ,0) + m(1, 1)*c(1, 0) + m(1, 2)*c(2, 0),        m(2, 0)*c(0, 0) + m(2, 1)*c(1, 0) + m(2, 2)*c(2, 0),
                m(0, 0)*c(0, 1) + m(0, 1)*c(1, 1) + m(0, 2)*c(2, 1),        m(1, 0)*c(0 ,1) + m(1, 1)*c(1, 1) + m(1, 2)*c(2, 1),        m(2, 0)*c(0, 1) + m(2, 1)*c(1, 1) + m(2, 2)*c(2, 1),
                m(0, 0)*c(0, 2) + m(0, 1)*c(1, 2) + m(0, 2)*c(2, 2),        m(1, 0)*c(0 ,2) + m(1, 1)*c(1, 2) + m(1, 2)*c(2, 2),        m(2, 0)*c(0, 2) + m(2, 1)*c(1, 2) + m(2, 2)*c(2, 2)
            ); }
            inline mat<float> rot_xy(float alpha) {
                return mat<float>(
                     cos(alpha), sin(alpha), 0,
                    -sin(alpha), cos(alpha), 0,
                     0         , 0         , 1
                );
            }
            inline mat<float> rot_yz(float alpha) {
                return mat<float>(
                    1,  0         , 0         ,
                    0,  cos(alpha), sin(alpha),
                    0, -sin(alpha), cos(alpha)
                );
            }
            inline mat<float> rot_zx(float alpha) {
                return mat<float>(
                    cos(alpha), 0, -sin(alpha),
                    0         , 1,  0         ,
                    sin(alpha), 0,  cos(alpha)
                );
            }
        };
    };
};
#endif//_R3_ACTIONS_HPP_