#include "big_var_renderer.hpp"
#include <ppl_shad.hpp>
#include <ogl.hpp>
#include <ubo.hpp>
#include "shaders/shaders.hpp"

namespace vogl {
    namespace var {
        namespace big_var {
            namespace ubo {
                struct __ubo_struct {
                    float n;
                    float w;
                    float h;

                    uint32_t size[3];

                    float pos[3];
                    float ang[3];

                    float mod_pos[3];

                    fmat3x3 mod_mat;
                };
                sh_ogl::ubo<GL_DYNAMIC_DRAW, 3, struct __ubo_struct> ubo_buf;

                void init() {
                    ubo_buf.init();
                    ubo_buf.load_ubo();
                }
                void load() { ubo_buf.load_ubo(); }

                void set_cam_near(float near) {ubo_buf.buf.n = near;}
                void set_cam_width(float width) {ubo_buf.buf.w = width;}
                void set_cam_height(float height) {ubo_buf.buf.h = height;}

                void set_cam_pos(float x0, float x1, float x2) {
                    ubo_buf.buf.pos[0] = x0;
                    ubo_buf.buf.pos[1] = x1;
                    ubo_buf.buf.pos[2] = x2;
                }
                void add_cam_pos(float x0, float x1, float x2) {
                    ubo_buf.buf.pos[0] += x0;
                    ubo_buf.buf.pos[1] += x1;
                    ubo_buf.buf.pos[2] += x2;
                }
                void set_cam_pos_index(uint32_t i, float x) { ubo_buf.buf.pos[i] = x; }
                void add_cam_pos_index(uint32_t i, float x) { ubo_buf.buf.pos[i] += x; }

                void set_cam_ang(float a0, float a1, float a2) {
                    ubo_buf.buf.ang[0] = a0;
                    ubo_buf.buf.ang[1] = a1;
                    ubo_buf.buf.ang[2] = a2;
                }
                void add_cam_ang(float a0, float a1, float a2) {
                    ubo_buf.buf.ang[0] += a0;
                    ubo_buf.buf.ang[1] += a1;
                    ubo_buf.buf.ang[2] += a2;
                }
                void set_cam_ang_index(uint32_t i, float x) { ubo_buf.buf.ang[i] = x; }
                void add_cam_ang_index(uint32_t i, float x) { ubo_buf.buf.ang[i] += x; }

                void set_mod_pos(float x0, float x1, float x2) {
                    ubo_buf.buf.mod_pos[0] = x0;
                    ubo_buf.buf.mod_pos[1] = x1;
                    ubo_buf.buf.mod_pos[2] = x2;
                }
                void add_mod_pos(float x0, float x1, float x2) {
                    ubo_buf.buf.mod_pos[0] += x0;
                    ubo_buf.buf.mod_pos[1] += x1;
                    ubo_buf.buf.mod_pos[2] += x2;
                }
                void set_mod_pos_index(uint32_t i, float x) { ubo_buf.buf.mod_pos[i] = x; }
                void add_mod_pos_index(uint32_t i, float x) { ubo_buf.buf.mod_pos[i] += x; }

                void set_mod_mat(fmat3x3 mat) { ubo_buf.buf.mod_mat = mat; }
                void set_mod_mat(fmat3x3* mat) { ubo_buf.buf.mod_mat = *mat; }
                void set_mod_mat_index(uint32_t i, uint32_t j, float a_ij) { ubo_buf.buf.mod_mat(i, j) = a_ij; }
                void add_mod_mat_index(uint32_t i, uint32_t j, float a_ij) { ubo_buf.buf.mod_mat(i, j) = a_ij; }

                float get_cam_near() { return ubo_buf.buf.n; }
                float get_cam_width() { return ubo_buf.buf.w; }
                float get_cam_height() { return ubo_buf.buf.h; }

                float get_cam_pos_index(uint32_t i) { return ubo_buf.buf.pos[i]; }
                float get_cam_ang_index(uint32_t i) { return ubo_buf.buf.ang[i]; }
                float get_mod_pos_index(uint32_t i) { return ubo_buf.buf.mod_pos[i]; }

                fmat3x3 get_mod_mat() { return ubo_buf.buf.mod_mat; }
                float get_mod_mat_index(uint32_t i, uint32_t j) { return ubo_buf.buf.mod_mat(i, j); }
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
            void set_var(uint32_t ox, uint32_t oy, uint32_t oz, size_t type_size, void* data) {
                ubo::ubo_buf.buf.size[0] = ox;
                ubo::ubo_buf.buf.size[1] = oy;
                ubo::ubo_buf.buf.size[2] = oz;
                
                ubo::ubo_buf.load_ubo();

                glBindTexture(GL_TEXTURE_3D, texture);
                glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, ox, oy, oz, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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