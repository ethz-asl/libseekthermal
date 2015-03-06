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

#include <iomanip>
#include <sstream>

#include <libusb.h>

#include <seekthermal/base/singleton.h>
#include <seekthermal/base/factory.h>

#include "interface.h"
#include "error.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::Usb::Interface::DeviceClasses::DeviceClasses() {
  (*this)[LIBUSB_CLASS_PER_INTERFACE] = "Specific";
  (*this)[LIBUSB_CLASS_AUDIO] = "Audio";
  (*this)[LIBUSB_CLASS_COMM] = "Communication";
  (*this)[LIBUSB_CLASS_HID] = "Hid";
  (*this)[LIBUSB_CLASS_PRINTER] = "Printer";
  (*this)[LIBUSB_CLASS_MASS_STORAGE] = "Storage";
  (*this)[LIBUSB_CLASS_HUB] = "Hub";
  (*this)[LIBUSB_CLASS_DATA] = "Data";
  (*this)[LIBUSB_CLASS_VENDOR_SPEC] = "Vendor";
}

SeekThermal::Usb::Interface::Interface(libusb_device* device) :
  device(device),
  descriptor(0),
  handle(0) {
  if (this->device) {
    libusb_ref_device(this->device);

    descriptor = new libusb_device_descriptor();
    SeekThermal::Usb::Error::assert(
      libusb_get_device_descriptor(this->device, descriptor));

    std::ostringstream stream;
    stream << std::setfill('0') << std::setw(3) <<
      (size_t)getDeviceBus() << ":" << std::setfill('0') <<
      std::setw(3) << (size_t)getDeviceAddress();

    address = stream.str();
  }
}

SeekThermal::Usb::Interface::Interface(const Interface& src) :
  SeekThermal::Interface(src),
  descriptor(src.descriptor ?
    new libusb_device_descriptor(*src.descriptor) : 0),
  device(src.device),
  handle(0) {
  if (device)
    libusb_ref_device(device);
}

SeekThermal::Usb::Interface::~Interface() {
  if (handle)
    close();
  if (descriptor)
    delete descriptor;
  if (device)
    libusb_unref_device(device);
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

std::string SeekThermal::Usb::Interface::DeviceClasses::operator[](
    unsigned char code) const {
  const_iterator it = find(code);
  if (it != end())
    return it->second;
  else
    return "Other";
}

unsigned char SeekThermal::Usb::Interface::getDeviceBus() const {
  if (device)
    return libusb_get_bus_number(device);
  else
    return 0;
}

unsigned char SeekThermal::Usb::Interface::getDeviceAddress() const {
  if (device)
    return libusb_get_device_address(device);
  else
    return 0;
}

unsigned short SeekThermal::Usb::Interface::getDeviceVendorId() const {
  if (descriptor)
    return descriptor->idVendor;
  else
    return 0;
}

unsigned short SeekThermal::Usb::Interface::getDeviceProductId() const {
  if (descriptor)
    return descriptor->idProduct;
  else
    return 0;
}

unsigned char SeekThermal::Usb::Interface::getDeviceClassCode() const {
  if (descriptor)
    return descriptor->bDeviceClass;
  else
    return 0;
}

unsigned short SeekThermal::Usb::Interface::getDeviceSpecification() const {
  if (descriptor)
    return descriptor->bcdUSB;
  else
    return 0;
}

std::string SeekThermal::Usb::Interface::getDeviceSerialNumber() const {
  if (handle) {
    unsigned char data[256];
    size_t length;

    length = Error::assert(libusb_get_string_descriptor_ascii(handle,
      descriptor->iSerialNumber, data, sizeof(data)));

    return std::string(reinterpret_cast<char*>(data), length);
  }
  else
    throw OperationError();
}

std::string SeekThermal::Usb::Interface::getName() const {
  return "Universal Serial Bus (USB) Interface";
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

SeekThermal::Usb::Interface& SeekThermal::Usb::Interface::operator=(const
    Interface& src) {
  if (handle)
    close();
  if (device)
    libusb_unref_device(device);

  device = src.device;
  if (src.descriptor) {
    if (!descriptor)
      descriptor = new  libusb_device_descriptor(*src.descriptor);
    else
      *descriptor = *src.descriptor;
  }
  else if (descriptor) {
    delete descriptor;
    descriptor = 0;
  }

  if (device)
    libusb_ref_device(device);

  SeekThermal::Interface::operator=(src);

  return *this;
}

void SeekThermal::Usb::Interface::open() {
  if (!handle) {
    Error::assert(libusb_open(device, &handle));
    if (Error::assert(libusb_kernel_driver_active(handle, 0)))
      Error::assert(libusb_detach_kernel_driver(handle, 0));
  }
}

void SeekThermal::Usb::Interface::close() {
  if (handle) {
    libusb_close(handle);
    handle = 0;
  }
}

void SeekThermal::Usb::Interface::transfer(Request& request) {
  if (handle) {
    Error::assert(libusb_claim_interface(handle, 0));
    request.transfer(handle, request.data, timeout);
    Error::assert(libusb_release_interface(handle, 0));
  }
  else
    throw OperationError();
}

void SeekThermal::Usb::Interface::read(std::vector<unsigned char>& data) {
  if (handle) {
    Error::assert(libusb_claim_interface(handle, 0));

    size_t numRead = 0;
    while (numRead < data.size()) {
      int transferred = 0;
      Error::assert(libusb_bulk_transfer(handle, 0x81, &data[numRead],
        data.size()-numRead, &transferred, timeout*1e3));      
      numRead += transferred;
    }
    
    Error::assert(libusb_release_interface(handle, 0));
  }
  else
    throw OperationError();
}

void SeekThermal::Usb::Interface::transfer(SeekThermal::Request& request) {
  Request* usbRequest = dynamic_cast<Request*>(&request);

  if (usbRequest)
    transfer(*usbRequest);
  else
    throw RequestError();
}

SeekThermal::Pointer<SeekThermal::Device>
    SeekThermal::Usb::Interface::discoverDevice() const {
  const std::map<std::string, Pointer<Device> >&
    prototypes = Singleton<Factory<Device> >::getInstance().getPrototypes();

  for (std::map<std::string, Pointer<Device> >::const_iterator
      it = prototypes.begin(); it != prototypes.end(); ++it)
    if ((getDeviceVendorId() == it->second->getVendorId()) &&
        (getDeviceProductId() == it->second->getProductId()))
      return  it->second->clone();

  return 0;
}

bool SeekThermal::Usb::Interface::isOpen() const {
  return handle;
}
