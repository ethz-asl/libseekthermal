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

#include "protocol.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::Usb::Protocol::Protocol() {
}

SeekThermal::Usb::Protocol::Protocol(const Protocol& src) {
  operator=(src);
}

SeekThermal::Usb::Protocol::~Protocol() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

std::string SeekThermal::Usb::Protocol::getName() const {
  return "SeekThermal Native USB Protocol";
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

SeekThermal::Usb::Protocol& SeekThermal::Usb::Protocol::operator=(const
    Protocol& src) {
  requests.clear();

  for (std::map<std::string, Pointer<Request> >::const_iterator
      it = src.requests.begin(); it != src.requests.end(); ++it)
    requests.insert(std::make_pair(it->first, it->second->clone()));

  return *this;
}

SeekThermal::Usb::Request* SeekThermal::Usb::Protocol::createRequest(
    const std::string& typeName) const {
  std::map<std::string, Pointer<SeekThermal::Usb::Request> >::const_iterator
    it = requests.find(typeName);

  if (it != requests.end())
    return it->second->clone();
  else
    throw RequestError(typeName);
}

bool SeekThermal::Usb::Protocol::hasRequest(const std::string& typeName)
    const {
  return requests.find(typeName) != requests.end();
}

void SeekThermal::Usb::Protocol::write(std::ostream& stream) const {
  for (std::map<std::string, Pointer<Request> >::const_iterator
      it = requests.begin(); it != requests.end(); ++it) {
    if (it != requests.begin())
       stream << std::endl;
    stream << it->first;
  }
}
