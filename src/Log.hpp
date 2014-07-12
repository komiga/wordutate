
#pragma once

#include "./Console.hpp"

#include <duct/traits.hpp>
#include <cstdarg>

/**
*/
class Log final
	: duct::traits::restrict_all
{
private:
	static void
	init() __attribute__((constructor (220)));

	static void
	shutdown() __attribute__((destructor (220)));

	static void
	msgp_va(
		char const* const format,
		ConsoleAttribute const attr,
		ConsoleColor const fg_color,
		ConsoleColor const bg_color,
		bool newline,
		bool keep,
		va_list& va
	);

public:
	static void
	msgp(
		char const* const format,
		ConsoleAttribute const attr=ATTR_NONE,
		ConsoleColor const fg_color=COLOR_DEFAULT,
		ConsoleColor const bg_color=COLOR_DEFAULT,
		...
	);

	static void
	msgps(
		char const* const format,
		ConsoleAttribute const attr=ATTR_NONE,
		ConsoleColor const fg_color=COLOR_DEFAULT,
		ConsoleColor const bg_color=COLOR_DEFAULT,
		...
	);

	static void
	msgpsk(
		char const* const format,
		ConsoleAttribute const attr=ATTR_NONE,
		ConsoleColor const fg_color=COLOR_DEFAULT,
		ConsoleColor const bg_color=COLOR_DEFAULT,
		...
	);

	static void
	msg(
		char const* const format,
		...
	);

	static void
	error(
		char const* const format,
		...
	);

	static void
	warning(
		char const* const format,
		...
	);

};
