
#include "./Log.hpp"

#include <duct/debug.hpp>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>

// class Log implementation

void Log::init() {
	DUCT_DEBUG_CALLED();
}

void Log::shutdown() {
	DUCT_DEBUG_CALLED();
}

void Log::msgp_va(
	char const* const format,
	ConsoleAttribute const attr,
	ConsoleColor const fg_color,
	ConsoleColor const bg_color,
	bool newline,
	bool keep,
	va_list& va
) {
	Console::instance()->push(attr, fg_color, bg_color);
	std::vprintf(format, va);
	if (!keep) {
		Console::instance()->pop();
	}
	if (newline) {
		std::putchar('\n');
	}
}

void Log::msgp(
	char const* const format,
	ConsoleAttribute const attr,
	ConsoleColor const fg_color,
	ConsoleColor const bg_color,
	...
) {
	va_list va;
	va_start(va, bg_color);
	msgp_va(format, attr, fg_color, bg_color, true, false, va);
	va_end(va);
}

void Log::msgps(
	char const* const format,
	ConsoleAttribute const attr,
	ConsoleColor const fg_color,
	ConsoleColor const bg_color,
	...
) {
	va_list va;
	va_start(va, bg_color);
	msgp_va(format, attr, fg_color, bg_color, false, false, va);
	va_end(va);
}

void Log::msgpsk(
	char const* const format,
	ConsoleAttribute const attr,
	ConsoleColor const fg_color,
	ConsoleColor const bg_color,
	...
) {
	va_list va;
	va_start(va, bg_color);
	msgp_va(format, attr, fg_color, bg_color, false, true, va);
	va_end(va);
}

void Log::msg(char const* const format, ...) {
	va_list va;
	va_start(va, format);
	std::vprintf(format, va);
	va_end(va);
	std::putchar('\n');
}

void Log::error(char const* const format, ...) {
	Console::instance()->push(ATTR_BOLD, COLOR_WHITE, COLOR_RED);
	std::printf("error: ");
	va_list va;
	va_start(va, format);
	std::vprintf(format, va);
	va_end(va);
	Console::instance()->pop();
	std::putchar('\n');
}

void Log::warning(char const* const format, ...) {
	Console::instance()->push(ATTR_BOLD, COLOR_RED);
	std::printf("warning: ");
	va_list va;
	va_start(va, format);
	std::vprintf(format, va);
	va_end(va);
	Console::instance()->pop();
	std::putchar('\n');
}
