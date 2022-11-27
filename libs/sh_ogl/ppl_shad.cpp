#include "ppl_shad.hpp"

#include "ogl.hpp"
#include <Windows.h>

void send_error(char* c, uint32_t size, char* name, uint32_t name_size) {
        LPWSTR ww = new wchar_t[size];
        LPWSTR name_ww = new wchar_t[name_size];
        MultiByteToWideChar(0, 0, c, size, ww, size);
        MultiByteToWideChar(0, 0, name, name_size, name_ww, name_size);
        MessageBoxW(nullptr, ww, name_ww, MB_OK);
}

void errorGLshader(char* name, uint32_t id) {
    char* error_log = new char[4096];
    glGetProgramInfoLog(id, 4096, NULL, error_log);
    if (error_log[0] != 0) send_error(error_log, 4096, name, 64);
}

namespace sh_ogl {
    void ppl_prog::create(uint32_t type, uint32_t typeBIT, const char* text, uint32_t num) {
        const char* t[] = { text };
        progs[num] = glCreateShaderProgramv(type, sizeof(t) / sizeof(void*), t);
        errorGLshader("shader\0", progs[num]);
        glUseProgramStages(id, typeBIT, progs[num]);
    }
    void ppl_prog::gen(uint32_t shads) { 
        finit();
        glGenProgramPipelines(1, &id); 
        progs = (uint32_t*) malloc(shads*sizeof(uint32_t)); 
    }
    void ppl_prog::bind() { glBindProgramPipeline(id); }
    void ppl_prog::unbind() { glBindProgramPipeline(0); }
    void ppl_prog::finit() {
        if (!id) {
            glDeleteProgramPipelines(1, &id);
            id = 0;
        }
        if (!progs) {
            free(progs);
            progs = 0;
        }
    }
};