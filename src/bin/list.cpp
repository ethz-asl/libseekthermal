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
    "List Seek Thermal devices in all available contexts");

  if (application.parseArguments(argc, argv)) {
    std::list<Pointer<Device> > devices = discoverDevices();

    if (!devices.empty()) {
      for (std::list<Pointer<Device> >::const_iterator it = devices.begin();
          it != devices.end(); ++it)
        std::cout << **it << " on " << (*it)->getInterface()->getName() <<
          " " << *(*it)->getInterface() << std::endl;
    }
  }

  return 0;
}
