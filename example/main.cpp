#include <iostream>
#include <cinttypes>
#include <sh_ogl.hpp>
#include <ogl.hpp>
#include <sh_key.hpp>
#include <Windows.h>
#include <var/var.hpp>
#include <sh_game.hpp>
#include <var/big_var/big_var_renderer.hpp>
#include <chrono>
#include <string>
#include <cmath>
#include <math/vec3.hpp>
#include <math/R3_actions.hpp>
#include <math/mat3.hpp>

sh_ogl::gl wgl;

void do_with_errors(HWND p, uint32_t error) {
    switch(error) {
        case sh_game::G::OK: {

        } break;
        case sh_game::G::NICE_END: {
            PostMessageW(p, WM_CLOSE, sh_game::G::NICE_END, 0);
        }
        case sh_game::G::UNDEF_ERR: {
            PostMessageW(p, WM_CLOSE, sh_game::G::UNDEF_ERR, 0);
        } break;
        default: {

        } break;
    }
}

sh_vogl::var::var<uint32_t, 256, 256, 256> main_var;
sh_vogl::math::R3::vec<float> ang;
sh_vogl::math::R3::vec<float> pos;
float pos_speed = 80;
float mouse_speed = 1;

void edit_block(sh_vogl::math::R3::vec<float> mod_pos, sh_vogl::math::R3::vec<uint32_t> v, sh_vogl::math::R3::vec<uint32_t> subvar_size, sh_vogl::math::R3::vec<float> subvar_ang, float subvar_size_) {
    sh_vogl::math::R3::mat<float> inv_mod = sh_vogl::math::R3::rot_xy(-subvar_ang.z) * sh_vogl::math::R3::rot_zx(-subvar_ang.y) * sh_vogl::math::R3::rot_yz(-subvar_ang.x);
    sh_vogl::math::R3::vec<float> rd(0,0,1);
    rd = inv_mod * sh_vogl::math::R3::rot_xy(-ang.z) * sh_vogl::math::R3::rot_zx(-ang.y) * sh_vogl::math::R3::rot_yz(-ang.x) * rd;

    sh_vogl::math::R3::vec<float> ro = pos - mod_pos;
    ro = inv_mod * ro;
    ro *= (1.0f / subvar_size_);

    uint32_t vox;
    float l;
    sh_vogl::math::R3::vec<float> norm;

    if (!main_var.cast_ray(&vox, &l, &(norm.x), &(norm.y), &(norm.z), &(ro.x), &(ro.y), &(ro.z), rd.x, rd.y, rd.z, v.x, v.y, v.z, subvar_size.x, subvar_size.y, subvar_size.z)) {
        std::cout << "\nno, bool\n";
        return;
    }
    
    main_var(ro.x, ro.y, ro.z) = 0xFF000000 | (0x00FF0000&((rand()+200)*0x10000));

    sh_vogl::var::big_var::set_vox(ro, &main_var(ro.x, ro.y, ro.z));
}

const uint32_t amount_var_obj = 3; 
sh_vogl::var::big_var::var_obj kk[amount_var_obj];

