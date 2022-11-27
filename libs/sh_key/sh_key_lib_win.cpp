#include "sh_key.hpp"

namespace shks {
	int16_t m_keyOldState[256] = { 0 };
	int16_t m_keyNewState[256] = { 0 };
	k_state m_keys[256];

	POINT pt = { 0, 0 };

	void get_cur_pos(POINT& pptt) {
		GetCursorPos(&pptt);
		pptt.x = pptt.x;
		pptt.y = pptt.y;
	}

	void update() {
		for (uint16_t i(256); i--;) {
			m_keyNewState[i] = GetAsyncKeyState(i);

			m_keys[i].pressed = false;
			m_keys[i].released = false;

			if (m_keyNewState[i] != m_keyOldState[i])
				if (m_keyNewState[i] & 0x8000) {
					m_keys[i].pressed = !m_keys[i].held;
					m_keys[i].held = true;
				} else {
					m_keys[i].released = true;
					m_keys[i].held = false;
				}

			m_keyOldState[i] = m_keyNewState[i];
		}
		get_cur_pos(pt);
	}

	int32_t get_x() { return pt.x; }
	int32_t get_y() { return pt.y; }

	k_state get_key(uint32_t id) { return m_keys[id]; }

	void set_cursor(int32_t x, int32_t y) {
		SetCursorPos(x, y);
		get_cur_pos(pt);
	}
};