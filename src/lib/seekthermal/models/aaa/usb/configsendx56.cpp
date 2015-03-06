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

#include "configsendx56.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::AAA::Usb::ConfigSendx56::ConfigSendx56(unsigned char b1,
    unsigned char b2, unsigned char b3, unsigned char b4, unsigned char b5,
    unsigned char b6) :
  ConfigSend(0x56, 0, 0, 6) {
  data[0] = b1;
  data[1] = b2;
  data[2] = b3;
  data[3] = b4;
  data[4] = b5;
  data[5] = b6;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

SeekThermal::AAA::Usb::ConfigSendx56*
    SeekThermal::AAA::Usb::ConfigSendx56::clone() const {
  return new ConfigSendx56(*this);
}
