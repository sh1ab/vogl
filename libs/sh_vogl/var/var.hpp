#ifndef _VAR_HPP_
#define _VAR_HPP_

#include <cinttypes>

namespace vogl {
    namespace var {
        
        template<typename vox_type, const uint32_t _ox, const uint32_t _oy, const uint32_t _oz>
        class var {
        public:
            vox_type* vox_ids;
            var() {
                vox_ids = (vox_type*) malloc(_ox*_oy*_oz*sizeof(vox_type));
                memset(vox_ids, 0, (_ox*_oy*_oz*sizeof(vox_type))>>3);
            }
            ~var() {
                finit();
            }
            uint32_t ox() const { return _ox; }
            uint32_t oy() const { return _oy; }
            uint32_t oz() const { return _oz; }
            vox_type& operator()(const uint32_t& x, const uint32_t& y, const uint32_t& z) {
                return vox_ids[z + _oz*(y + _oy*x)];
            }
            vox_type get(const uint32_t& x, const uint32_t& y, const uint32_t& z) const {
                return vox_ids[z + _oz*(y + _oy*x)];
            }
            void finit() {
                free(vox_ids);
            }
        };
    };
};

#endif //_VAR_HPP_