// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#include "crypt.h"

void codec::reset(uint8_t key) {
	m_key = key;
	m_offset = 0;
	m_iterator = ITERATOR_START_VAL;
}

void codec::apply(uint8_t* buf, size_t len) {
	const uint8_t* iterator = reinterpret_cast<const uint8_t*>(
		&m_iterator);
 
	for (size_t i = 0; i < len; ++i) {
		const size_t i_ = i + m_offset;
		if (i_ % 4 == 0) {
			m_iterator *= 0x8088405;
			m_iterator += m_key;
			m_offset = 0;
		}
 
		buf[i] ^= iterator[i_%4];
	}
}

