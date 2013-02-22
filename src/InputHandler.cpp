
#include "./String.hpp"
#include "./Log.hpp"
#include "./InputHandler.hpp"
#include "./Word.hpp"

#include <duct/debug.hpp>
#include <cstdio>
#include <algorithm>
#include <functional>

void compare_phrase(String const& phrase);
bool cmd_set(InputHandler& input_handler, String& params);
bool cmd_compare(InputHandler& input_handler, String& params);
bool cmd_print(InputHandler& input_handler, String&);
bool cmd_exit(InputHandler&, String&);

Command const s_commands[]{
	{"$", CMDFLAG_START|CMDFLAG_ABSOLUTE, cmd_set},
	{"@", CMDFLAG_START, cmd_compare},
	{"#", CMDFLAG_ABSOLUTE, cmd_print},
	{"q", CMDFLAG_ABSOLUTE, cmd_exit},
	{"", CMDFLAG_NULL, nullptr}
};

bool InputHandler::handle_input() {
	std::getline(std::cin, m_input);
	if (0<m_input.size()) {
		for (auto const* cmd=s_commands; nullptr!=cmd->func; ++cmd) {
			if (match_command(*cmd)) {
				m_params.assign(
					m_input, cmd->kernel.size(),
					m_input.size() - cmd->kernel.size());
				return cmd->func(*this, m_params);
			}
		}
		cmd_compare(*this, m_input);
	}
	return true;
}

bool InputHandler::match_command(Command const& cmd) {
	if (CMDFLAG_ABSOLUTE&cmd.flags) {
		if (0==m_input.compare(cmd.kernel)) {
			return true;
		}
	}
	if (CMDFLAG_START&cmd.flags) {
		// If input starts with kernel
		if (0==m_input.find(cmd.kernel)) {
			return true;
		}
	}
	return false;
}

bool cmd_compare(InputHandler& input_handler, String& params) {
	auto& current=input_handler.get_current();
	auto& comparer=input_handler.get_comparer();
	if (0<current.get_count()) {
		comparer.set_phrase(params);
		Console::instance()->clear_line(true);
		if (current.compare(comparer)) {
			//Log::msgp("matching", ATTR_BOLD, COLOR_WHITE, COLOR_GREEN);
		} else {
			//Log::msgp("unmatching", ATTR_BOLD, COLOR_WHITE, COLOR_RED);
		}
	} else {
		Log::error("no current phrase");
	}
	//std::putchar('\n');
	return true;
}

bool cmd_set(InputHandler& input_handler, String& params) {
	size_t const size=input_handler.get_current().set_phrase(params);
	Console::instance()->clear_line(true);
	if (0<size) {
		input_handler.get_current().print("phrase ");
	} else {
		Log::msgp("cleared phrase", ATTR_BOLD, COLOR_BLUE);
	}
	return true;
}

bool cmd_print(InputHandler& input_handler, String&) {
	Console::instance()->clear_line(true);
	input_handler.get_current().print("phrase ");
	return true;
}

bool cmd_exit(InputHandler&, String&) {
	return false;
}
