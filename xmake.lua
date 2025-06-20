add_rules("mode.debug", "mode.release")

target("CONCRT140_APP")
    set_kind("shared")
    add_files("src/*.cpp")
    add_syslinks("User32")
    set_languages("c++20")