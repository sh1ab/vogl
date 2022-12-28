#ifndef _SH_GAME_HPP_
#define _SH_GAME_HPP_

#include <cinttypes>

namespace sh_game {    
    enum G {
        OK,
        UNDEF_ERR,
        NICE_END,
    };

    typedef struct game {
        uint32_t screen_w = 512, screen_h = 512;
        virtual uint32_t init();
        virtual uint32_t loop();
        virtual uint32_t resz(uint32_t w, uint32_t h);
        virtual uint32_t draw();
        virtual uint32_t stop();
    } game;
};

#endif //_SH_GAME_HPP_