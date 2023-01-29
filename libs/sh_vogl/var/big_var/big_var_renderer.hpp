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

                void set_cam_near  (float near  );
                void set_cam_width (float width );
                void set_cam_height(float height);

                void set_cam_pos(math::R3::vec<float>  v);
                void set_cam_pos(math::R3::vec<float>* v);
                void set_cam_ang(math::R3::vec<float>  v);
                void set_cam_ang(math::R3::vec<float>* v);
                
                float get_cam_near  ();
                float get_cam_width ();
                float get_cam_height();

                math::R3::vec<float> get_cam_pos();
                math::R3::vec<float> get_cam_ang();
            };
            
            typedef struct var_obj {
                math::R3::vec<float> tex;
                math::R3::vec<float> tex_size;
                math::R3::vec<float> pos;
                math::R3::vec<float> ang;
                float size;
                var_obj(math::R3::vec<float> tex = math::R3::vec<float>(0, 0, 0), math::R3::vec<float> tex_size = math::R3::vec<float>(0, 0, 0), math::R3::vec<float> mod_pos = math::R3::vec<float>(0, 0, 0), math::R3::vec<float> mod_ang = math::R3::vec<float>(0, 0, 0), float mod_size = 1);
            }var_obj;
            
            void set_var_objects(var_obj* objects, size_t offset, size_t amount);

            void init();

            void set_var(math::R3::vec<uint32_t> var_size, void* data);
            template<typename vox_type, uint32_t var_size_x, uint32_t var_size_y, uint32_t var_size_z> 
            void set_var(var<vox_type, var_size_x, var_size_y, var_size_z> *v) { set_var(math::R3::vec<uint32_t>(var_size_x, var_size_y, var_size_z), v->vox_ids); }

            void set_subvar(math::R3::vec<uint32_t> v, math::R3::vec<uint32_t> subvar_size, void* data);
            template<typename vox_type, uint32_t subvar_size_x, uint32_t subvar_size_y, uint32_t subvar_size_z> 
            void set_subvar(math::R3::vec<uint32_t> V, var<vox_type, subvar_size_x, subvar_size_y, subvar_size_z> *v) { set_subvar(V, math::R3::vec<uint32_t>(subvar_size_x, subvar_size_y, subvar_size_z), v->vox_ids); }
            
            void set_vox(math::R3::vec<uint32_t> v, void* data);

            void draw(uint32_t offset, uint32_t amount);
            void stop();
        };
    };
};

#endif //_VAR_RENDERER_HPP_