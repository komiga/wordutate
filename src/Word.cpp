
#include "./stl.hpp"
#include "./String.hpp"
#include "./Log.hpp"
#include "./Word.hpp"

#include <duct/char.hpp>
#include <duct/debug.hpp>
#include <duct/CharacterSet.hpp>
#include <duct/IO.hpp>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <iostream>

namespace {

static duct::CharacterSet const s_set_whitespace{"\t \r\n"};

} // anonymous implementation namespace

// class Word implementation

void Word::refresh_counts() {
	m_counts.clear();
	duct::char32 cp;
	String::const_iterator pos=m_word.cbegin(), next;
	for (; m_word.cend()!=pos; pos=next) {
		next=duct::UTF8Utils::decode(pos, m_word.cend(), cp, 0xFFFD);
		if (next==pos) { // Incomplete sequence; just get out of here
			return;
		} else if (!s_set_whitespace.contains(cp)) {
			count_map_type::iterator const iter=m_counts.find(cp);
			if (m_counts.end()!=iter) {
				++(iter->second);
			} else {
				m_counts[cp]=1;
			}
		}
	}
}

bool Word::matches(Word const& other) const {
	if (distinct_char_count()!=other.distinct_char_count()) { // Quick exit: the words don't share the same number of distinct characters
		return false;
	} else {
		for (auto const& x : m_counts) {
			auto const& iter=other.m_counts.find(x.first);
			if (other.m_counts.cend()==iter || x.second!=iter->second) { // Either other doesn't have character x.first or its count is not equal to x's
				return false;
			}
		}
	}
	return true;
}

void Word::print(bool const style, ConsoleAttribute const attr, ConsoleColor const ovr_fgc, ConsoleColor const ovr_bgc) const {
	if (style) {
		Console::instance()->push(attr, (COLOR_NULL!=ovr_fgc) ? ovr_fgc : m_fgc, (COLOR_NULL!=ovr_bgc) ? ovr_bgc : m_bgc);
		std::cout<<m_word;
		Console::instance()->pop();
	} else {
		std::cout<<m_word;
	}
}

// class Wordutator implementation

namespace {

static PhraseParser s_parser{};

struct ColorPair {
	ConsoleColor fg;
	ConsoleColor bg;
};

ColorPair const s_color_pairs[]={
	// BG black
	{COLOR_RED, COLOR_BLACK},
	{COLOR_GREEN, COLOR_BLACK},
	{COLOR_BLUE, COLOR_BLACK},
	{COLOR_CYAN, COLOR_BLACK},
	{COLOR_MAGENTA, COLOR_BLACK},
	{COLOR_YELLOW, COLOR_BLACK},
	{COLOR_WHITE, COLOR_BLACK},
	// BG blue
	{COLOR_RED, COLOR_BLUE},
	{COLOR_GREEN, COLOR_BLUE},
	{COLOR_CYAN, COLOR_BLUE},
	{COLOR_MAGENTA, COLOR_BLUE},
	{COLOR_YELLOW, COLOR_BLUE},
	{COLOR_WHITE, COLOR_BLUE},
	// BG magenta
	{COLOR_RED, COLOR_MAGENTA},
	{COLOR_GREEN, COLOR_MAGENTA},
	{COLOR_CYAN, COLOR_MAGENTA},
	{COLOR_YELLOW, COLOR_MAGENTA},
	{COLOR_WHITE, COLOR_MAGENTA},
	// BG cyan
	{COLOR_RED, COLOR_CYAN},
	{COLOR_YELLOW, COLOR_CYAN},
	{COLOR_WHITE, COLOR_CYAN},
	// BG white
	{COLOR_RED, COLOR_WHITE},
	{COLOR_BLACK, COLOR_WHITE},
	{COLOR_CYAN, COLOR_WHITE},
	{COLOR_MAGENTA, COLOR_WHITE},
	// BG yellow
	{COLOR_RED, COLOR_YELLOW},
	{COLOR_BLACK, COLOR_YELLOW},
	{COLOR_NULL, COLOR_NULL}
};

} // anonymous implementation namespace

size_t Wordutator::set_phrase(String const& phrase) {
	m_phrase.assign(phrase);
	clear();
	s_parser.process(m_group, m_phrase);
	calc_colors();
	return get_count();
}

void Wordutator::calc_colors() {
	auto iter=m_group.begin();
	auto const* pair=s_color_pairs;
	for (; m_group.end()!=iter; ++iter, ++pair) {
		if (COLOR_NULL==pair->fg) {
			pair=s_color_pairs;
		}
		(*iter)->set_color(pair->fg, pair->bg);
	}
}

