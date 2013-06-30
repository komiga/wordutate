
#include "./Log.hpp"
#include "./InputHandler.hpp"
#include "./Word.hpp"

#include <duct/debug.hpp>
#include <csignal>
#include <cstdio>
#include <cstdlib>

static void
init() __attribute__((constructor (999)));

static void
shutdown() __attribute__((destructor (999)));

static void
handle_interrupt(
	signed
) [[noreturn]];

signed
main();

static void
init() {
	DUCT_DEBUG_CALLED();
	std::signal(SIGINT, handle_interrupt);
}

static void
shutdown() {
	DUCT_DEBUG_CALLED();
}

static void
handle_interrupt(signed) {
	std::signal(SIGINT, SIG_DFL);
	std::exit(0);
}

signed
main() {
	InputHandler ihandler;
	do {} while (ihandler.handle_input());
	return 0;
}
