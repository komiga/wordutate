
#include "./Word.hpp"
#include "./PhraseParser.hpp"

#include <duct/debug.hpp>
#include <duct/CharacterSet.hpp>
#include <duct/IO/memstream.hpp>

// class PhraseParser implementation

namespace {

enum {
	TOK_WORD = 1,
	TOK_WORD_SPAN,
	TOK_PUNCTUATION,
	TOK_EOF
};

static duct::CharacterSet const
s_set_whitespace{"\t \r\n"};

} // anonymous namespace

bool
PhraseParser::process(
	Wordutator::word_vector_type& group,
	String const& str
) {
	duct::IO::imemstream stream{str.data(), str.size()};
	if (initialize(stream)) {
		m_group = &group;
		do {} while (parse());
		reset();
		return true;
	}
	return false;
}

void
PhraseParser::skip_whitespace() {
	while (
		m_curchar != duct::CHAR_EOF &&
		s_set_whitespace.contains(m_curchar)
	) {
		next_char();
	}
}

void
PhraseParser::reset() noexcept {
	duct::Parser::reset();
	m_group = nullptr;
}

bool
PhraseParser::parse() {
	skip_whitespace();
	discern_token();
	read_token();
	handle_token();
	if (TOK_EOF == m_token.type() || duct::CHAR_EOF == m_curchar) {
		return false;
	}
	return true;
}

void
PhraseParser::discern_token() {
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

void
PhraseParser::read_token() {
	switch (m_token.type()) {
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

void
PhraseParser::handle_token() {
	switch (m_token.type()) {
	case TOK_WORD_SPAN:
	case TOK_WORD:
		if (!m_token.buffer().compare(s_set_whitespace)) {
			m_group->emplace_back(
				new Word(m_token.buffer().to_string<String>()));
		}
		break;
	default:
		break;
	}
}

void
PhraseParser::read_word_token() {
	while (duct::CHAR_EOF != m_curchar) {
		if (s_set_whitespace.contains(m_curchar)) {
			break;
		} else {
			m_token.buffer().push_back(m_curchar);
		}
		next_char();
	}
}

void
PhraseParser::read_word_span_token() {
	while (duct::CHAR_EOF != m_curchar) {
		if (duct::CHAR_OPENBRACE == m_curchar) {
			// Ignore
		} else if (duct::CHAR_CLOSEBRACE == m_curchar) {
			next_char();
			break;
		} else {
			m_token.buffer().push_back(m_curchar);
		}
		next_char();
	}
}
