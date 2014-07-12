
#pragma once

#include "./config.hpp"

#include <string>

// UTF-8 container
using String
= std::basic_string<
	char,
	std::char_traits<char>,
	WORDUTATE_ALLOCATOR<char>
>;
