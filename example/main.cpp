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

sh_ogl::gl wgl;

void do_with_errors(HWND p, uint32_t error) {
    switch(error) {
        case G::OK: {

        } break;
        case G::NICE_END: {
            PostMessageW(p, WM_CLOSE, G::NICE_END, 0);
        }
        case G::UNDEF_ERR: {
            PostMessageW(p, WM_CLOSE, G::UNDEF_ERR, 0);
        } break;
        default: {

        } break;
    }
}

vogl::var::var<uint32_t, 1024, 512, 1024> v;
float ang[3];
float pos[3];
float pos_speed = 80;
float mouse_speed = 1;

struct gm : public game {
    uint32_t init() {
        for(uint32_t i(v.ox());i--;)
            for(uint32_t k(v.oz());k--;)
                for(uint32_t j(v.oy()*0.5f*(1+sin(0.01f*i + sin(0.01f*k))));j--;)
                    v(i, j, k) = (rand()&0x00FFFFFF) + 0xFF000000;

        vogl::var::big_var::init();
        vogl::var::big_var::set_var(&v);
        
        vogl::var::big_var::ubo::set_mod_mat_index(0, 0, 1);vogl::var::big_var::ubo::set_mod_mat_index(0, 1, 0);vogl::var::big_var::ubo::set_mod_mat_index(0, 2, 0);
        vogl::var::big_var::ubo::set_mod_mat_index(1, 0, 0);vogl::var::big_var::ubo::set_mod_mat_index(1, 1, 1);vogl::var::big_var::ubo::set_mod_mat_index(1, 2, 0);
        vogl::var::big_var::ubo::set_mod_mat_index(2, 0, 0);vogl::var::big_var::ubo::set_mod_mat_index(2, 1, 0);vogl::var::big_var::ubo::set_mod_mat_index(2, 2, 1);
        
        vogl::var::big_var::ubo::set_cam_near(0.1f);
        vogl::var::big_var::ubo::set_cam_width(512);
        vogl::var::big_var::ubo::set_cam_height(512);

        pos[0] = 0; pos[1] = 0; pos[2] = 0;
        pos[0] = 0; ang[1] = -0.4f; ang[2] = 0;

        vogl::var::big_var::ubo::load();
        //shks::set_cursor(100, 100);

        return G::OK;
    }
    uint32_t loop(float dt) {
        if (shks::get_key(VK_ESCAPE).pressed)
            return G::NICE_END;

        float v[3] = {0, 0, 0};
        v[2] = shks::get_key('W').held - shks::get_key('S').held;
        v[0] = shks::get_key('D').held - shks::get_key('A').held;
        float l = v[0]*v[0] + v[2]*v[2];
        if (l != 0) {
            l = 1/sqrt(l);
            v[0] *= l;
            v[2] *= l;
            l    = v[0]*cos(ang[1]) - v[2]*sin(ang[1]);
            v[2] = v[2]*cos(ang[1]) + v[0]*sin(ang[1]);
            v[0] = l;
        }
        v[1] = shks::get_key(VK_SPACE).held - shks::get_key(VK_SHIFT).held;
        pos[0] += v[0]*dt*pos_speed;
        pos[1] += v[1]*dt*pos_speed;
        pos[2] += v[2]*dt*pos_speed;

        float a[3] = {0, 0, 0};
        //static float new_x = shks::get_x();
        //static float new_y = shks::get_y();
        //new_x = shks::get_x();
        //new_y = shks::get_y();
        //a[1] = 100 - new_x;
        a[1] = shks::get_key(VK_LEFT).held - shks::get_key(VK_RIGHT).held;
        //a[0] = 100 - new_y;
        a[0] = shks::get_key(VK_UP).held - shks::get_key(VK_DOWN).held;
        //shks::set_cursor(100, 100);
        ang[0] += a[0]*dt*mouse_speed;
        ang[1] += a[1]*dt*mouse_speed;
        ang[2] += a[2]*dt*mouse_speed;
        
        if (ang[0] < -1.570796f) ang[0] = -1.570796f;
        if (ang[0] >  1.570796f) ang[0] =  1.570796f;
        if (ang[1] < 0) ang[1] += 6.283185f;
        if (ang[1] > 6.283185f) ang[1] -= 6.283185f;
        if (ang[2] < 0) ang[1] += 6.283185f;
        if (ang[2] > 6.283185f) ang[1] -= 6.283185f;

        return G::OK;
    }
    uint32_t draw() {
        wgl.start_draw();
        glClearColor(0.01f, 0.01f, 0.01f, 1);
        vogl::var::big_var::ubo::set_cam_pos(pos[0], pos[1], pos[2]);
        vogl::var::big_var::ubo::set_cam_ang(ang[0], ang[1], ang[2]);
        vogl::var::big_var::ubo::load();
        //vogl::var::big_var::set_var(&v);
        vogl::var::big_var::draw();
        wgl.end_draw();
        return G::OK;
    }
    uint32_t stop() {
        vogl::var::big_var::stop();
        return G::OK;
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
                    case G::NICE_END: {
                        do_with_errors(win, gm.stop());
                        PostQuitMessage(0);
                    } break;
                    case G::UNDEF_ERR: {
                        std::cout<<"ERROR!!!\n";
                        MessageBoxW(NULL, L"Oh, sheet!", L"It's undefined error, it so warst(", MB_ERR_INVALID_CHARS);
                        do_with_errors(win, gm.stop());
                    } break;
                };
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
	
	wRect.right  += w - cRect.right - wRect.left;
	wRect.bottom += h - cRect.bottom - wRect.top;
	
	MoveWindow(win, wRect.left, wRect.top, wRect.right, wRect.bottom, FALSE);
    return win;
}

int32_t main() {
    std::cout << "Hello world!\n";
    HWND p = createHWND(L"Hello world!", 1080, 1920);
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
        std::wstring s = L" ms to frame: " + std::to_wstring(dt) + L" mspf, speed: " + std::to_wstring(1000.0f/dt) + L" fps ";
        TextOutW(GetDC(p), 0, 0, s.c_str(), s.length());
        do_with_errors(p, gm.loop(dt));
        RedrawWindow(p, nullptr, nullptr, RDW_INTERNALPAINT);

    } while (msg.message != WM_QUIT);
    wgl.finit();
    DestroyWindow(p);
    return 0;
}