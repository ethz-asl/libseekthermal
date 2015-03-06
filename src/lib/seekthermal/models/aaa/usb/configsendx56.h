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

#ifndef SEEKTHERMAL_AAA_USB_CONFIG_SEND_X56_H
#define SEEKTHERMAL_AAA_USB_CONFIG_SEND_X56_H

/** \brief Seek XX-AAA Thermal camera config send request "0x56"
  *   (USB protocol)
  * 
  * \note The meaning of this request must be clarified some day.
  */

#include <seekthermal/models/aaa/usb/configsend.h>

namespace SeekThermal {
  namespace AAA {
    namespace Usb {
      class ConfigSendx56 :
        public ConfigSend {
      public:
        /** \brief Construct a Seek XX-AAA Thermal camera config send
          *   request "0x56"
          */
        ConfigSendx56(unsigned char b1 = 0, unsigned char b2 = 0,
          unsigned char b3 = 0, unsigned char b4 = 0, unsigned char b5 = 0,
          unsigned char b6 = 0);

        ConfigSendx56* clone() const;
      };
    };
  };
};

#endif
