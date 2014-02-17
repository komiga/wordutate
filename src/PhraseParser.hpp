
#ifndef WORDUTATE_PHRASEPARSER_HPP_
#define WORDUTATE_PHRASEPARSER_HPP_

#include "./config.hpp"
#include "./String.hpp"
#include "./Wordutator.hpp"

#include <duct/traits.hpp>
#include <duct/EncodingUtils.hpp>
#include <duct/Parser.hpp>

// Forward declarations
class PhraseParser;

/**
*/
class PhraseParser final
	: public duct::Parser
	, duct::traits::restrict_copy
{
private:
	Wordutator::word_vector_type* m_group{nullptr};

public:
	PhraseParser()
		: duct::Parser({duct::Encoding::UTF8, duct::Endian::system})
	{}
	~PhraseParser() = default;

	bool
	process(
		Wordutator::word_vector_type& group,
		String const& str
	);

private:
	void
	skip_whitespace();

	bool
	parse() override;

	void
	reset() noexcept override;

	void
	discern_token() override;

	void
	read_token() override;

	void
	handle_token() override;

	void
	read_word_token();

	void
	read_word_span_token();
};

#endif // WORDUTATE_PHRASEPARSER_HPP_
