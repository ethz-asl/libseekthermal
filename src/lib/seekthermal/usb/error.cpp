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

#include "error.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::Usb::Error::Codes::Codes() {
  (*this)[LIBUSB_SUCCESS] = success;
  (*this)[LIBUSB_ERROR_IO] = io;
  (*this)[LIBUSB_ERROR_INVALID_PARAM] = parameter;
  (*this)[LIBUSB_ERROR_ACCESS] = access;
  (*this)[LIBUSB_ERROR_NO_DEVICE] = device;
  (*this)[LIBUSB_ERROR_NOT_FOUND] = entity;
  (*this)[LIBUSB_ERROR_BUSY] = busy;
  (*this)[LIBUSB_ERROR_TIMEOUT] = timeout;
  (*this)[LIBUSB_ERROR_OVERFLOW] = overflow;
  (*this)[LIBUSB_ERROR_PIPE] = pipe;
  (*this)[LIBUSB_ERROR_INTERRUPTED] = interrupt;
  (*this)[LIBUSB_ERROR_NO_MEM] = memory;
  (*this)[LIBUSB_ERROR_NOT_SUPPORTED] = support;
  (*this)[LIBUSB_ERROR_OTHER] = other;
}

SeekThermal::Usb::Error::Descriptions::Descriptions() {
  (*this)[LIBUSB_SUCCESS] = "Success.";
  (*this)[LIBUSB_ERROR_IO] = "Input/output error.";
  (*this)[LIBUSB_ERROR_INVALID_PARAM] = "Invalid parameter.";
  (*this)[LIBUSB_ERROR_ACCESS] = "Access denied.";
  (*this)[LIBUSB_ERROR_NO_DEVICE] = "No such device.";
  (*this)[LIBUSB_ERROR_NOT_FOUND] = "Entity not found.";
  (*this)[LIBUSB_ERROR_BUSY] = "Resource busy.";
  (*this)[LIBUSB_ERROR_TIMEOUT] = "Operation timed out.";
  (*this)[LIBUSB_ERROR_OVERFLOW] = "Overflow.";
  (*this)[LIBUSB_ERROR_PIPE] = "Pipe error.";
  (*this)[LIBUSB_ERROR_INTERRUPTED] = "System call interrupted.";
  (*this)[LIBUSB_ERROR_NO_MEM] = "Insufficient memory.";
  (*this)[LIBUSB_ERROR_NOT_SUPPORTED] = "Operation not supported.";
  (*this)[LIBUSB_ERROR_OTHER] = "Other error.";
}

SeekThermal::Usb::Error::Error(int error) :
  Exception("USB error: %s",
    Singleton<Descriptions>::getInstance()[error].c_str()),
  code(Singleton<Codes>::getInstance()[error]) {
}

SeekThermal::Usb::Error::Error(const Error& src) :
  Exception(src),
  code(src.code) {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

SeekThermal::Usb::Error::Code SeekThermal::Usb::Error::Codes::operator[](
    int error) const {
  const_iterator it = find(error);
  if (it != end())
    return it->second;
  else
    return unknown;
}

std::string SeekThermal::Usb::Error::Descriptions::operator[](int error)
    const {
  const_iterator it = find(error);
  if (it != end())
    return it->second;
  else
    return "Unkown error.";
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

SeekThermal::Usb::Error& SeekThermal::Usb::Error::operator=(const Error& src) {
  Exception::operator=(src);
  code = src.code;

  return *this;
}

bool SeekThermal::Usb::Error::operator==(Code code) const {
  return (code == this->code);
}

bool SeekThermal::Usb::Error::operator!=(Code code) const {
  return (code != this->code);
}

int SeekThermal::Usb::Error::assertSuccess(int error) {
  if (error < LIBUSB_SUCCESS)
    throw Error(error);
  else
    return error;
}
