remake_find_package(libusb-1.0 CONFIG)
remake_find_package(libudev CONFIG)

remake_include(lib)

remake_add_directories(lib)
remake_add_directories(bin COMPONENT utils)
remake_pkg_config_generate()
