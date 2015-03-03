/***************************************************************************
 *   Copyright (C) 2013 by Ralf Kaestner                                   *
 *   ralf.kaestner@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>

#include <libudev.h>
#include <libusb.h>

#include <seekthermal/base/singleton.h>

#include "context.h"
#include "error.h"

/*****************************************************************************/
/* Statics                                                                   */
/*****************************************************************************/

SeekThermal::Prototype<SeekThermal::Context>
  SeekThermal::Usb::Context::prototype(new SeekThermal::Usb::Context(), "Usb");

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::Usb::Context::DebugLevels::DebugLevels() {
  (*this)[debugLevelMinimal] = "Minimal";
  (*this)[debugLevelError] = "Error";
  (*this)[debugLevelWarning] = "Warning";
  (*this)[debugLevelVerbose] = "Verbose";
}

SeekThermal::Usb::Context::Context(DebugLevel debugLevel) :
  debugLevel(debugLevel),
  context(0) {
  Error::assert(libusb_init(&context));
  libusb_set_debug(context, this->debugLevel);
}

SeekThermal::Usb::Context::Context(const Context& src) :
  debugLevel(src.debugLevel),
  context(0) {
  Error::assert(libusb_init(&context));
  libusb_set_debug(context, debugLevel);
}

SeekThermal::Usb::Context::~Context() {
  libusb_exit(context);
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

void SeekThermal::Usb::Context::setDebugLevel(DebugLevel debugLevel) {
  this->debugLevel = debugLevel;
  libusb_set_debug(context, this->debugLevel);
}

SeekThermal::Usb::Context::DebugLevel
    SeekThermal::Usb::Context::getDebugLevel() const {
  return debugLevel;
}

std::list<SeekThermal::Usb::Interface>
    SeekThermal::Usb::Context::getInterfaces() const {
  std::list<Interface> interfaces;

  libusb_device** devices;
  size_t numDevices = libusb_get_device_list(context, &devices);
  for (int i = 0; i < numDevices; ++i)
    interfaces.push_back(devices[i]);
  libusb_free_device_list(devices, 1);

  return interfaces;
}

SeekThermal::Usb::Interface* SeekThermal::Usb::Context::getInterface(const
    std::string& address) const {
  Interface* interface = 0;
  std::string busAddress = address;
  struct stat statBuffer;

  if (!stat(address.c_str(), &statBuffer) && S_ISCHR(statBuffer.st_mode)) {
    struct udev* udev = 0;
    struct udev_device* dev = 0;

    udev = udev_new();
    dev = udev_device_new_from_devnum(udev, 'c', statBuffer.st_rdev);

    if (dev) {
      busAddress = udev_device_get_sysattr_value(dev, "busnum");
      busAddress += ':';
      busAddress += udev_device_get_sysattr_value(dev, "devnum");
    }

    udev_unref(udev);
  }

  unsigned char deviceBus, deviceAddress;
  if (sscanf(busAddress.c_str(), "%hhu:%hhu", &deviceBus,
      &deviceAddress) == 2) {
    libusb_device** devices;
    size_t numDevices = libusb_get_device_list(context, &devices);
    for (int i = 0; i < numDevices; ++i)
      if ((libusb_get_bus_number(devices[i]) == deviceBus) &&
        (libusb_get_device_address(devices[i]) == deviceAddress)) {
      interface = new Interface(devices[i]);
      break;
    }
    libusb_free_device_list(devices, 1);
  }

  if (interface)
    return interface;
  else
    throw AddressError(address);
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

SeekThermal::Usb::Context& SeekThermal::Usb::Context::operator=(
    const Context& src) {
  setDebugLevel(src.debugLevel);
  return *this;
}

SeekThermal::Usb::Context* SeekThermal::Usb::Context::clone() const {
  return new Context(*this);
}

std::list<SeekThermal::Pointer<SeekThermal::Device> >
    SeekThermal::Usb::Context::discoverDevices() const {
  std::list<Pointer<Device> > devices;

  const std::map<std::string, Pointer<Device> >&
    prototypes = Singleton<Factory<Device> >::getInstance().getPrototypes();
  std::list<Interface> interfaces = getInterfaces();
  for (std::list<Interface>::const_iterator it = interfaces.begin();
      it != interfaces.end(); ++it)
    for (std::map<std::string, Pointer<Device> >::const_iterator
        jt = prototypes.begin(); jt != prototypes.end(); ++jt)
      if ((it->getDeviceVendorId() == jt->second->getVendorId()) &&
        (it->getDeviceProductId() == jt->second->getProductId())) {
    devices.push_back(jt->second->clone());
    devices.back()->setInterface(new Interface(*it));
  }

  return devices;
}
