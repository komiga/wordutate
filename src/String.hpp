
#ifndef WORDUTATE_STRING_HPP_
#define WORDUTATE_STRING_HPP_

#include "./config.hpp"

#include <string>

using String=std::basic_string<char, std::char_traits<char>, WORDUTATE_CONFIG_STL_ALLOCATOR<char> >;

#endif // WORDUTATE_STRING_HPP_
