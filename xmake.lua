--[[
TODO:

- Setup output to be a dependency of a "testing" file so that the output is a .a
  file
]]

-- Source Files
units = "src/sandbox/*.cpp"
components = "src/*.cppm"

set_toolchains("clang")
set_languages("c++latest")
set_runtimes("c++_static")
set_config("sdk", "/usr/lib/llvm-20/")


-- Latest SFML version
add_requires("sfml 3.0.1")
-- add_requires("sfml")

-- Build std module
target("std")
	set_kind("static")
	add_files("/usr/lib/llvm-20/share/libc++/v1/*.cppm", { public = true })

target("cakeFramework")
	set_kind("static")
	
	-- Add dependency on std module
	add_deps("std")
	
	-- Add SFML package
	add_packages("sfml")

	-- Add all source files making them public
	add_files(components, { public = true })

target("a")
	set_kind("binary")
	set_extension(".out")

	add_deps("std")

	add_deps("cakeFramework")

	add_packages("sfml")

	add_files(units)