
## What is this?

**wordutate** is a command-line phrase permutation matcher for GNU/Linux (and probably OS X).

… Uh, let me explain by example.

Given the phrase `tea is delicious`, wordutate will match any reordering and permutation of the words. Thus, all of the following match this phrase:

```
is tea delicious
delicious is tea
eat is delicious
tea is ledicious
```

It also allows grouping with braces, where whitespace is ignored in matching. The following input will match `sledgehammer`:

```
{sledge hammer}
{hammer sledge}
```

(If you wanted to match a phrase in any broken-up form, you could set *and* match the entire phrase with braces.)

If an input word matches one in the current phrase, it will share the color of the matched word. Non-matching input words will have a red background and all unmatched words from the current phrase will be underlined to the right.

## Commands

Input commands (`$` and `@`) take arguments immediately after the command (which need not be spaced).

* `$<args>`: set the current phrase.
* `@<args>`: compare current phrase with inputted phrase (can also input the bare phrase — with no `@`).
* `#`: print the current phrase.
* `q`: quit.

## Compiling

You'll need a respectable C++11 compiler (like Clang 3.1+ or GCC 4.7+, which are actually the only two that currently work due to global ctors & dtors), [premake](http://industriousone.com/premake) 4.4+, and [duct++](/komiga/duct-cpp) (see the README in [deps/](/komiga/wordutate/deps)).

Run `build.sh` (or `build.sh _ gmake --clang` for Clang, and set `CC` and `CXX` to `clang` — yay, premake hacks!) and then `make`.

## License

wordutate carries the MIT license, which is conveniently plastered below.

```
Copyright (c) 2012 Tim Howard

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```
