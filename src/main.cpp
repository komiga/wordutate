
#include "./Log.hpp"
#include "./InputHandler.hpp"
#include "./Word.hpp"

#include <duct/config.hpp>
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
) /*[[noreturn]]*/;

signed
main();

#if (DUCT_COMPILER & DUCT_FLAG_COMPILER_GCC) ||			\
	((DUCT_COMPILER & DUCT_FLAG_COMPILER_CLANG) &&		\
	  DUCT_COMPILER > DUCT_FLAG_COMPILER_CLANG33)
//
	#define WORDUTATE_IGNORE_OLD_STYLE_CASTS
#endif

static void
init() {
	DUCT_DEBUG_CALLED();
	#ifdef WORDUTATE_IGNORE_OLD_STYLE_CASTS
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wold-style-cast"
	#endif
	std::signal(SIGINT, handle_interrupt);
	#ifdef WORDUTATE_IGNORE_OLD_STYLE_CASTS
		#pragma GCC diagnostic pop
	#endif
}

static void
shutdown() {
	DUCT_DEBUG_CALLED();
}

static void
handle_interrupt(signed) {
	#ifdef WORDUTATE_IGNORE_OLD_STYLE_CASTS
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wold-style-cast"
	#endif
	std::signal(SIGINT, SIG_DFL);
	#ifdef WORDUTATE_IGNORE_OLD_STYLE_CASTS
		#pragma GCC diagnostic pop
	#endif
	std::exit(0);
}

signed
main() {
	InputHandler ihandler;
	do {} while (ihandler.handle_input());
	return 0;
}
