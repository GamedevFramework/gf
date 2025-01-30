set_project("gf")
set_version("1.3.0", { soname = true })

add_requires("boost", "freetype", "libsdl", "opengl-headers", "pugixml", "zlib")
add_requires("gtest", { configs = { gmock = false }})
add_requires("stb", { system = false })

add_rules("mode.coverage", "mode.debug", "mode.releasedbg", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "$(buildir)"})

if is_mode("sanitizers") then
    set_symbols("debug")
    set_optimize("none")
    set_policy("build.sanitizer.address", true)
    set_policy("build.sanitizer.undefined", true)
end

set_policy("build.warning", true)
set_warnings("allextra")
set_languages("cxx17")
set_encodings("utf-8")
set_exceptions("cxx")

if is_plat("windows") then
    add_cxflags("/wd4251") -- Disable warning: class needs to have dll-interface to be used by clients of class blah blah blah
    add_defines("NOMINMAX", "_CRT_SECURE_NO_WARNINGS")
end

set_configdir("$(buildir)/config")
set_configvar("GF_VERSION", "1.3.0")
set_configvar("GF_VERSION_MAJOR", "1")
set_configvar("GF_VERSION_MINOR", "3")
set_configvar("GF_VERSION_PATCH", "0")


target("gfcore0")
  add_configfiles("$(projectdir)/library/config.h.in", {pattern = "@(.-)@"})
    set_kind("$(kind)")
    if is_kind("static") then
        add_defines("GF_STATIC", { public = true })
    else
        add_defines("GF_BUILD")
    end
    add_defines("ZLIB_CONST")
    add_files("library/core/*.cc")
    add_headerfiles("include/(gf/*.h)")
    add_includedirs("include", { public = true })
    add_includedirs("include-priv")
    add_includedirs("$(buildir)/config")
    add_packages("pugixml", "stb")
    add_packages("boost", "zlib", { public = true })
    set_license("Zlib")

target("gf0")
    set_kind("$(kind)")
    if is_kind("static") then
        add_defines("GF_STATIC", { public = true })
    else
        add_defines("GF_BUILD")
    end
    add_files("library/graphics/*.cc", "library/graphics/data/generated.cc", "library/vendor/glad/gles20/src/glad.cc")
    add_headerfiles("include/(gf/*.h)")
    add_includedirs("include", { public = true })
    add_includedirs("include-priv")
    add_includedirs("$(buildir)/config")
    add_includedirs("library/vendor/glad/gles20/include")
    add_packages("opengl-headers", "libsdl", "freetype")
    add_deps("gfcore0")
    set_license("Zlib")

target("gfnet0")
    set_kind("$(kind)")
    if is_kind("static") then
        add_defines("GF_STATIC", { public = true })
    else
        add_defines("GF_BUILD")
    end
    add_files("library/net/*.cc")
    add_headerfiles("include/(gf2/network/*.h)")
    add_includedirs("include", { public = true })
    add_includedirs("include-priv")
    add_deps("gfcore0")
    if is_plat("linux") then
        add_defines("_POSIX_C_SOURCE=200809L")
    end
    if is_plat("windows") then
        add_syslinks("ws2_32")
    end
    set_license("Zlib")

includes("tests/xmake.lua")
