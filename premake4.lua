
newoption {
	trigger="clang",
	description="Use Clang in-place of GCC",
}

if _OPTIONS["clang"] then
	premake.gcc.cc="clang"
	premake.gcc.cxx="clang++"
end

-- Solution
solution("wordutate")
	configurations {"debug", "release"}
	platforms {"x64", "x32"}

-- Project
local proj=project("wordutate")
	proj.language="C++"
	proj.kind="ConsoleApp"

targetname("wordutate")

configuration {"x32"}
	objdir("out/")
	targetdir("bin/x32/")

configuration {"x64"}
	objdir("out/")
	targetdir("bin/x64/")

configuration {"debug"}
	defines {"DEBUG", "_DEBUG"}
	flags {"ExtraWarnings", "Symbols"}

configuration {"release"}
	defines {"NDEBUG"}
	flags {"ExtraWarnings", "Optimize"}

configuration {"linux"}
	buildoptions {
		"-pedantic-errors",
		"-Werror",
		"-Wextra",

		"-Wuninitialized",
		"-Winit-self",

		"-Wmissing-field-initializers",
		"-Wredundant-decls",

		"-Wfloat-equal",
		"-Wold-style-cast",

		"-Wnon-virtual-dtor",
		"-Woverloaded-virtual",

		"-Wunused"
	}

configuration {"linux", "not clang"}
	buildoptions {"-std=c++0x"}

configuration {"linux", "clang"}
	buildoptions {"-std=c++11"}
	buildoptions {"-stdlib=libstdc++"}
	links {"stdc++"}

configuration {}
	includedirs {
		"dep/duct/"
	}
	files {
		"src/**"
	}
