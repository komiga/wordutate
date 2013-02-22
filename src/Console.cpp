
#include "./Console.hpp"

#include <duct/debug.hpp>
#include <cstdio>

// class Console implementation

Console* Console::s_instance=nullptr;

void Console::init() {
	DUCT_DEBUG_CALLED();
	DUCT_ASSERT(nullptr==s_instance,
		"Console already initialized");
	s_instance=new Console();
}

void Console::shutdown() {
	DUCT_DEBUG_CALLED();
	if (nullptr!=s_instance) {
		delete s_instance;
		s_instance=nullptr;
	}
}

void Console::push(
	ConsoleAttribute const attr,
	ConsoleColor const fgc,
	ConsoleColor const bgc
) {
	auto const& prev_style=m_stack.top();
	m_stack.push({
		(0>attr) ? prev_style.attr : attr,
		(0>fgc) ? prev_style.fgc : fgc,
		(0>bgc) ? prev_style.bgc : bgc
	});
	set_props();
}

void Console::pop() {
	// NOTE: Style stack has persistent
	// {ATTR_NONE, COLOR_DEFAULT, COLOR_DEFAULT}
	// at bottom
	DUCT_ASSERT(1<m_stack.size(),
		"Push/pop mismatch. Cannot pop at bottom of stack!");
	m_stack.pop();
	set_props();
}

void Console::set_props() {
	// At default style, we can just reset
	if (1==m_stack.size()) {
		reset_props();
	} else {
		auto const& style=m_stack.top();
		m_style_str[2]='0'+style.attr;
		m_style_str[5]='0'+style.fgc;
		m_style_str[8]='0'+style.bgc;
		std::printf(
			"%.*s",
			static_cast<signed>(m_style_str.size()),
			m_style_str.data()
		);
	}
}

void Console::reset_props() const {
	std::printf("\033[0m");
}

void Console::clear_line(bool moveup) const {
	std::printf("%s\033[2K", (moveup) ? "\033[A" : "");
}
