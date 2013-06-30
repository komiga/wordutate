
#include "./aux.hpp"
#include "./Log.hpp"
#include "./Word.hpp"
#include "./PhraseParser.hpp"
#include "./Wordutator.hpp"

#include <cstdlib>

// class Wordutator implementation

namespace {

static PhraseParser s_parser{};

struct ColorPair {
	ConsoleColor fg;
	ConsoleColor bg;
};

ColorPair const
s_color_pairs[]{
	// BG black
	{COLOR_RED, COLOR_BLACK},
	{COLOR_GREEN, COLOR_BLACK},
	{COLOR_BLUE, COLOR_BLACK},
	{COLOR_CYAN, COLOR_BLACK},
	{COLOR_MAGENTA, COLOR_BLACK},
	{COLOR_YELLOW, COLOR_BLACK},
	{COLOR_WHITE, COLOR_BLACK},
	// BG blue
	{COLOR_RED, COLOR_BLUE},
	{COLOR_GREEN, COLOR_BLUE},
	{COLOR_CYAN, COLOR_BLUE},
	{COLOR_MAGENTA, COLOR_BLUE},
	{COLOR_YELLOW, COLOR_BLUE},
	{COLOR_WHITE, COLOR_BLUE},
	// BG magenta
	{COLOR_RED, COLOR_MAGENTA},
	{COLOR_GREEN, COLOR_MAGENTA},
	{COLOR_CYAN, COLOR_MAGENTA},
	{COLOR_YELLOW, COLOR_MAGENTA},
	{COLOR_WHITE, COLOR_MAGENTA},
	// BG cyan
	{COLOR_RED, COLOR_CYAN},
	{COLOR_YELLOW, COLOR_CYAN},
	{COLOR_WHITE, COLOR_CYAN},
	// BG white
	{COLOR_RED, COLOR_WHITE},
	{COLOR_BLACK, COLOR_WHITE},
	{COLOR_CYAN, COLOR_WHITE},
	{COLOR_MAGENTA, COLOR_WHITE},
	// BG yellow
	{COLOR_RED, COLOR_YELLOW},
	{COLOR_BLACK, COLOR_YELLOW},
	{COLOR_NULL, COLOR_NULL}
};

} // anonymous namespace

std::size_t
Wordutator::set_phrase(
	String const& phrase
) {
	m_phrase.assign(phrase);
	clear();
	s_parser.process(m_group, m_phrase);
	calc_colors();
	return get_count();
}

void
Wordutator::calc_colors() {
	auto iter = m_group.begin();
	auto const* pair = s_color_pairs;
	for (; m_group.end() != iter; ++iter, ++pair) {
		if (COLOR_NULL == pair->fg) {
			pair = s_color_pairs;
		}
		(*iter)->set_color(pair->fg, pair->bg);
	}
}

bool
Wordutator::compare(
	Wordutator& other
) const {
	// Match words
	aux::vector<Word const*> unmatched;
	aux::list<std::shared_ptr<Word> >
		candidates{other.m_group.begin(), other.m_group.end()};

	for (auto const& word : m_group) {
		auto candidate_iter = candidates.begin();
		for (; candidates.end() != candidate_iter; ++candidate_iter) {
			if (word->matches(**candidate_iter)) {
				break;
			}
		}
		if (candidates.end() != candidate_iter) { // Match found
			// Colorize matching word
			(*candidate_iter)->set_color(*word);
			candidates.erase(candidate_iter);
		} else { // No match for word
			unmatched.emplace_back(word.get());
		}
	}
	// Any remaining words in candidates were not matchable
	for (auto& word : candidates) {
		word->set_color(COLOR_WHITE, COLOR_RED);
	}

	bool matches=(candidates.empty() && get_count() == other.get_count());
	other.print("compare", false, matches ? COLOR_GREEN : COLOR_RED);

	// Print un-matched original words
	if (!unmatched.empty()) {
		std::putchar(' ');
		Console::instance()->push(ATTR_BOLD);
		Log::msgps("||", ATTR_STRIKE, COLOR_DEFAULT, COLOR_RED);
		Console::instance()->pop();
		std::printf("  ");
		for (auto const word : unmatched) {
			word->print(true);
			std::putchar(' ');
		}
	}
	std::putchar('\n');
	return matches;
}

void
Wordutator::print(
	char const prefix[],
	bool const newline,
	ConsoleColor const fgc
) const {
	Log::msgps("%s (%-2lu): ",
		ATTR_BOLD, fgc, COLOR_DEFAULT, prefix, get_count());
	for (auto const& word : m_group) {
		word->print(true);
		std::putchar(' ');
	}
	if (newline) {
		std::putchar('\n');
	}
}
