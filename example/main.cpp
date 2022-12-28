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
#include <thread>

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

sh_vogl::var::var<uint32_t, 1024, 512, 1024> v;
float ang[3];
float pos[3];
float pos_speed = 80;
float mouse_speed = 1;

struct gm : public sh_game::game {
    uint32_t init() {
        for(uint32_t i(v.ox());i--;)
            for(uint32_t k(v.oz());k--;)
                for(uint32_t j(v.oy()*0.5f*(1+sin(0.01f*i + sin(0.01f*k))));j--;)
                    v(i, j, k) = (rand() & 0x00FFFFFF) + 0xFF000000;

        sh_vogl::var::big_var::init();
        sh_vogl::var::big_var::set_var(&v);

        sh_vogl::var::big_var::ubo::set_mod_mat_index(0, 0, 1);sh_vogl::var::big_var::ubo::set_mod_mat_index(0, 1, 0);sh_vogl::var::big_var::ubo::set_mod_mat_index(0, 2, 0);
        sh_vogl::var::big_var::ubo::set_mod_mat_index(1, 0, 0);sh_vogl::var::big_var::ubo::set_mod_mat_index(1, 1, 1);sh_vogl::var::big_var::ubo::set_mod_mat_index(1, 2, 0);
        sh_vogl::var::big_var::ubo::set_mod_mat_index(2, 0, 0);sh_vogl::var::big_var::ubo::set_mod_mat_index(2, 1, 0);sh_vogl::var::big_var::ubo::set_mod_mat_index(2, 2, 1);
        
        sh_vogl::var::big_var::ubo::set_cam_near(0.1f);
        sh_vogl::var::big_var::ubo::set_cam_width(screen_w);
        sh_vogl::var::big_var::ubo::set_cam_height(screen_h);

        pos[0] = 0; pos[1] = 0; pos[2] = 0;
        ang[0] = 0; ang[1] = 0; ang[2] = 0;

        sh_vogl::var::big_var::ubo::load();

        return sh_game::G::OK;
    }
    float dt = 0;
    uint32_t loop(float dt) {
        if(shks::get_key(VK_MENU).held)SetCursor(LoadCursor(NULL, IDC_ARROW));
        else SetCursor(NULL);
        this->dt = dt;
        if (shks::get_key(VK_ESCAPE).pressed)
            return sh_game::G::NICE_END;

        float v[3] = {0, 0, 0};
        v[2] = shks::get_key('W').held - shks::get_key('S').held;
        v[0] = shks::get_key('D').held - shks::get_key('A').held;
        float l = v[0]*v[0] + v[2]*v[2];
        if (l != 0) {
            l = 1.0f/sqrt(l);
            v[0] *= l;
            v[2] *= l;
            l = v[0]*cos(ang[1]) - v[2]*sin(ang[1]);
            v[2] = v[2]*cos(ang[1]) + v[0]*sin(ang[1]);
            v[0] = l;
        }
        v[1] = shks::get_key(VK_SPACE).held - shks::get_key(VK_SHIFT).held;
        pos[0] += v[0]*dt*pos_speed;
        pos[1] += v[1]*dt*pos_speed;
        pos[2] += v[2]*dt*pos_speed;
        
        return sh_game::G::OK;
    }
    float a[3] = {0, 0, 0};
    uint32_t touch_move(int32_t to_x, int32_t to_y, int32_t from_x, int32_t from_y) {
        a[1] += (from_x - to_x) * dt;
        a[0] += (from_y - to_y) * dt;
        ang[0] += a[0]*mouse_speed;
        ang[1] += a[1]*mouse_speed;
        ang[2] += a[2]*mouse_speed;
        a[0]*=0.6f - dt;
        a[1]*=0.6f - dt;
        a[2]*=0.6f - dt;
        if (ang[0] < -1.570796f) ang[0] = -1.570796f;
        if (ang[0] >  1.570796f) ang[0] =  1.570796f;
        if (ang[1] < 0) ang[1] += 6.283185f;
        if (ang[1] > 6.283185f) ang[1] -= 6.283185f;
        if (ang[2] < 0) ang[1] += 6.283185f;
        if (ang[2] > 6.283185f) ang[1] -= 6.283185f;

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
        sh_vogl::var::big_var::ubo::set_cam_pos(pos[0], pos[1], pos[2]);
        sh_vogl::var::big_var::ubo::set_cam_ang(ang[0], ang[1], ang[2]);

        sh_vogl::var::big_var::ubo::set_mod_pos(0, 0, 0);
        sh_vogl::var::big_var::ubo::set_tex(0, 0, 0);
        sh_vogl::var::big_var::ubo::set_tex_size(32, 32, 32);
        sh_vogl::var::big_var::ubo::load();
        sh_vogl::var::big_var::draw();
static float t = 0;
t += dt;
        sh_vogl::var::big_var::ubo::set_mod_pos(0, 64 + 32*sin(t), 0);
        sh_vogl::var::big_var::ubo::set_tex(0, 32, 0);
        sh_vogl::var::big_var::ubo::set_tex_size(32, 32, 32);
        sh_vogl::var::big_var::ubo::load();
        sh_vogl::var::big_var::draw();

        sh_vogl::var::big_var::ubo::set_mod_pos(0, 32*cos(t) - 64, 0);
        sh_vogl::var::big_var::ubo::set_tex(0, 67, 0);
        sh_vogl::var::big_var::ubo::set_tex_size(32, 32, 32);
        sh_vogl::var::big_var::ubo::load();
        sh_vogl::var::big_var::draw();

        for(uint32_t i(2);i--;)
        for(uint32_t j(2);j--;)
        for(uint32_t k(2);k--;){
        uint32_t ggg[] = {(rand() & 0x00FFFFFF) + 0xFF000000};
        sh_vogl::var::big_var::set_vox(i, j, k, ggg);
        }

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