
#pragma once

#include "./config.hpp"
#include "./aux.hpp"
#include "./String.hpp"
#include "./Console.hpp"

#include <duct/traits.hpp>
#include <algorithm>
#include <memory>

// Forward declarations
class Word;
class Wordutator;

/**
*/
class Wordutator final
	: duct::traits::restrict_copy
{
public:
	using word_vector_type = aux::vector<std::shared_ptr<Word> >;

private:
	String m_phrase{};
	word_vector_type m_group{};

public:
/** @name Constructor and destructor */ /// @{
	Wordutator() = default;
	~Wordutator() = default;
/// @}

/** @name Properties */ /// @{
	std::size_t
	set_phrase(
		String const& phrase
	);

	String&
	get_phrase() noexcept {
		return m_phrase;
	}

	word_vector_type&
	get_group() noexcept {
		return m_group;
	}

	std::size_t
	get_count() const noexcept {
		return m_group.size();
	}
/// @}

/** @name Comparison & operations */ /// @{
	bool
	compare(
		Wordutator& other
	) const;

	void
	calc_colors();

	void
	clear() noexcept {
		m_group.clear();
	}

	void
	print(
		char const prefix[],
		bool const newline = true,
		ConsoleColor const fgc = COLOR_BLUE
	) const;
/// @}
};
