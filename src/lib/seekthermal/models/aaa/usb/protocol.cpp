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

#include <seekthermal/models/aaa/device.h>

#include "protocol.h"

#include "initialize.h"
#include "deinitialize.h"

#include "configsendx3c.h"
#include "configsendx3e.h"
#include "configsendx56.h"

#include "configreceivex3d.h"
#include "configreceivex4e.h"
#include "configreceivex36.h"
#include "configreceivex58.h"

#include "readframe.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::AAA::Usb::Protocol::Protocol() {
  requests["Initialize"] = new Initialize();
  requests["Deinitialize"] = new Deinitialize();
  
  requests["ConfigSendx3C"] = new ConfigSendx3C();
  requests["ConfigSendx3E"] = new ConfigSendx3E();
  requests["ConfigSendx56"] = new ConfigSendx56();
  
  requests["ConfigReceivex3D"] = new ConfigReceivex3D();
  requests["ConfigReceivex4E"] = new ConfigReceivex4E();
  requests["ConfigReceivex36"] = new ConfigReceivex36();
  requests["ConfigReceivex58"] = new ConfigReceivex58();
  
  requests["ReadFrame"] = new ReadFrame();
}

SeekThermal::AAA::Usb::Protocol::Protocol(const Protocol& src) :
  SeekThermal::Usb::Protocol(src) {
}

SeekThermal::AAA::Usb::Protocol::~Protocol() {
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

SeekThermal::AAA::Usb::Protocol& SeekThermal::AAA::Usb::Protocol::operator=(
    const Protocol& src) {
  SeekThermal::Usb::Protocol::operator=(src);
  return *this;
}

SeekThermal::AAA::Usb::Protocol* SeekThermal::AAA::Usb::Protocol::clone()
    const {
  return new Protocol(*this);
}