struct gm : public sh_game::game {
    uint32_t init() {
        for(uint32_t i(main_var.ox());i--;)
            for(uint32_t k(main_var.oz());k--;)
                for(uint32_t j(main_var.oy()*0.4f*(1+sin(0.01f*i + sin(0.01f*k))));j--;)
                    main_var(i, j, k) = (rand() & 0x00FFFFFF) | 0xFF000000;

        sh_vogl::var::big_var::init();
        sh_vogl::var::big_var::set_var(&main_var);

        sh_vogl::var::big_var::ubo::set_cam_near(0.1f);
        sh_vogl::var::big_var::ubo::set_cam_width(screen_w);
        sh_vogl::var::big_var::ubo::set_cam_height(screen_h);
        
        kk[0].ang.x = 0.7f;
        kk[0].ang.y = -0.7f;
        kk[0].ang.z = -0.7f;
        kk[0].tex_size = sh_vogl::math::R3::vec<float>(128, 32, 128);
        
        kk[1].pos.y = 10;
        kk[1].pos.x = 10;
        kk[1].pos.z = -10;
        kk[1].ang.x = -0.7f;
        kk[1].ang.y = 0.7f;
        kk[1].ang.z = 0.7f;
        kk[1].tex_size = sh_vogl::math::R3::vec<float>(256, 128, 256);
        
        kk[2].ang.x = 1.7f;
        kk[2].ang.y = 0.7f;
        kk[2].ang.z = 0.7f;
        kk[2].pos.y = -10;
        kk[2].pos.x = -256;
        kk[2].pos.z = 10;
        kk[2].tex_size = sh_vogl::math::R3::vec<float>(64, 256, 64);

        sh_vogl::var::big_var::set_var_objects(kk, 0, amount_var_obj);

        pos = sh_vogl::math::R3::vec<float>(0, 1, 0);

        sh_vogl::var::big_var::ubo::load();

        return sh_game::G::OK;
    }
    float dt = 0.001f;
    uint32_t loop(float dt) {
        if(shks::get_key(VK_MENU).held)SetCursor(LoadCursor(NULL, IDC_ARROW));
        else SetCursor(NULL);
        this->dt = dt;
        if (shks::get_key(VK_ESCAPE).pressed)
            return sh_game::G::NICE_END;

        sh_vogl::math::R3::vec<float> v;
        v.x = shks::get_key('D').held - shks::get_key('A').held;
        v.z = shks::get_key('W').held - shks::get_key('S').held;
        float l = v.x*v.x + v.z*v.z;
        if (l != 0) {
            l = 1.0f/sqrt(l);
            v.x *= l;
            v.z *= l;
            v = sh_vogl::math::R3::rot_zx(-ang.y)*v;
        }
        v.y = shks::get_key(VK_SPACE).held - shks::get_key(VK_SHIFT).held;
        pos += v*pos_speed*dt;
        
        return sh_game::G::OK;
    }
    sh_vogl::math::R3::vec<float> a;
    uint32_t touch_move(int32_t to_x, int32_t to_y, int32_t from_x, int32_t from_y) {
        a.y += (from_x - to_x) * dt;
        a.x += (from_y - to_y) * dt;
        ang += a*mouse_speed;
        a *= 0.9f - dt;
        if (ang.x < -1.570796f) ang.x = -1.570796f;
        if (ang.x >  1.570796f) ang.x =  1.570796f;
        if (ang.y < 0)         ang.y += 6.283185f;
        if (ang.y > 6.283185f) ang.y -= 6.283185f;
        if (ang.z < 0)         ang.z += 6.283185f;
        if (ang.z > 6.283185f) ang.z -= 6.283185f;

        return sh_game::G::OK;
    }
    uint32_t resz(uint32_t w, uint32_t h) {
        screen_w = w;
        screen_h = h;
        glViewport(0, 0, screen_w, screen_h);
        sh_vogl::var::big_var::ubo::set_cam_width(screen_w);
        sh_vogl::var::big_var::ubo::set_cam_height(screen_h);
        sh_vogl::var::big_var::ubo::load();
        return sh_game::G::OK;
    }
    uint32_t draw() {
        wgl.start_draw();
        glClearColor(0.01f, 0.01f, 0.01f, 1);
        sh_vogl::var::big_var::ubo::set_cam_pos(pos);
        sh_vogl::var::big_var::ubo::set_cam_ang(ang);
        sh_vogl::var::big_var::ubo::load();

        #define gg 1
        if(shks::get_key('G').held)
            edit_block(kk[gg].pos, kk[gg].tex, kk[gg].tex_size, kk[gg].ang, kk[gg].size);
        #undef gg

        sh_vogl::var::big_var::draw(0, amount_var_obj);
        
        wgl.end_draw();
        return sh_game::G::OK;
    }
    uint32_t stop() {
        sh_vogl::var::big_var::stop();
        return sh_game::G::OK;
    }
} gm;

