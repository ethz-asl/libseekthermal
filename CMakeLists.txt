cmake_minimum_required(VERSION 2.6)

include(ReMake)

ADD_DEFINITIONS( -std=c++0x)

remake_project(
  libseekthermal
  VERSION 0.1
  RELEASE beta
  SUMMARY "Driver library for Seek Thermal imaging devices."
  AUTHOR "Ralf Kaestner"
  CONTACT "ralf.kaestner@gmail.com"
  HOME "http://github.com/ethz-asl/libseekthermal"
  LICENSE "GNU Lesser General Public License (LGPL)"
  README README.md
  PREFIX seekthermal-
  INSTALL /usr
  HEADER_DESTINATION include/seekthermal
  EXTRA_CXX_FLAGS -std=c++0x
)

remake_component(
  udev
  CONFIGURATION_DESTINATION /etc/udev
)
remake_add_directories(
  rules
  COMPONENT udev
)
remake_doc(
  html
)

