
#ifndef WORDUTATE_WORD_HPP_
#define WORDUTATE_WORD_HPP_

#include "./config.hpp"
#include "./stl.hpp"
#include "./String.hpp"
#include "./Console.hpp"

#include <duct/char.hpp>
#include <duct/Parser.hpp>
#include <algorithm>
#include <memory>

// Forward declarations
class Word;
class Wordutator;
class PhraseParser;

/**
*/
class Word {
public:
	typedef stl::unordered_map<duct::char32, unsigned int> count_map_type;

private:
	String m_word;
	count_map_type m_counts;
	ConsoleColor m_fgc;
	ConsoleColor m_bgc;

public:
/** @name Constructor and destructor */ /// @{
	explicit Word(String word)
		: m_word(word)
		, m_counts()
		, m_fgc(COLOR_CURRENT)
		, m_bgc(COLOR_CURRENT)
	{
		refresh_counts();
	}
	~Word() {
		m_counts.clear();
	}
/// @}

/** @name Properties */ /// @{
	inline void set_word(String const& word) {
		m_word.assign(word);
		refresh_counts();
	}
	inline String const& get_word() const { return m_word; }
	inline std::size_t get_length() const {
		return duct::UTF8Utils::count(m_word.cbegin(), m_word.cend(), true);
	}

	inline void set_color(ConsoleColor const fgc, ConsoleColor const bgc) {
		m_fgc=fgc;
		m_bgc=bgc;
	}
	inline void set_color(Word const& other) {
		m_fgc=other.m_fgc;
		m_bgc=other.m_bgc;
	}
	inline ConsoleColor get_fg_color() const { return m_fgc; }
	inline ConsoleColor get_bg_color() const { return m_bgc; }

	inline count_map_type& get_counts() { return m_counts; }
	std::size_t distinct_letter_count() const { return m_counts.size(); }
/// @}

/** @name Comparison and operations */ /// @{
	inline bool has(duct::char32 const cp) const {
		return m_counts.cend()!=m_counts.find(cp);
	}
	bool matches(Word const& other) const;

	void refresh_counts();
	void print(bool style=true, ConsoleAttribute const attr=ATTR_BOLD, ConsoleColor const ovr_fgc=COLOR_NULL, ConsoleColor const ovr_bgc=COLOR_NULL) const;
/// @}
};

/**
*/
class Wordutator {
public:
	typedef stl::vector<std::shared_ptr<Word> > word_vector_type;

private:
	typedef stl::unordered_map<std::shared_ptr<Word>, word_vector_type> word_match_map_type;

	String m_phrase;
	word_vector_type m_group;

public:
/** @name Constructor and destructor */ /// @{
	Wordutator()
		: m_phrase()
		, m_group()
	{}
	~Wordutator() {
		clear();
	}
/// @}

/** @name Properties */ /// @{
	size_t set_phrase(String const& phrase);
	inline String& get_phrase() { return m_phrase; }
	inline word_vector_type& get_group() { return m_group; }
	inline size_t get_count() const { return m_group.size(); }
/// @}

/** @name Comparison & operations */ /// @{
	bool compare(Wordutator const& other) const;

	void calc_colors();
	inline void clear() {
		m_group.clear();
	}
	void print(char const prefix[], bool const newline=true, ConsoleColor const fgc=COLOR_BLUE) const;
/// @}

private:
	void match_words(word_match_map_type& map, Wordutator const& with) const;
};

/**
*/
class PhraseParser : public duct::Parser {
private:
	Wordutator::word_vector_type* m_group;

public:
	PhraseParser()
		: duct::Parser(duct::Encoding::UTF8, duct::Endian::SYSTEM)
		, m_group(nullptr)
	{}

	bool process(Wordutator::word_vector_type& group, String const& str);

// TODO: Make all of these virtuals private in duct++
private:
	void skip_whitespace();
	virtual bool parse();
	virtual void reset();
	virtual void discern_token();
	virtual void read_token();
	virtual void handle_token();
	void read_word_token();
	void read_word_span_token();
};

#endif // WORDUTATE_WORD_HPP_
