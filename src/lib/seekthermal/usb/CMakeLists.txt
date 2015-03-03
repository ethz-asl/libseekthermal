remake_include(${LIBUDEV_INCLUDEDIR})
remake_include(${LIBUSB_1_0_INCLUDE_DIRS})

remake_add_library(usb LINK ${LIBUDEV_LIBRARIES} ${LIBUSB_1_0_LIBRARIES})
remake_add_headers(INSTALL usb)
