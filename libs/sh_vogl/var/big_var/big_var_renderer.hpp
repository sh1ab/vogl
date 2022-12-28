#ifndef _BIG_VAR_RENDERER_HPP_
#define _BIG_VAR_RENDERER_HPP_

#include "../../math/mat3.hpp"
#include "../var.hpp"

namespace sh_vogl {
    namespace var {
        namespace big_var {
            namespace ubo {
                void load();

                void set_tex(uint32_t x0, uint32_t x1, uint32_t x2);
                void set_tex_size(uint32_t x0, uint32_t x1, uint32_t x2);

                void set_cam_near(float near);
                void set_cam_width(float width);
                void set_cam_height(float height);

                void set_cam_pos(float x0, float x1, float x2);
                void add_cam_pos(float x0, float x1, float x2);
                void set_cam_pos_index(uint32_t i, float x);
                void add_cam_pos_index(uint32_t i, float x);

                void set_cam_ang(float a0, float a1, float a2);
                void add_cam_ang(float a0, float a1, float a2);
                void set_cam_ang_index(uint32_t i, float x);
                void add_cam_ang_index(uint32_t i, float x);

                void set_mod_pos(float x0, float x1, float x2);
                void add_mod_pos(float x0, float x1, float x2);
                void set_mod_pos_index(uint32_t i, float x);
                void add_mod_pos_index(uint32_t i, float x);

                void set_mod_mat(math::R3::mat<float> mat);
                void set_mod_mat(math::R3::mat<float>* mat);
                void set_mod_mat_index(uint32_t i, uint32_t j, float a_ij);
                void add_mod_mat_index(uint32_t i, uint32_t j, float a_ij);

                
                float get_cam_near();
                float get_cam_width();
                float get_cam_height();

                float get_cam_pos_index(uint32_t i);
                float get_cam_ang_index(uint32_t i);
                float get_mod_pos_index(uint32_t i);

                math::R3::mat<float> get_mod_mat();
                float get_mod_mat_index(uint32_t i, uint32_t j);
            };
            void init();
            template<typename vox_type, const uint32_t _ox, const uint32_t _oy, const uint32_t _oz>
            void set_var(var<vox_type, _ox, _oy, _oz> *v) {
                set_var(_ox, _oy, _oz, v->vox_ids);
            }
            void set_var(uint32_t ox, uint32_t oy, uint32_t oz, void* data);
            template<typename vox_type, const uint32_t _ox, const uint32_t _oy, const uint32_t _oz>
            void set_subvar(uint32_t x, uint32_t y, uint32_t z, var<vox_type, _ox, _oy, _oz> *v) {
                set_subvar(x, y, z, _ox, _oy, _oz, v->vox_ids);
            }
            void set_subvar(uint32_t x, uint32_t y, uint32_t z, uint32_t ox, uint32_t oy, uint32_t oz, void* data);
            void set_vox(uint32_t x, uint32_t y, uint32_t z, void* data);
            void draw();
            void stop();
        };
    };
};

#endif //_VAR_RENDERER_HPP_