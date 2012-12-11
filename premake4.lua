
-- wordutate premake file

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
	platforms {"x32", "x64"}

-- Project
local proj=project("wordutate")
	proj.language="C++"
	proj.kind="ConsoleApp"

targetname("wordutate")

configuration {"x32"}
	objdir("out/x86/")
	targetdir("bin/x86/")

configuration {"x64"}
	objdir("out/x86_64/")
	targetdir("bin/x86_64/")

configuration {"debug"}
	defines {"DEBUG", "_DEBUG"}
	flags {"ExtraWarnings", "Symbols"}

configuration {"release"}
	defines {"NDEBUG"}
	flags {"ExtraWarnings", "Optimize"}

configuration {"linux" or "clang"}
	buildoptions {"-std=c++0x", "-pedantic", "-Wextra"}

configuration {"clang"}
	buildoptions {"-stdlib=libstdc++"}
	links {
		"stdc++"
	}

configuration {}
	includedirs {
		"deps/duct/",
		"include/"
	}
	files {
		"src/**"
	}
