
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
	"murk",
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

if "clean" == _ACTION then
	for _, pc_sol in pairs(precore.state.solutions) do
		for _, pc_proj in pairs(pc_sol.projects) do
			os.rmdir(path.join(pc_proj.obj.basedir, "out"))
			os.rmdir(path.join(pc_proj.obj.basedir, "bin"))
		end
	end
end
