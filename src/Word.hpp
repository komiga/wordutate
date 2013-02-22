
#ifndef WORDUTATE_WORD_HPP_
#define WORDUTATE_WORD_HPP_

#include "./config.hpp"
#include "./aux.hpp"
#include "./String.hpp"
#include "./Console.hpp"

#include <duct/char.hpp>
#include <duct/traits.hpp>
#include <duct/Parser.hpp>
#include <algorithm>
#include <memory>

// Forward declarations
class Word;
class Wordutator;
class PhraseParser;

/**
*/
class Word final
	: duct::traits::restrict_copy {
private:
	typedef aux::unordered_map<duct::char32, unsigned> count_map_type;
	String m_word;
	count_map_type m_counts{};
	ConsoleColor m_fgc{COLOR_CURRENT};
	ConsoleColor m_bgc{COLOR_CURRENT};

public:
/** @name Constructor and destructor */ /// @{
	explicit Word(String word)
		: m_word{std::move(word)}
	{
		refresh_counts();
	}
	~Word()=default;
/// @}

/** @name Properties */ /// @{
	void set_word(String const& word) {
		m_word.assign(word);
		refresh_counts();
	}
	String const& get_word() const { return m_word; }
	std::size_t get_length() const {
		return duct::UTF8Utils::count(m_word.cbegin(), m_word.cend(), true);
	}

	void set_color(ConsoleColor const fgc, ConsoleColor const bgc) {
		m_fgc=fgc;
		m_bgc=bgc;
	}
	void set_color(Word const& other) {
		m_fgc=other.m_fgc;
		m_bgc=other.m_bgc;
	}
	ConsoleColor get_fg_color() const { return m_fgc; }
	ConsoleColor get_bg_color() const { return m_bgc; }

	std::size_t distinct_char_count() const { return m_counts.size(); }
/// @}

/** @name Comparison and operations */ /// @{
	bool has(duct::char32 const cp) const {
		return m_counts.cend()!=m_counts.find(cp);
	}
	bool matches(Word const& other) const;

	void refresh_counts();
	void print(
		bool style,
		ConsoleAttribute const attr=ATTR_BOLD,
		ConsoleColor const ovr_fgc=COLOR_NULL,
		ConsoleColor const ovr_bgc=COLOR_NULL
	) const;
/// @}
};

/**
*/
class Wordutator final
	: duct::traits::restrict_copy {
public:
	typedef aux::vector<std::shared_ptr<Word> > word_vector_type;

private:
	String m_phrase{};
	word_vector_type m_group{};

public:
/** @name Constructor and destructor */ /// @{
	Wordutator()=default;
	~Wordutator()=default;
/// @}

/** @name Properties */ /// @{
	size_t set_phrase(String const& phrase);
	String& get_phrase() { return m_phrase; }
	word_vector_type& get_group() { return m_group; }
	size_t get_count() const { return m_group.size(); }
/// @}

/** @name Comparison & operations */ /// @{
	bool compare(Wordutator& other) const;

	void calc_colors();
	void clear() {
		m_group.clear();
	}
	void print(
		char const prefix[],
		bool const newline=true,
		ConsoleColor const fgc=COLOR_BLUE
	) const;
/// @}
};

/**
*/
class PhraseParser final
	: public duct::Parser
	, duct::traits::restrict_copy {
private:
	Wordutator::word_vector_type* m_group{nullptr};

public:
	PhraseParser()
		: duct::Parser({duct::Encoding::UTF8, duct::Endian::SYSTEM})
	{}
	~PhraseParser()=default;

	bool process(Wordutator::word_vector_type& group, String const& str);

// TODO: Make all of these virtuals private in duct++
private:
	void skip_whitespace();
	bool parse() override;
	void reset() override;
	void discern_token() override;
	void read_token() override;
	void handle_token() override;
	void read_word_token();
	void read_word_span_token();
};

#endif // WORDUTATE_WORD_HPP_
