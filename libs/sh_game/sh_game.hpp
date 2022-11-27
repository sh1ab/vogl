#ifndef _SH_GAME_HPP_
#define _SH_GAME_HPP_

#include <cinttypes>

enum G {
    OK,
    UNDEF_ERR,
    NICE_END,
};

typedef struct game {
    virtual uint32_t init();
    virtual uint32_t loop();
    virtual uint32_t draw();
    virtual uint32_t stop();
} game;

#endif //_SH_GAME_HPP_