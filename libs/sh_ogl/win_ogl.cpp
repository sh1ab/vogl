#include "win_ogl.hpp"
#include <Windows.h>
#include "ogl.hpp"

namespace sh_ogl {

    void win_ogl::setHWND(HWND w) {
	    this->win = w;
    }
    
    void win_ogl::init() {
        this->hdc = GetDC(this->win);

        PIXELFORMATDESCRIPTOR pfd;
        ZeroMemory(&pfd, sizeof(pfd));
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;
        pfd.cDepthBits = 24;
        pfd.iLayerType = PFD_MAIN_PLANE;

        SetPixelFormat(this->hdc, ChoosePixelFormat(this->hdc, &pfd), &pfd);
        this->hRC = wglCreateContext(this->hdc);
        wglMakeCurrent(this->hdc, this->hRC);

        gladLoadGL();
    }

    void win_ogl::finit() {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(this->hRC);
        ReleaseDC(this->win, this->hdc);
    }

    void win_ogl::start_draw() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void win_ogl::end_draw() {
        SwapBuffers(this->hdc);
    }
    
};