
#ifndef WORDUTATE_WORD_HPP_
#define WORDUTATE_WORD_HPP_

#include "./config.hpp"
#include "./aux.hpp"
#include "./String.hpp"
#include "./Console.hpp"

#include <duct/char.hpp>
#include <duct/traits.hpp>
#include <duct/EncodingUtils.hpp>

// Forward declarations
class Word;

/**
*/
class Word final
	: duct::traits::restrict_copy
{
private:
	using count_map_type = aux::unordered_map<duct::char32, unsigned>;
	String m_word;
	count_map_type m_counts{};
	ConsoleColor m_fgc{COLOR_CURRENT};
	ConsoleColor m_bgc{COLOR_CURRENT};

public:
/** @name Constructor and destructor */ /// @{
	explicit
	Word(
		String word
	)
		: m_word(std::move(word))
	{
		refresh_counts();
	}

	~Word() = default;
/// @}

/** @name Properties */ /// @{
	void
	set_word(
		String const& word
	) {
		m_word.assign(word);
		refresh_counts();
	}

	String const&
	get_word() const noexcept {
		return m_word;
	}

	std::size_t
	get_length() const noexcept {
		return duct::UTF8Utils::count(
			m_word.cbegin(), m_word.cend(),
			true
		);
	}

	void
	set_color(
		ConsoleColor const fgc,
		ConsoleColor const bgc
	) noexcept {
		m_fgc = fgc;
		m_bgc = bgc;
	}

	void
	set_color(
		Word const& other
	) noexcept {
		m_fgc = other.m_fgc;
		m_bgc = other.m_bgc;
	}

	ConsoleColor
	get_fg_color() const noexcept {
		return m_fgc;
	}

	ConsoleColor
	get_bg_color() const noexcept {
		return m_bgc;
	}

	std::size_t
	distinct_char_count() const noexcept {
		return m_counts.size();
	}
/// @}

/** @name Comparison and operations */ /// @{
	bool
	has(
		duct::char32 const cp
	) const {
		return m_counts.cend() != m_counts.find(cp);
	}

	bool
	matches(
		Word const& other
	) const;

	void
	refresh_counts();

	void
	print(
		bool style,
		ConsoleAttribute const attr = ATTR_BOLD,
		ConsoleColor const ovr_fgc = COLOR_NULL,
		ConsoleColor const ovr_bgc = COLOR_NULL
	) const;
/// @}
};

#endif // WORDUTATE_WORD_HPP_
