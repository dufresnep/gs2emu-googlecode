// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#ifndef ENC_CODEC_HPP
#define ENC_CODEC_HPP

#if defined(_MSC_VER)
#define         uint8_t         unsigned __int8
#define         uint32_t        unsigned __int32
#else
#include <stdint.h>
#endif

#include <cstdlib>

class codec {
public:
       	static const unsigned ITERATOR_START_VAL = 0x4A80B38;
        void reset(uint8_t key);
        void apply(uint8_t* buf, size_t len);
        void limit(uint32_t limit);

private:
        uint8_t  m_key;
        uint8_t  m_offset;
        uint32_t m_iterator;
        uint32_t m_limit;
};

#endif
