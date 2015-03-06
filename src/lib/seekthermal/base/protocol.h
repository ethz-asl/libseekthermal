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

#ifndef SEEKTHERMAL_PROTOCOL_H
#define SEEKTHERMAL_PROTOCOL_H

/** \brief Abstract Seek Thermal communication protocol
  */

#include <seekthermal/base/object.h>
#include <seekthermal/base/exception.h>
#include <seekthermal/base/pointer.h>

namespace SeekThermal {
  class Request;
  class Device;

  class Protocol :
    public Object {
  public:
    /** \brief Types and non-static subclasses
      */
    class RequestError :
      public Exception {
    public:
      /** \brief Construct a protocol request error
        */
      RequestError(const std::string& typeName);
    };

    /** \brief Construct a Seek Thermal protocol
      */
    Protocol();

    /** \brief Destroy a Seek Thermal protocol
      */
    virtual ~Protocol();

    /** \brief Access the name of the Seek Thermal protocol
      */
    virtual std::string getName() const = 0;

    /** \brief Clone the Seek Thermal protocol
      */
    virtual Protocol* clone() const = 0;

    /** \brief Create a Seek Thermal request with the specified type name
      */
    virtual Request* createRequest(const std::string& typeName)
      const = 0;

    /** \brief Seek Thermal protocol queries
      */
    virtual bool hasRequest(const std::string& typeName) const = 0;

    /** \brief Write the protocol to the given stream
      */
    virtual void write(std::ostream& stream) const = 0;
  };
};

std::ostream& operator<<(std::ostream& stream, const
  SeekThermal::Protocol& protocol);

#include <seekthermal/base/request.h>

#endif
