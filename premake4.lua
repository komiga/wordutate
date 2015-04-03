
dofile("precore_import.lua")
local S, G, R = precore.helpers()

precore.init(
	nil,
	{
		"precore.clang-opts",
		"precore.c++11-core",
		"precore.env-common",
	}
)

precore.import(G"${DEP_PATH}/duct")

precore.make_solution(
	"wordutate",
	{"debug", "release"},
	{"native"},
	nil,
	{
		"precore.generic",
	}
)

precore.make_project(
	"wordutate",
	"C++", "ConsoleApp",
	"bin/", "out/",
	nil, {
		"duct.dep",
	}
)

configuration {"linux"}
	targetsuffix(".elf")

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
	files {
		"src/**.cpp"
	}

precore.action_clean("out", "bin")
