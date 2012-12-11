
#ifndef WORDUTATE_INPUT_HPP_
#define WORDUTATE_INPUT_HPP_

#include "./config.hpp"
#include "./String.hpp"
#include "./Word.hpp"

// Forward declarations
enum CommandFlags : unsigned int;
struct Command;
class InputHandler;

/**
*/
enum CommandFlags : unsigned int {
	CMDFLAG_NULL		=1<<0,
	CMDFLAG_START		=1<<1,
	CMDFLAG_ABSOLUTE	=1<<2
};

/**
*/
struct Command {
	String const kernel;
	unsigned int const flags;
	bool (*func)(InputHandler&, String&);
};

/**
*/
class InputHandler {
private:
	String m_input;
	String m_params;
	Wordutator m_wt_current;
	Wordutator m_wt_comparer;

public:
	InputHandler()
		: m_input()
		, m_params()
		, m_wt_current()
		, m_wt_comparer()
	{}

	inline Wordutator& get_current() { return m_wt_current; }
	inline Wordutator& get_comparer() { return m_wt_comparer; }

	bool handle_input();

private:
	bool match_command(Command const& cmd);
};

#endif // WORDUTATE_INPUT_HPP_