LRESULT win_proc(HWND win, UINT msg, WPARAM wpr, LPARAM lpr) {
    LONG_PTR win0 = GetWindowLongPtr(win, GWLP_USERDATA);
	if (win0 == (LONG_PTR)(&gm))
        switch (msg) {
            case WM_PAINT: {
                do_with_errors(win, gm.draw());
            } break;
            case WM_CLOSE: {
                switch (wpr) {
                    case sh_game::G::NICE_END: {
                        do_with_errors(win, gm.stop());
                        PostQuitMessage(0);
                    } break;
                    case sh_game::G::UNDEF_ERR: {
                        std::cout<<"ERROR!!!\n";
                        MessageBoxW(NULL, L"Oh, sheet!", L"It's undefined error, it so warst(", MB_ERR_INVALID_CHARS);
                        do_with_errors(win, gm.stop());
                    } break;
                };
            } break;
            case WM_SIZE: {
                RECT wRect; GetClientRect(win, &wRect);
                do_with_errors(win, gm.resz(wRect.right - wRect.left, wRect.bottom - wRect.top));
            }break;
            case WM_MOUSEMOVE: {
                if(!shks::get_key(VK_MENU).held) {
                    static int32_t prev_x = 0, prev_y = 0;
                    POINT pt;
                    GetCursorPos(&pt);
                    RECT wRect; GetWindowRect(win, &wRect);
                    prev_x = (wRect.left+wRect.right)>>1;
                    prev_y = (wRect.bottom+wRect.top)>>1;
                    do_with_errors(win, gm.touch_move(pt.x, pt.y, prev_x, prev_y));
                    SetCursorPos(prev_x, prev_y);
                }
            } break;
            default: {
                return DefWindowProc(win, msg, wpr, lpr);
            } break;
        }
    else
        return DefWindowProc(win, msg, wpr, lpr);
    return 0;
}

HWND createHWND(LPCWSTR win_name, uint32_t w, uint32_t h) {
    WNDCLASSEXW c;
    ZeroMemory(&c, sizeof(c));

    c.cbSize = sizeof(c);
    c.style = CS_OWNDC;
    c.lpfnWndProc = win_proc;
    c.hInstance = GetModuleHandle(NULL);
    c.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    c.hCursor = LoadCursor(NULL, IDC_ARROW);
    c.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    c.lpszClassName = win_name+1;
    RegisterClassExW(&c);
    HWND win = CreateWindowW( 
        c.lpszClassName,
        win_name,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        w, h,
        NULL, NULL,
        c.hInstance, NULL
    );
	RECT wRect, cRect;
	GetWindowRect(win, &wRect);
	GetClientRect(win, &cRect);
	
	wRect.right  = (wRect.right - wRect.left) - cRect.right  + w;
	wRect.bottom = (wRect.bottom - wRect.top) - cRect.bottom + h;

	MoveWindow(win, wRect.left, wRect.top, wRect.right, wRect.bottom, FALSE);
    return win;
}

int32_t main() {
    std::cout << "Hello world!\n";
    HWND p = createHWND(L"Hello world!", gm.screen_w, gm.screen_h);
	SetWindowLongPtr(p, GWLP_USERDATA, (LONG_PTR)(&gm));
    wgl.setHWND(p);
    wgl.init();
    ShowWindow(p, SW_SHOWNORMAL);
    shks::update();
    do_with_errors(p, gm.init());

    std::chrono::system_clock::time_point new_time = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point old_time = new_time;
    float dt;
    MSG msg;
    do {
        if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            shks::update();
        }
        
        new_time = std::chrono::system_clock::now();
		std::chrono::duration<float> dTime = new_time - old_time;
        dt = dTime.count();
        old_time = new_time;
        std::wstring s = L" time to frame: " + std::to_wstring(dt) + L"tpf, speed: " + std::to_wstring(1.0f/dt) + L" fps ";
        TextOutW(GetDC(p), 0, 0, s.c_str(), s.length());
        do_with_errors(p, gm.loop(dt));
        RedrawWindow(p, nullptr, nullptr, RDW_INTERNALPAINT);

    } while (msg.message != WM_QUIT);
    wgl.finit();
    DestroyWindow(p);
    return 0;
}