#ifndef _ABSTRACT_OGL_HPP_
#define _ABSTRACT_OGL_HPP_

namespace sh_ogl {

    struct abstract_ogl {
        virtual void init() = 0;
        virtual void finit() = 0;
        virtual void start_draw() = 0;
        virtual void end_draw() = 0;
    };

}

#endif //_ABSTRACT_OGL_HPP_