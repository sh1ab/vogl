#ifndef _VAR_HPP_
#define _VAR_HPP_

#include <cinttypes>
#include "../math/vec3.hpp"

namespace sh_vogl {
    namespace var {        
        template<typename vox_type, uint32_t vox_size_x, uint32_t vox_size_y, uint32_t vox_size_z>
        class var {
        public:
            vox_type* vox_ids;
            var() {
                vox_ids = (vox_type*) malloc(vox_size_x*vox_size_y*vox_size_z*sizeof(vox_type));
                memset(vox_ids, 0, (vox_size_x*vox_size_y*vox_size_z*sizeof(vox_type))>>3);
            }
            ~var() {
                finit();
            }
            uint32_t ox() const { return vox_size_x; }
            uint32_t oy() const { return vox_size_y; }
            uint32_t oz() const { return vox_size_z; }
            vox_type& operator()(const uint32_t& x, const uint32_t& y, const uint32_t& z) {
                return vox_ids[x + vox_size_x*(y + vox_size_y*z)];
            }
            vox_type get(const uint32_t& x, const uint32_t& y, const uint32_t& z) const {
                return vox_ids[x + vox_size_x*(y + vox_size_y*z)];
            }
        private:
            vox_type* __get(const uint32_t& x, const uint32_t& y, const uint32_t& z) const {
                return vox_ids + (x + vox_size_x*(y + vox_size_y*z));
            }
            #define _t_max__(a, b) (((a)>(b))?(a):(b))
            float par_intersect(float rox, float roy, float roz, float inv_rdx, float inv_rdy, float inv_rdz, float x, float y, float z) {
                rox = inv_rdx * ((inv_rdx < 0) * x - rox);
                roy = inv_rdy * ((inv_rdy < 0) * y - roy);
                roz = inv_rdz * ((inv_rdz < 0) * z - roz);
                return _t_max__(_t_max__(rox, roy), _t_max__(roz, 0));
            }
            float cube_intersect(float rox, float roy, float roz, float inv_rdx, float inv_rdy, float inv_rdz) {
                rox = inv_rdx * ((inv_rdx < 0) - rox);
                roy = inv_rdy * ((inv_rdy < 0) - roy);
                roz = inv_rdz * ((inv_rdz < 0) - roz);
                return _t_max__(_t_max__(rox, roy), _t_max__(roz, 0));
            }
            #undef _t_max__
            bool ppp(float v, float w) {return v <= 0 && w <= 0;}
        public:
        
            bool cast_ray(
                vox_type* v,    float* l,                                       float* normx,   float* normy,   float* normz, 
                float* rox,     float* roy,     float* roz,                     float rdx,      float rdy,      float rdz, 
                uint32_t x0,    uint32_t y0,    uint32_t z0,                    uint32_t sx,    uint32_t sy,    uint32_t sz
            ) {

                float ro_0x = *rox;
                float ro_0y = *roy;
                float ro_0z = *roz;

                if (rdx == 0 || rdy == 0 || rdz == 0) return false;

                float inv_rdx = 1.0f / rdx;
                float inv_rdy = 1.0f / rdy;
                float inv_rdz = 1.0f / rdz;

                float ll = par_intersect(*rox, *roy, *roz, inv_rdx, inv_rdy, inv_rdz, sx, sy, sz) + 0.001f;

                *rox += rdx * ll;
                *roy += rdy * ll;
                *roz += rdz * ll;

                if (*rox < 0 || *rox >= sx || *roy < 0 || *roy >= sy || *roz < 0 || *roz >= sz)
                    return false;

                
                uint32_t vox_posx = *rox;
                uint32_t vox_posy = *roy;
                uint32_t vox_posz = *roz;
                int8_t stepx = (rdx < 0) - (rdx > 0);
                int8_t stepy = (rdy < 0) - (rdy > 0);
                int8_t stepz = (rdz < 0) - (rdz > 0);
                float tmaxx = inv_rdx*((1 - stepx) * 0.5f + vox_posx - *rox);
                float tmaxy = inv_rdy*((1 - stepy) * 0.5f + vox_posy - *roy);
                float tmaxz = inv_rdz*((1 - stepz) * 0.5f + vox_posz - *roz);

                
                while(true) {
                    v = __get(vox_posx + x0, vox_posy + y0, vox_posz + z0);

                    if (*v != 0) {
                        *l = cube_intersect(ro_0x - vox_posx, ro_0y - vox_posy, ro_0z - vox_posz, inv_rdx, inv_rdy, inv_rdz);
                        *rox = vox_posx;
                        *roy = vox_posy;
                        *roz = vox_posz;
                        return true;
                    }

                    *normx = stepx * ppp(tmaxx - tmaxy, tmaxx - tmaxz);
                    *normy = stepy * ppp(tmaxy - tmaxz, tmaxy - tmaxx);
                    *normz = stepz * ppp(tmaxz - tmaxx, tmaxz - tmaxy);
                    vox_posx -= (*normx);
                    vox_posy -= (*normy);
                    vox_posz -= (*normz);

                    if (vox_posx < 0 || vox_posx >= sx || vox_posy < 0 || vox_posy >= sy || vox_posz < 0 || vox_posz >= sz)
                        return false;

                    tmaxx -= inv_rdx * (*normx);
                    tmaxy -= inv_rdy * (*normy);
                    tmaxz -= inv_rdz * (*normz);
                }

                return false;
            }
            void finit() {
                free(vox_ids);
            }
        };
    };
};

#endif //_VAR_HPP_