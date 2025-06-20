add_rules("mode.debug", "mode.release")

set_runtimes("MD")
target("CONCRT140_APP")
    set_kind("shared")
    add_files("src/*.cpp")
    set_languages("c++20")