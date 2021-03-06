
#pragma once

#include "./config.hpp"
#include "./String.hpp"
#include "./aux.hpp"

#include <duct/traits.hpp>

// Forward declarations
enum ConsoleAttribute : signed;
enum ConsoleColor : signed;
class Console;

/**
*/
enum ConsoleAttribute : signed {
	ATTR_CURRENT = -1,
	ATTR_NONE = 0,
	ATTR_BOLD = 1,
	ATTR_UNDERSCORE = 4,
	ATTR_STRIKE = 9
};

/**
*/
enum ConsoleColor : signed {
	COLOR_FG_OFFSET = 30,
	COLOR_BG_OFFSET = 40,
	COLOR_CURRENT = -1,
	// NOTE: Used in class Word to indicate no override
	COLOR_NULL = -2,
	COLOR_BLACK = 0,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_YELLOW,
	COLOR_BLUE,
	COLOR_MAGENTA,
	COLOR_CYAN,
	COLOR_WHITE,
	_COLOR_PC,
	COLOR_DEFAULT
};

/**
*/
class Console final
	: duct::traits::restrict_copy
{
private:
	struct StyleState final {
		ConsoleAttribute attr;
		ConsoleColor fgc;
		ConsoleColor bgc;
	};
	using stack_type = aux::stack<StyleState>;

	stack_type m_stack{};
	String m_style_str;

	Console()
		: m_style_str("\033[A;3F;4Bm")
	{
		m_stack.push({ATTR_NONE, COLOR_DEFAULT, COLOR_DEFAULT});
	}

	~Console() {
		reset_props();
	}

	static Console* s_instance;

	static void
	init() __attribute__((constructor (210)));

	static void
	shutdown() __attribute__((destructor (210)));

public:
	static Console*
	instance() noexcept {
		return s_instance;
	}

	void push(
		ConsoleAttribute const attr = ATTR_CURRENT,
		ConsoleColor const fgc = COLOR_CURRENT,
		ConsoleColor const bgc = COLOR_CURRENT
	);

	void
	pop();

	void
	clear_line(
		bool const moveup = true
	) const;

private:
	void
	set_props();

	void
	reset_props() const;
};
