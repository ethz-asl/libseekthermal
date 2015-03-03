remake_project_header(config.h.remake)

remake_add_directories()

remake_add_library(
  seekthermal PREFIX OFF
  LINK base command
  FORCE_LINK usb aaa
)
remake_add_headers()
remake_pkg_config_generate()
