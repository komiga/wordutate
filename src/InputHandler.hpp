
#ifndef WORDUTATE_INPUT_HPP_
#define WORDUTATE_INPUT_HPP_

#include "./config.hpp"
#include "./String.hpp"
#include "./Wordutator.hpp"

#include <duct/traits.hpp>

// Forward declarations
enum CommandFlags : unsigned;
struct Command;
class InputHandler;

/**
*/
enum CommandFlags : unsigned {
	CMDFLAG_NULL		= 1 << 0,
	CMDFLAG_START		= 1 << 1,
	CMDFLAG_ABSOLUTE	= 1 << 2
};

/**
*/
struct Command final {
	String const kernel;
	unsigned const flags;
	// (handler, params)
	bool
	(*func)(
		InputHandler&, String&
	);
};

/**
*/
class InputHandler final
	: duct::traits::restrict_copy
{
private:
	String m_input{};
	String m_params{};
	Wordutator m_wt_current{};
	Wordutator m_wt_comparer{};

public:
	InputHandler() = default;

	Wordutator&
	get_current() noexcept {
		return m_wt_current;
	}

	Wordutator&
	get_comparer() noexcept {
		return m_wt_comparer;
	}

	bool
	handle_input();

private:
	bool
	match_command(
		Command const& cmd
	);
};

#endif // WORDUTATE_INPUT_HPP_
