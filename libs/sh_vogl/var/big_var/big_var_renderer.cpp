#include "big_var_renderer.hpp"
#include <ppl_shad.hpp>
#include <ogl.hpp>
#include <ubo.hpp>
#include "shaders/shaders.hpp"
#include "../../math/vec3.hpp"

namespace sh_vogl {
    namespace var {
        namespace big_var {
            namespace ubo {
                struct __ubo_struct {
                    math::R3::vec<uint32_t> atlas_size;
                    math::R3::vec<uint32_t> tex;
                    math::R3::vec<uint32_t> tex_size;
                    
                    float n;
                    float w;
                    float h;

                    math::R3::vec<float> cam_pos;
                    math::R3::vec<float> cam_ang;
                    math::R3::vec<float> mod_pos;
                    math::R3::mat<float> mod_mat;
                };
                sh_ogl::ubo<GL_DYNAMIC_DRAW, 3, struct __ubo_struct> ubo_buf;

                void init() {
                    ubo_buf.init();
                    ubo_buf.load_ubo();
                }
                void load() { ubo_buf.load_ubo(); }

                void set_tex(math::R3::vec<uint32_t>  v) { ubo_buf.buf.tex =  v; }
                void set_tex(math::R3::vec<uint32_t>* v) { ubo_buf.buf.tex = *v; }
                void set_tex_size(math::R3::vec<uint32_t>  v) { ubo_buf.buf.tex_size =  v; }
                void set_tex_size(math::R3::vec<uint32_t>* v) { ubo_buf.buf.tex_size = *v; }

                void set_cam_near  (float near  ) { ubo_buf.buf.n = near  ; }
                void set_cam_width (float width ) { ubo_buf.buf.w = width ; }
                void set_cam_height(float height) { ubo_buf.buf.h = height; }

                void set_cam_pos(math::R3::vec<float>  v) { ubo_buf.buf.cam_pos =  v; }
                void set_cam_pos(math::R3::vec<float>* v) { ubo_buf.buf.cam_pos = *v; }
                void set_cam_ang(math::R3::vec<float>  v) { ubo_buf.buf.cam_ang =  v; }
                void set_cam_ang(math::R3::vec<float>* v) { ubo_buf.buf.cam_ang = *v; }
                void set_mod_pos(math::R3::vec<float>  v) { ubo_buf.buf.mod_pos =  v; }
                void set_mod_pos(math::R3::vec<float>* v) { ubo_buf.buf.mod_pos = *v; }

                void set_mod_mat(math::R3::mat<float>  mat) { ubo_buf.buf.mod_mat =  mat; }
                void set_mod_mat(math::R3::mat<float>* mat) { ubo_buf.buf.mod_mat = *mat; }

                float get_cam_near  () { return ubo_buf.buf.n; }
                float get_cam_width () { return ubo_buf.buf.w; }
                float get_cam_height() { return ubo_buf.buf.h; }

                math::R3::vec<float> get_cam_pos() { return ubo_buf.buf.cam_pos; }
                math::R3::vec<float> get_cam_ang() { return ubo_buf.buf.cam_ang; }
                math::R3::vec<float> get_mod_pos() { return ubo_buf.buf.mod_pos; }

                math::R3::mat<float> get_mod_mat() { return ubo_buf.buf.mod_mat; }
            };
            sh_ogl::ppl_prog pr;
            uint32_t texture;

            void init() {

                glEnable(GL_DEPTH_TEST);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glEnable(GL_CULL_FACE);

                pr.gen(2);
                pr.create(GL_VERTEX_SHADER, GL_VERTEX_SHADER_BIT, big_var_vert_text, 0);
                pr.create(GL_FRAGMENT_SHADER, GL_FRAGMENT_SHADER_BIT, big_var_frag_text, 1);
                pr.bind();


                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_3D, texture);

                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                ubo::init();
            }
            void set_var(math::R3::vec<uint32_t> var_size, void* data) {
                ubo::ubo_buf.buf.atlas_size = var_size;
                
                ubo::ubo_buf.load_ubo();

                glBindTexture(GL_TEXTURE_3D, texture);
                glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, var_size.x, var_size.y, var_size.z, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            void set_subvar(math::R3::vec<uint32_t> v, math::R3::vec<uint32_t> subvar_size, void* data) {                
                glBindTexture(GL_TEXTURE_3D, texture);
                glTexSubImage3D(GL_TEXTURE_3D, 0, v.x, v.y, v.z, subvar_size.x, subvar_size.y, subvar_size.z, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            void set_vox(math::R3::vec<uint32_t> v, void* data) {                
                glBindTexture(GL_TEXTURE_3D, texture);
                glTexSubImage3D(GL_TEXTURE_3D, 0, v.x, v.y, v.z, 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            void draw() {
                pr.bind();
                glDrawArrays(GL_TRIANGLES, 0, 36);
                pr.unbind();
            }
            void stop() {
                pr.finit();
            }
        };
    };
};