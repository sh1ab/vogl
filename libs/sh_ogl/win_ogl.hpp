#ifndef _WIN_OGL_HPP_
#define _WIN_OGL_HPP_

#include <Windows.h>
#include "abstract_ogl.hpp"

namespace sh_ogl {

    struct win_ogl : public abstract_ogl {
    private:
        HDC hdc;
        HGLRC hRC;
        HWND win;
    public:
        void setHWND(HWND w);
        void init();
        void finit();
        void start_draw();
        void end_draw();
    };

};

#endif //_WIN_GL_HPP_