#ifndef _BIG_VAR_RENDERER_HPP_
#define _BIG_VAR_RENDERER_HPP_

#include "../../mat3x3.h"
#include "../var.hpp"

namespace vogl {
    namespace var {
        namespace big_var {
            namespace ubo {
                void load();
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

                void set_mod_mat(fmat3x3 mat);
                void set_mod_mat(fmat3x3* mat);
                void set_mod_mat_index(uint32_t i, uint32_t j, float a_ij);
                void add_mod_mat_index(uint32_t i, uint32_t j, float a_ij);

                
                float get_cam_near();
                float get_cam_width();
                float get_cam_height();

                float get_cam_pos_index(uint32_t i);
                float get_cam_ang_index(uint32_t i);
                float get_mod_pos_index(uint32_t i);

                fmat3x3 get_mod_mat();
                float get_mod_mat_index(uint32_t i, uint32_t j);
            };
            void init();
            template<typename vox_type, const uint32_t _ox, const uint32_t _oy, const uint32_t _oz>
            void set_var(vogl::var::var<vox_type, _ox, _oy, _oz> *v) {
                set_var(_ox, _oy, _oz, sizeof(vox_type), v->vox_ids);
            }
            void set_var(uint32_t ox, uint32_t oy, uint32_t oz, size_t type_size, void* data);
            void draw();
            void stop();
        };
    };
};

#endif //_VAR_RENDERER_HPP_