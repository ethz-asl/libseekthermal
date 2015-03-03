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

#include <seekthermal/base/singleton.h>
#include <seekthermal/base/factory.h>

#include "seekthermal.h"

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

SeekThermal::Pointer<SeekThermal::Context> SeekThermal::createContext(const
    std::string& typeName) {
  return Singleton<Factory<Context> >::getInstance().create(typeName);
}

SeekThermal::Pointer<SeekThermal::Device> SeekThermal::createDevice(const
    std::string& typeName) {
  return Singleton<Factory<Device> >::getInstance().create(typeName);
}

std::list<SeekThermal::Pointer<SeekThermal::Device> >
    SeekThermal::discoverDevices() {
  std::list<Pointer<Device> > devices;

  const std::map<std::string, Pointer<Context> >&
    contexts = Singleton<Factory<Context> >::getInstance().getPrototypes();
  for (std::map<std::string, Pointer<Context> >::const_iterator
      it = contexts.begin(); it != contexts.end(); ++it) {
    std::list<Pointer<Device> > discovered = it->second->discoverDevices();
    devices.insert(devices.end(), discovered.begin(), discovered.end());
  }

  return devices;
}
