
dofile("precore_import.lua")

precore.init(
	nil,
	{
		"opt-clang",
		"c++11-core"
	}
)

-- Solution

precore.make_solution(
	"wordutate",
	{"debug", "release"},
	{"x64", "x32"},
	nil,
	{
		"precore-generic"
	}
)

-- Project

precore.make_project(
	"wordutate",
	"C++", "ConsoleApp",
	"bin/", "out/",
	nil, nil
)

configuration {"x32"}
	targetsuffix(".x86")

configuration {"x64"}
	targetsuffix(".x86_64")

configuration {}
	flags {
		"FatalWarnings"
	}

configuration {"linux"}
	buildoptions {
		"-pedantic-errors",
		"-Wextra",

		"-Wuninitialized",
		"-Winit-self",

		"-Wmissing-field-initializers",
		"-Wredundant-decls",

		"-Wfloat-equal",
		"-Wold-style-cast",

		"-Wnon-virtual-dtor",
		"-Woverloaded-virtual",

		"-Wunused",
		"-Wundef",
	}

configuration {}
	includedirs {
		"dep/duct/"
	}
	files {
		"src/**.cpp"
	}

precore.action_clean("out", "bin")
