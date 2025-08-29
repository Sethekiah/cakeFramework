--[[
TODO:

- Setup output to be a dependency of a "testing" file so that the output is a .a
  file
]]

-- Source Files
units = "src/*.cpp"
components = "src/*.cppm"

set_toolchains("clang")
set_languages("c++latest")
set_runtimes("c++_static")
set_config("sdk", "/usr/lib/llvm-20/")

-- Latest SFML version
add_requires("sfml 3.0.0")

-- Build std module
target("std")
	set_kind("static")
	add_files("/usr/lib/llvm-20/share/libc++/v1/*.cppm", { public = true })

target("a")
	set_kind("binary")
	set_extension(".out")
	
	-- Add dependency on std module
	add_deps("std")
	
	-- Add SFML package
	add_packages("sfml")

	-- Add all source files
	add_files(units)
	add_files(components)

-- set_policy("build.c++.modules", true)