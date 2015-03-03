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

#ifndef SEEKTHERMAL_CONTEXT_H
#define SEEKTHERMAL_CONTEXT_H

/** \brief Abstract Seek Thermal communication context
  */

#include <list>

#include <seekthermal/base/object.h>
#include <seekthermal/base/exception.h>
#include <seekthermal/base/pointer.h>
#include <seekthermal/base/interface.h>
#include <seekthermal/base/device.h>

namespace SeekThermal {
  class Context :
    public Object {
  public:
    /** \brief Types and non-static subclasses
      */
    class AddressError :
      public Exception {
    public:
      /** \brief Construct a Seek Thermal context address error
        */
      AddressError(const std::string& address);
    };

    /** \brief Construct a Seek Thermal context
      */
    Context();

    /** \brief Destroy a Seek Thermal context
      */
    virtual ~Context();

    /** \brief Access the Seek Thermal interface corresponding to the
      *   specified address within the Seek Thermal context
      */
    virtual Interface* getInterface(const std::string& address) const = 0;

    /** \brief Clone the Seek Thermal context
      */
    virtual Context* clone() const = 0;

    /** \brief Discover all Seek Thermal devices in the context
      */
    virtual std::list<Pointer<Device> > discoverDevices() const = 0;
  };
};

#endif
