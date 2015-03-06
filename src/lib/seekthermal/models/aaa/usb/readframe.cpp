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

#include "readframe.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::AAA::Usb::ReadFrame::ReadFrame(size_t size) :
  SeekThermal::Usb::Request(typeVendor, recipientInterface, directionOut,
    0x53, 0, 0) {
  data.resize(4);
  setSize(size);
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

void SeekThermal::AAA::Usb::ReadFrame::setSize(size_t size) {
  data[0] = size;
  data[1] = size >> 8;
}

size_t SeekThermal::AAA::Usb::ReadFrame::getSize() const {
  size_t size = data[1];
  size <<= 8;
  size += data[0];
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

SeekThermal::AAA::Usb::ReadFrame*
    SeekThermal::AAA::Usb::ReadFrame::clone() const {
  return new ReadFrame(*this);
}
        
void SeekThermal::AAA::Usb::ReadFrame::read(std::istream& stream) {
  size_t size;
  stream >> size;
  
  setSize(size);
}
