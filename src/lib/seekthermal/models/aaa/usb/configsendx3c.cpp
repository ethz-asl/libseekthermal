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

#include "configsendx3c.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::AAA::Usb::ConfigSendx3C::ConfigSendx3C(unsigned char b1,
    unsigned char b2) :
  ConfigSend(0x3c, 0, 0, 2) {
  data[0] = b1;
  data[1] = b2;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

SeekThermal::AAA::Usb::ConfigSendx3C*
    SeekThermal::AAA::Usb::ConfigSendx3C::clone() const {
  return new ConfigSendx3C(*this);
}
