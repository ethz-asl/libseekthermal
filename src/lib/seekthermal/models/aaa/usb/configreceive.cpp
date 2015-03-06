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

#include "configreceive.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::AAA::Usb::ConfigReceive::ConfigReceive(unsigned char request,
    unsigned short value,  unsigned short index, size_t inputDataSize) :
  SeekThermal::Usb::Request(typeVendor, recipientInterface, directionIn,
    request, value, index) {
  data.resize(inputDataSize);
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void SeekThermal::AAA::Usb::ConfigReceive::write(std::ostream& stream) const {
  for (size_t i = 0; i < data.size(); ++i) {
    if (i)
      stream << " ";
    
    char hex[16];
    sprintf(hex, "0x%02x", data[i]);
    stream << hex;
  }
}
