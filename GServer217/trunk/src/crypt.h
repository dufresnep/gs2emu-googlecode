// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#ifndef ENC_CODEC_HPP
#define ENC_CODEC_HPP

#include <stdint.h>
#include <cstdlib>

class codec {
public:
	static const unsigned ITERATOR_START_VAL = 0x4A80B38;
	void reset(uint8_t key);
	void apply(uint8_t* buf, size_t len);
 
private:
	uint8_t  m_key;
	uint8_t  m_offset;
	uint32_t m_iterator;
}; 
 
#endif
