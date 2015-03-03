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

#include "device.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::Device::Protocols::Protocols() {
}

SeekThermal::Device::InterfaceError::InterfaceError() :
  Exception("Invalid or missing interface") {
}

SeekThermal::Device::ProtocolError::ProtocolError(const std::string&
    typeName) :
  Exception("Invalid protocol: %s", typeName.c_str()) {
}

SeekThermal::Device::ConnectionError::ConnectionError() :
  Exception("Device not connected") {
}

SeekThermal::Device::Device(size_t vendorId, size_t productId) :
  vendorId(vendorId),
  productId(productId) {
}

SeekThermal::Device::Device(const Device& src) :
  vendorId(src.vendorId),
  productId(src.productId),
  interface(src.interface) {
}

SeekThermal::Device::~Device() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

const SeekThermal::Protocol& SeekThermal::Device::Protocols::operator[](const
    std::string& typeName) const {
  const_iterator it = find(typeName);
  if (it != end())
    return *(it->second);
  else
    throw ProtocolError(typeName);
}

size_t SeekThermal::Device::getVendorId() const {
  return vendorId;
}

size_t SeekThermal::Device::getProductId() const {
  return productId;
}

void SeekThermal::Device::setInterface(const Pointer<Interface>& interface) {
  this->interface = interface;
}

const SeekThermal::Pointer<SeekThermal::Interface>&
    SeekThermal::Device::getInterface() const {
  return interface;
}

const SeekThermal::Protocol& SeekThermal::Device::getProtocol(const
    std::string& typeName) const {
  return getProtocols()[typeName];
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void SeekThermal::Device::Protocols::write(std::ostream& stream) const {
  for (const_iterator it = begin(); it != end(); ++it) {
    if (it != begin())
      stream << std::endl;
    stream << it->first;
  }
}

SeekThermal::Device& SeekThermal::Device::operator=(const
    SeekThermal::Device& src) {
  vendorId = src.vendorId;
  productId = src.productId;

  interface = src.interface;

  return *this;
}

void SeekThermal::Device::connect() {
  if (!interface.isNull())
    interface->open();
  else
    throw InterfaceError();
}

void SeekThermal::Device::disconnect() {
  if (!interface.isNull())
    interface->close();
}

void SeekThermal::Device::send(Request& request) {
  if (isConnected())
    interface->transfer(request);
  else
    throw ConnectionError();
}

bool SeekThermal::Device::isConnected() {
  if (!interface.isNull())
    return interface->isOpen();
  else
    return false;
}

void SeekThermal::Device::write(std::ostream& stream) const {
  stream << getName();
}

std::ostream& operator<<(std::ostream& stream, const SeekThermal::Device&
    device) {
  device.write(stream);
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const
    SeekThermal::Device::Protocols& protocols) {
  protocols.write(stream);
  return stream;
}
