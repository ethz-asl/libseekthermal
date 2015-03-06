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
#include <limits>

#include <seekthermal/base/singleton.h>

#include <seekthermal/models/aaa/usb/protocol.h>

#include <seekthermal/models/aaa/usb/initialize.h>
#include <seekthermal/models/aaa/usb/deinitialize.h>

#include <seekthermal/models/aaa/usb/configsendx3c.h>
#include <seekthermal/models/aaa/usb/configsendx3e.h>
#include <seekthermal/models/aaa/usb/configsendx56.h>

#include <seekthermal/models/aaa/usb/configreceivex3d.h>
#include <seekthermal/models/aaa/usb/configreceivex4e.h>
#include <seekthermal/models/aaa/usb/configreceivex36.h>
#include <seekthermal/models/aaa/usb/configreceivex58.h>

#include <seekthermal/models/aaa/usb/readframe.h>

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
  (*this)["USB"] = new SeekThermal::AAA::Usb::Protocol();
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

void SeekThermal::AAA::Device::doInitialize() {
  Usb::Initialize initializeRequest;
  
  try {
    send(initializeRequest);
  }
  catch (Exception& error) {
    Usb::Deinitialize deinitializeRequest;
    send(deinitializeRequest);
    send(initializeRequest);
  }
  
  Usb::ConfigSendx3C configSendx3CRequest1(0x00, 0x00);
  send(configSendx3CRequest1);
  Usb::ConfigReceivex4E configReceivex4ERequest;
  send(configReceivex4ERequest);
  Usb::ConfigReceivex36 configReceivex36Request;
  send(configReceivex36Request);
  
  Usb::ConfigSendx56 configSendx56Request1(0x20, 0x00, 0x30, 0x00, 0x00, 0x00);
  send(configSendx56Request1);
  Usb::ConfigReceivex58 configReceivex58Request1(64);
  send(configReceivex58Request1);
  
  Usb::ConfigSendx56 configSendx56Request2(0x20, 0x00, 0x50, 0x00, 0x00, 0x00);
  send(configSendx56Request2);
  Usb::ConfigReceivex58 configReceivex58Request2(64);
  send(configReceivex58Request2);
  
  Usb::ConfigSendx56 configSendx56Request3(0x0c, 0x00, 0x70, 0x00, 0x00, 0x00);
  send(configSendx56Request3);
  Usb::ConfigReceivex58 configReceivex58Request3(24);
  send(configReceivex58Request3);
  
  Usb::ConfigSendx56 configSendx56Request4(0x06, 0x00, 0x08, 0x00, 0x00, 0x00);
  send(configSendx56Request4);
  Usb::ConfigReceivex58 configReceivex58Request4(12);
  send(configReceivex58Request4);
  
  Usb::ConfigSendx3E configSendx3ERequest1(0x08, 0x00);
  send(configSendx3ERequest1);
  Usb::ConfigReceivex3D configReceivex3DRequest1;
  send(configReceivex3DRequest1);
  
  Usb::ConfigSendx3E configSendx3ERequest2(0x08, 0x00);
  send(configSendx3ERequest2);
  Usb::ConfigSendx3C configSendx3CRequest2(0x01, 0x00);
  send(configSendx3CRequest2);
  Usb::ConfigReceivex3D configReceivex3DRequest2;
  send(configReceivex3DRequest2);
}

void SeekThermal::AAA::Device::doCapture(Frame& frame) {
  const size_t width = 208;
  const size_t height = 156;
  const size_t size = width*height;
  
  frame.setType(Frame::typeInvalid);
  frame.setTimestamp(Timestamp::now());
  
  Usb::ReadFrame readFrameRequest(size);
  send(readFrameRequest);
  
  std::vector<unsigned char> data(size*sizeof(unsigned short));
  interface->read(data);

  frame.resize(width, height);  
  if (data[20] == 0x01)
    frame.setType(Frame::typeCalibration);
  else if (data[20] == 0x03)
    frame.setType(Frame::typeNormal);
  else if (data[20] == 0x06)
    frame.setType(Frame::typePreCalibration);
  else 
    frame.setType(Frame::typeUnknown);
  
  unsigned short* raw = reinterpret_cast<unsigned short*>(&data[0]);
  
  for (size_t i = 0; i < size; ++i) {
    size_t x = i%width;
    size_t y = i/width;
    
    size_t xs = 14-((y+1)*4%15);
    
    if ((x%15) != xs)  {
      unsigned short value = raw[i];
      size_t dx = 0;
      if (x > xs)
        dx = (x-xs)/15;
      frame(x-dx, y) = value;
    }
  }
  
  frame.crop(0, 0, 2+width/15, 0);
  frame.rotateCounterClockwise();
}
