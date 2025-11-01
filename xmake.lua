add_rules("mode.debug", "mode.release")

set_runtimes("MD")
target("WINHTTP")
    set_kind("shared")
    add_files("src/*.cpp","src/*.def","src/*.asm")
    set_languages("c++20")
    add_syslinks("Shell32")