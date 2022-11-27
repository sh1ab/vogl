#ifndef _PIPE_LINE_SHADER_HPP_
#define _PIPE_LINE_SHADER_HPP_

#include <cinttypes>

namespace sh_ogl {
	typedef struct ppl_prog {
		uint32_t id = 0;
		uint32_t* progs = 0;

		void create(uint32_t type, uint32_t typeBIT, const char* text, uint32_t num);
		void gen(uint32_t shads);
		void bind();
		void unbind();
		void finit();
	} ppl_prog;
};

#endif //_PIPE_LINE_SHADER_HPP_