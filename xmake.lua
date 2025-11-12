-- Copyright 2025 Caleb Whitmer

-- Source Files
components = "src/*.cppm"
sandbox_units = "src/sandbox/*.cpp"
sandbox_components = "src/sandbox/*.cppm"

-- set_toolchains("clang")
set_languages("c++latest")
set_runtimes("c++_static")
-- set_config("sdk", "/usr/lib/llvm-20/")

-- Latest SFML version
add_requires("sfml 3.0.1")

-- Build std module
-- target("std")
-- 	set_kind("static")
-- 	add_files("/usr/lib/llvm-20/share/libc++/v1/*.cppm", { public = true })

target("cakeFramework")
    set_policy("build.c++.modules", true)

	set_kind("static")
	
	-- Add dependency on std module
	-- add_deps("std")
	
	-- Add SFML package
	add_packages("sfml")

	-- Add all source files making them public
	add_files(components, { public = true })

target("a")
    set_policy("build.c++.modules", true)
    
	set_kind("binary")
	set_extension(".out")

	-- add_deps("std")
	add_deps("cakeFramework")

	add_packages("sfml")

	add_files(sandbox_units)
	add_files(sandbox_components)
	