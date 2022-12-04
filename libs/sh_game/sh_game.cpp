#include "sh_game.hpp"

uint32_t game::init() {
    return G::OK;
}
uint32_t game::loop() {
    return G::OK;
}
uint32_t game::resz(uint32_t w, uint32_t h) {
    screen_w = w;
    screen_h = h;
    return G::OK;
}
uint32_t game::draw() {
    return G::OK;
}
uint32_t game::stop() {
    return G::OK;
}