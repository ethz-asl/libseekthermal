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

#include <iostream>
#include <csignal>

#include <seekthermal/seekthermal.h>

#include <seekthermal/command/application.h>

#include <seekthermal/usb/context.h>

using namespace SeekThermal;

bool interrupt = false;

void signaled(int signal) {
  interrupt = true;
}

int main(int argc, char **argv) {
  Command::Application application(
    "Dump frames from a Seek Thermal camera device into files");
  application[0] = Command::Argument(
    "Interface address of connected device", "ADDRESS");
  application[1] = Command::Argument(
    "Filename format string or '-' (stdout)", "FILE", "-");
  application["device"] = Command::Argument(
    "Type of connected device", "TYPE");
  application["timeout"] = Command::Argument(
    "Request timeout in ms", "TIMEOUT", "1000");

  if (application.parseArguments(argc, argv)) {
    SeekThermal::Usb::Context context;
    Pointer<Interface> interface =
      context.getInterface(application[0].getValue());

    Pointer<Device> device;
    if (application["device"].getValue().empty())
      device = interface->discoverDevice();
    else
      device = createDevice(application["device"].getValue());

    if (!device.isNull()) {
      interface->setTimeout(
        application["timeout"].getValue<size_t>()*1e-3);
      device->setInterface(interface);
      device->connect();
      
      device->initialize();
      
      signal(SIGINT, signaled);
      
      size_t frameId = 0;
      while (!interrupt) {
        Frame frame;
        
        device->capture(frame);
        
        if (application[1].getValue() != "-") {
          char filename[1024];
          sprintf(filename, application[1].getValue().c_str(), frameId);
          frame.save(filename);
          
          std::cout << "." << std::flush;
        }
        else
          frame.save(std::cout);
        
        ++frameId;
      }
      if (application[1].getValue() != "-")
        std::cout << std::endl;
      
      device->disconnect();
    }
    else {
      std::cerr << "No such device." << std::endl;
      return -1;
    }
  }

  return 0;
}
