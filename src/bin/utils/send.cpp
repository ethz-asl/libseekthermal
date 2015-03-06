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

#include <seekthermal/seekthermal.h>

#include <seekthermal/command/application.h>

using namespace SeekThermal;

int main(int argc, char **argv) {
  Command::Application application(
    "Send protocol request to Seek Thermal camera device");
  application[0] = Command::Argument(
    "Interface address of connected device", "ADDRESS");
  application[1] = Command::Argument(
    "Identifier of request to be sent", "REQUEST", "Initialize");
  application[2] = Command::Argument(
    "Arguments of request or '-' (stdin)", "ARG", "-");
  application[2].setGreedy(true);
  application["context"] = Command::Argument(
    "Communication context", "CONTEXT", "USB");
  application["device"] = Command::Argument(
    "Type of connected device", "TYPE");
  application["protocol"] = Command::Argument(
    "Communication protocol", "PROTOCOL", "USB");
  application["timeout"] = Command::Argument(
    "Request timeout in ms", "TIMEOUT", "10");
  application["list-protocols"] = Command::Argument(
    "List protocols and exit", "", false);
  application["list-requests"] = Command::Argument(
    "List protocol requests and exit", "", false);

  if (application.parseArguments(argc, argv)) {
    Pointer<Context> context =
      createContext(application["context"].getValue());
    Pointer<Interface> interface =
      context->getInterface(application[0].getValue());

    Pointer<Device> device;
    if (application["device"].getValue().empty())
      device = interface->discoverDevice();
    else
      device = createDevice(application["device"].getValue());

    if (!device.isNull()) {
      if (application["list-protocols"].getValue<bool>()) {
        std::cout << device->getProtocols() << std::endl;
        return 0;
      }

      const Protocol& protocol = device->getProtocol(
        application["protocol"].getValue());

      if (application["list-requests"].getValue<bool>()) {
        std::cout << protocol << std::endl;
        return 0;
      }

      interface->setTimeout(
        application["timeout"].getValue<size_t>()*1e-3);
      device->setInterface(interface);
      device->connect();

      Pointer<Request> request = protocol.createRequest(
        application[1].getValue());
      if (application[2].getValue() != "-") {
        std::istringstream stream(application[2].getValue());
        stream >> *request;
      }
      else
        std::cin >> *request;
      device->send(*request);

      std::ostringstream stream;
      stream << *request;

      if (!stream.str().empty())
        std::cout << "Response: " << stream.str() << std::endl;
      else
        std::cout << "No response." << std::endl;

      device->disconnect();
    }
    else {
      std::cerr << "No such device." << std::endl;
      return -1;
    }
  }

  return 0;
}
