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

#include <sstream>
#include <cmath>

#include <seekthermal/base/singleton.h>

#include <seekthermal/models/aaa/usb/protocol.h>

#include "device.h"

/*****************************************************************************/
/* Statics                                                                   */
/*****************************************************************************/

SeekThermal::AAA::Device::Prototypes SeekThermal::AAA::Device::prototypes;

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::AAA::Device::Types::Types() {
  (*this)[typeUW] = "UW-AAA";
}

SeekThermal::AAA::Device::Prototypes::Prototypes() {
  for (std::map<Type, std::string>::const_iterator
      it = Singleton<Types>::getInstance().begin();
      it != Singleton<Types>::getInstance().end(); ++it)
    push_back(Prototype<SeekThermal::Device>(new Device(it->first),
      it->second));
}

SeekThermal::AAA::Device::Names::Names() {
  (*this)[typeUW] = "Seek UW-AAA Thermal Imaging Camera";
}

SeekThermal::AAA::Device::Protocols::Protocols() {
  (*this)["Usb"] = new SeekThermal::AAA::Usb::Protocol();
}

SeekThermal::AAA::Device::Device(Type productId) :
  SeekThermal::Device(0x289d, productId) {
}

SeekThermal::AAA::Device::Device(const Device& src) :
  SeekThermal::Device(src) {
}

SeekThermal::AAA::Device::~Device() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

std::string SeekThermal::AAA::Device::Types::operator[](Type productId) const {
  const_iterator it = find(productId);
  if (it != end())
    return it->second;
  else
    return "Unknown";
}

std::string SeekThermal::AAA::Device::Names::operator[](Type productId) const {
  const_iterator it = find(productId);
  if (it != end())
    return it->second;
  else
    return "Seek XX-AAA Thermal Imaging Camera (Unknown)";
}

std::string SeekThermal::AAA::Device::getName() const {
  return Singleton<Names>::getInstance()[(Type)productId];
}

const SeekThermal::AAA::Device::Protocols&
    SeekThermal::AAA::Device::getProtocols() const {
  return Singleton<Protocols>::getInstance();
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

SeekThermal::AAA::Device& SeekThermal::AAA::Device::operator=(const Device&
    src) {
  SeekThermal::Device::operator=(src);
  return *this;
}

SeekThermal::AAA::Device* SeekThermal::AAA::Device::clone() const {
  return new Device(*this);
}