bool Wordutator::compare(Wordutator& other) const {
	// Match words
	stl::vector<Word const*> unmatched;
	stl::list<std::shared_ptr<Word> > candidates{other.m_group.begin(), other.m_group.end()};
	for (auto const& word : m_group) {
		auto candidate_iter=candidates.begin();
		for (; candidates.end()!=candidate_iter; ++candidate_iter) {
			if (word->matches(**candidate_iter)) {
				break;
			}
		}
		if (candidates.end()!=candidate_iter) { // Match found, remove from candidates
			(*candidate_iter)->set_color(*word); // Colorize matching word
			candidates.erase(candidate_iter);
		} else { // No match for word
			unmatched.emplace_back(word.get());
		}
	}
	// Any remaining words in candidates were not matchable
	for (auto& word : candidates) {
		word->set_color(COLOR_WHITE, COLOR_RED);
	}

	bool matches=(candidates.empty() && get_count()==other.get_count());
	other.print("compare", false, matches ? COLOR_GREEN : COLOR_RED);

	// Print un-matched original words
	if (!unmatched.empty()) {
		std::putchar(' ');
		Console::instance()->push(ATTR_BOLD);
		Log::msgps("||", ATTR_STRIKE, COLOR_DEFAULT, COLOR_RED);
		Console::instance()->pop();
		std::printf("  ");
		for (auto const word : unmatched) {
			word->print(true);
			std::putchar(' ');
		}
	}
	std::putchar('\n');
	return matches;
}

void Wordutator::print(char const prefix[], bool const newline, ConsoleColor const fgc) const {
	Log::msgps("%s (%-2lu): ", ATTR_BOLD, fgc, COLOR_DEFAULT, prefix, get_count());
	for (auto const& word : m_group) {
		word->print(true);
		std::putchar(' ');
	}
	if (newline) {
		std::putchar('\n');
	}
}

// class PhraseParser implementation

namespace {

enum {
	TOK_WORD=1,
	TOK_WORD_SPAN,
	TOK_PUNCTUATION,
	TOK_EOF
};

} // anonymous implementation namespace

bool PhraseParser::process(Wordutator::word_vector_type& group, String const& str) {
	duct::IO::imemstream stream{str.data(), str.size()};
	if (initialize(stream)) {
		m_group=&group;
		do {} while (parse());
		reset();
		return true;
	}
	return false;
}

void PhraseParser::skip_whitespace() {
	while (m_curchar!=duct::CHAR_EOF && s_set_whitespace.contains(m_curchar)) {
		next_char();
	}
}

void PhraseParser::reset() {
	duct::Parser::reset();
	m_group=nullptr;
}

bool PhraseParser::parse() {
	skip_whitespace();
	discern_token();
	read_token();
	handle_token();
	if (TOK_EOF==m_token.get_type() || duct::CHAR_EOF==m_curchar) {
		return false;
	}
	return true;
}

void PhraseParser::discern_token() {
	m_token.reset(duct::NULL_TOKEN, false);
	m_token.set_position(m_line, m_column);
	switch (m_curchar) {
	case duct::CHAR_EOF			: m_token.set_type(TOK_EOF); break;
	case duct::CHAR_OPENBRACE	: m_token.set_type(TOK_WORD_SPAN); break;
	default:
		m_token.set_type(TOK_WORD);
		break;
	}
}

void PhraseParser::read_token() {
	switch (m_token.get_type()) {
	case TOK_WORD:
		read_word_token();
		break;
	case TOK_WORD_SPAN:
		read_word_span_token();
		break;
	case TOK_EOF:
		break; // Do nothing
	default:
		DUCT_ASSERT(false, "unhandled token");
	}
}

void PhraseParser::handle_token() {
	switch (m_token.get_type()) {
	case TOK_WORD_SPAN:
	case TOK_WORD:
		if (!m_token.get_buffer().compare(s_set_whitespace)) {
			m_group->emplace_back(new Word(m_token.get_buffer().to_string<String>()));
		}
		break;
	default:
		break;
	}
}

void PhraseParser::read_word_token() {
	while (duct::CHAR_EOF!=m_curchar) {
		if (s_set_whitespace.contains(m_curchar)) {
			break;
		} else {
			m_token.get_buffer().push_back(m_curchar);
		}
		next_char();
	}
}

void PhraseParser::read_word_span_token() {
	while (duct::CHAR_EOF!=m_curchar) {
		if (duct::CHAR_OPENBRACE==m_curchar) {
			// Ignore
		} else if (duct::CHAR_CLOSEBRACE==m_curchar) {
			next_char();
			break;
		} else {
			m_token.get_buffer().push_back(m_curchar);
		}
		next_char();
	}
}
