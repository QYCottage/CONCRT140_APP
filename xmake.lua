add_rules("mode.debug", "mode.release")

target("IPHLPAPI")
    set_kind("shared")
    add_files("src/*.cpp")
    add_syslinks("User32")
    set_languages("c++20")