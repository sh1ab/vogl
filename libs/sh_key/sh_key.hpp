#ifndef _SH_KEY_H_
#define _SH_KEY_H_

#include <Windows.h>
#include <cstdint>

namespace shks {
	typedef struct k_state {
		bool pressed;
		bool released;
		bool held;
	}k_state;

	int32_t get_x();
	int32_t get_y();

	k_state get_key(uint32_t id);
	void update();
	void set_cursor(int32_t x, int32_t y);
	void mouse_update();
};

#endif //_SH_KEY_H_