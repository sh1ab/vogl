#ifndef _UBO_HPP_
#define _UBO_HPP_

#include <cinttypes>
#include "ogl.hpp"

namespace sh_ogl {
    template<uint32_t draw_type, uint32_t base, typename ubo_struct>
    struct st_ubo {
        private:
            uint32_t id;
        public:
        ubo_struct buf;
        void init() {
            glGenBuffers(1, &id);
            glBindBuffer(GL_UNIFORM_BUFFER, id);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(ubo_struct), &buf, draw_type);
            glBindBufferBase(GL_UNIFORM_BUFFER, base, id);
        }
        void load_ubo() {
            glBindBuffer(GL_UNIFORM_BUFFER, id);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ubo_struct), &buf);
        }
        void finit() {
            glDeleteBuffers(1, &id);
            id = 0;
        }
    };
    template<uint32_t draw_type, uint32_t base, typename ubo_struct> using ubo = struct st_ubo<draw_type, base, ubo_struct>;
};

#endif //_UBO_HPP_