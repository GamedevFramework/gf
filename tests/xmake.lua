option("tests", { description = "Build tests", default = true })

if has_config("tests") then
    set_group("Tests")

    target("gf_core_tests")
        set_kind("binary")
        set_languages("cxx17")
        add_files("testCirc.cc", "testDice.cc", "testFlags.cc", "testId.cc", "testMatrix.cc", "testMatrix2.cc", "testRange.cc", "testRect.cc", "testSerialization.cc", "testSingleton.cc", "testSpatial.cc", "testSpan.cc", "testVector.cc", "testVector1.cc", "testVector2.cc", "testVector3.cc", "testVector4.cc")
        add_files("main.cc")
        add_deps("gfcore0")
        add_packages("gtest")
        add_tests("default")

    target("gf_network_tests")
        set_kind("binary")
        set_languages("cxx17")
        add_files("testSocket.cc")
        add_files("main.cc")
        add_deps("gfnet0")
        add_packages("gtest")
        add_tests("default")

end
