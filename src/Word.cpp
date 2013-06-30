
#include "./aux.hpp"
#include "./String.hpp"
#include "./Log.hpp"
#include "./Word.hpp"

#include <duct/char.hpp>
#include <duct/CharacterSet.hpp>
#include <iostream>

// class Word implementation

namespace {

static duct::CharacterSet const s_set_whitespace{"\t \r\n"};

} // anonymous namespace

void
Word::refresh_counts() {
	m_counts.clear();
	duct::char32 cp = duct::CHAR_REPLACEMENT;
	String::const_iterator pos = m_word.cbegin(), next;
	for (; m_word.cend() != pos; pos = next) {
		next = duct::UTF8Utils::decode(
			pos, m_word.cend(), cp, duct::CHAR_REPLACEMENT);
		if (next == pos) { // Incomplete sequence; just get out of here
			return;
		} else if (!s_set_whitespace.contains(cp)) {
			count_map_type::iterator const iter = m_counts.find(cp);
			if (m_counts.end() != iter) {
				++(iter->second);
			} else {
				m_counts[cp] = 1u;
			}
		}
	}
}

bool
Word::matches(
	Word const& other
) const {
	if (distinct_char_count() != other.distinct_char_count()) {
		// Quick exit: the words don't share the same number
		// of distinct characters
		return false;
	} else {
		for (auto const& x : m_counts) {
			auto const& iter = other.m_counts.find(x.first);
			// Either other doesn't have character x.first or
			// its count is not equal to x's
			if (other.m_counts.cend() == iter || x.second != iter->second) {
				return false;
			}
		}
	}
	return true;
}

void
Word::print(
	bool const style,
	ConsoleAttribute const attr,
	ConsoleColor const ovr_fgc,
	ConsoleColor const ovr_bgc
) const {
	if (style) {
		Console::instance()->push(
			attr,
			(COLOR_NULL != ovr_fgc) ? ovr_fgc : m_fgc,
			(COLOR_NULL != ovr_bgc) ? ovr_bgc : m_bgc
		);
		std::cout << m_word;
		Console::instance()->pop();
	} else {
		std::cout << m_word;
	}
}
