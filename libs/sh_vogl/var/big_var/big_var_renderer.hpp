#ifndef _BIG_VAR_RENDERER_HPP_
#define _BIG_VAR_RENDERER_HPP_

#include "../../math/mat3.hpp"
#include "../../math/vec3.hpp"
#include "../var.hpp"

namespace sh_vogl {
    namespace var {
        namespace big_var {
            namespace ubo {
                void load();

                void set_tex(math::R3::vec<uint32_t>  v);
                void set_tex(math::R3::vec<uint32_t>* v);
                void set_tex_size(math::R3::vec<uint32_t>  v);
                void set_tex_size(math::R3::vec<uint32_t>* v);

                void set_cam_near(float near);
                void set_cam_width(float width);
                void set_cam_height(float height);

                void set_cam_pos(math::R3::vec<float>  v);
                void set_cam_pos(math::R3::vec<float>* v);
                void set_cam_pos_index(uint32_t i, float x);

                void set_cam_ang(math::R3::vec<float>  v);
                void set_cam_ang(math::R3::vec<float>* v);
                void set_cam_ang_index(uint32_t i, float x);

                void set_mod_pos(math::R3::vec<float>  v);
                void set_mod_pos(math::R3::vec<float>* v);
                void set_mod_pos_index(uint32_t i, float x);

                void set_mod_mat(math::R3::mat<float>  m);
                void set_mod_mat(math::R3::mat<float>* m);
                void set_mod_mat_index(uint32_t i, uint32_t j, float a_ij);

                
                float get_cam_near();
                float get_cam_width();
                float get_cam_height();

                math::R3::vec<float> get_cam_pos();
                math::R3::vec<float> get_cam_ang();
                math::R3::vec<float> get_mod_pos();
                math::R3::mat<float> get_mod_mat();
            };
            void init();

            void set_var(uint32_t ox, uint32_t oy, uint32_t oz, void* data);
            template<typename vox_type, uint32_t _ox, uint32_t _oy, uint32_t _oz> void set_var(var<vox_type, _ox, _oy, _oz> *v) { set_var(_ox, _oy, _oz, v->vox_ids); }

            void set_subvar(uint32_t x, uint32_t y, uint32_t z, uint32_t ox, uint32_t oy, uint32_t oz, void* data);
            template<typename vox_type, const uint32_t _ox, const uint32_t _oy, const uint32_t _oz> void set_subvar(uint32_t x, uint32_t y, uint32_t z, var<vox_type, _ox, _oy, _oz> *v) { set_subvar(x, y, z, _ox, _oy, _oz, v->vox_ids); }
            
            void set_vox(uint32_t x, uint32_t y, uint32_t z, void* data);
            void set_vox(math::R3::vec<uint32_t> v, void* data);

            void draw();
            void stop();
        };
    };
};

#endif //_VAR_RENDERER_HPP_