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

#include <libusb.h>

#include <seekthermal/base/singleton.h>

#include "request.h"
#include "error.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::Usb::Request::TypeFlags::TypeFlags() {
  (*this)[typeStandard] = LIBUSB_REQUEST_TYPE_STANDARD;
  (*this)[typeClass] = LIBUSB_REQUEST_TYPE_CLASS;
  (*this)[typeVendor] = LIBUSB_REQUEST_TYPE_VENDOR;
  (*this)[typeReserved] = LIBUSB_REQUEST_TYPE_RESERVED;
}

SeekThermal::Usb::Request::RecipientFlags::RecipientFlags() {
  (*this)[recipientDevice] = LIBUSB_RECIPIENT_DEVICE;
  (*this)[recipientInterface] = LIBUSB_RECIPIENT_INTERFACE;
  (*this)[recipientEndpoint] = LIBUSB_RECIPIENT_ENDPOINT;
  (*this)[recipientOther] = LIBUSB_RECIPIENT_OTHER;
}

SeekThermal::Usb::Request::DirectionFlags::DirectionFlags() {
  (*this)[directionIn] = LIBUSB_ENDPOINT_IN;
  (*this)[directionOut] = LIBUSB_ENDPOINT_OUT;
}

SeekThermal::Usb::Request::Request(Type type, Recipient recipient, Direction
    direction, unsigned char request, unsigned short value, unsigned short
    index, const std::vector<unsigned char>& data) :
  type(type),
  recipient(recipient),
  direction(direction),
  request(request),
  value(value),
  index(index),
  data(data) {
}

SeekThermal::Usb::Request::Request(Type type, unsigned char request, unsigned
    short value, unsigned short index, size_t inputDataSize) :
  type(type),
  recipient(recipientDevice),
  direction(directionIn),
  request(request),
  value(value),
  index(index),
  data(inputDataSize) {
}

SeekThermal::Usb::Request::Request(Type type, unsigned char request, unsigned
    short value, unsigned short index, const std::vector<unsigned char>&
    outputData) :
  type(type),
  recipient(recipientDevice),
  direction(directionOut),
  request(request),
  value(value),
  index(index),
  data(outputData) {
}

SeekThermal::Usb::Request::Request(const Request& src) :
  type(src.type),
  recipient(src.recipient),
  direction(src.direction),
  request(src.request),
  value(src.value),
  index(src.index),
  data(src.data) {
}

SeekThermal::Usb::Request::~Request() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

SeekThermal::Usb::Request::Type SeekThermal::Usb::Request::getType() const {
  return type;
}

void SeekThermal::Usb::Request::setType(Type type) {
  this->type = type;
}

SeekThermal::Usb::Request::Recipient SeekThermal::Usb::Request::getRecipient()
    const {
  return recipient;
}

void SeekThermal::Usb::Request::setRecipient(Recipient recipient) {
  this->recipient = recipient;
}

SeekThermal::Usb::Request::Direction SeekThermal::Usb::Request::getDirection()
    const {
  return direction;
}

void SeekThermal::Usb::Request::setDirection(Direction direction) {
  this->direction = direction;
}

unsigned char SeekThermal::Usb::Request::getRequestType() const {
  return Singleton<TypeFlags>::getInstance()[type] |
    Singleton<RecipientFlags>::getInstance()[recipient] |
    Singleton<DirectionFlags>::getInstance()[direction];
}

unsigned char SeekThermal::Usb::Request::getRequest() const {
  return request;
}

void SeekThermal::Usb::Request::setRequest(unsigned char request) {
  this->request = request;
}

unsigned short SeekThermal::Usb::Request::getValue() const {
  return value;
}

void SeekThermal::Usb::Request::setValue(unsigned short value) {
  this->value = value;
}

unsigned short SeekThermal::Usb::Request::getIndex() const {
  return index;
}

void SeekThermal::Usb::Request::setIndex(unsigned short index) {
  this->index = index;
}

const std::vector<unsigned char>& SeekThermal::Usb::Request::getData() const {
  return data;
}

void SeekThermal::Usb::Request::setData(const std::vector<unsigned char>&
    data) {
  this->data = data;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

SeekThermal::Usb::Request& SeekThermal::Usb::Request::operator=(const Request&
    src) {
  type = src.type;
  recipient = src.recipient;
  direction = src.direction;

  request = src.request;
  value = src.value;
  index = src.index;

  data = src.data;

  return *this;
}

void SeekThermal::Usb::Request::transfer(libusb_device_handle* handle,
    std::vector<unsigned char>& data, double timeout) {
  transfer(handle, getRequestType(), request, value, index, data, timeout);
}

void SeekThermal::Usb::Request::transfer(libusb_device_handle* handle,
    unsigned char requestType, unsigned char request, unsigned short value,
    unsigned short index,  std::vector<unsigned char>& data, double timeout) {
  unsigned char* requestData = 0;
  unsigned short requestLength = data.size();

  if (requestLength)
    requestData = &data[0];

  Error::assert(libusb_control_transfer(handle, requestType, request,
    value, index, requestData, requestLength, timeout*1e3));
}
