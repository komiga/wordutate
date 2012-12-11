
#ifndef WORDUTATE_STL_HPP_
#define WORDUTATE_STL_HPP_

#include "./config.hpp"

#include <stack>
#include <deque>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace stl {

template<typename T>
using deque=std::deque<T, WORDUTATE_CONFIG_STL_ALLOCATOR<T> >;

template<typename T, class Container=deque<T> >
using stack=std::stack<T, Container>;

template<typename T>
using vector=std::vector<T, WORDUTATE_CONFIG_STL_ALLOCATOR<T> >;

template<typename Key, typename T, class Hash=std::hash<Key>, class KeyEqual=std::equal_to<Key> >
using unordered_map=std::unordered_map<Key, T, Hash, KeyEqual, WORDUTATE_CONFIG_STL_ALLOCATOR<std::pair<Key const, T>> >;

template<typename Key, class Hash=std::hash<Key>, class KeyEqual=std::equal_to<Key> >
using unordered_set=std::unordered_set<Key, Hash, KeyEqual, WORDUTATE_CONFIG_STL_ALLOCATOR<Key> >;

}

#endif // WORDUTATE_STL_HPP_
