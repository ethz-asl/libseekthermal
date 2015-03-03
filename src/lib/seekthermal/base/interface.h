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

#ifndef SEEKTHERMAL_INTERFACE_H
#define SEEKTHERMAL_INTERFACE_H

/** \brief Abstract Seek Thermal communication interface
  */

#include <vector>

#include <seekthermal/base/object.h>
#include <seekthermal/base/exception.h>
#include <seekthermal/base/pointer.h>
#include <seekthermal/base/request.h>

namespace SeekThermal {
  class Device;

  class Interface :
    public Object {
  public:
    /** \brief Types and non-static subclasses
      */
    class OperationError :
      public Exception {
    public:
      /** \brief Construct an interface operation error
        */
      OperationError();
    };

    class RequestError :
      public Exception {
    public:
      /** \brief Construct an interface request error
        */
      RequestError();
    };

    /** \brief Construct a Seek Thermal interface
      */
    Interface(const Interface& src);

    /** \brief Destroy a Seek Thermal interface
      */
    virtual ~Interface();

    /** \brief Access the name of the Seek Thermal interface
      */
    virtual std::string getName() const = 0;

    /** \brief Access the address of the Seek Thermal interface
      */
    const std::string& getAddress() const;
    /** \brief Access the timeout of the Seek Thermal interface
      */
    double getTimeout() const;
    void setTimeout(double timeout);

    /** \brief Seek Thermal interface assignments
      */
    Interface& operator=(const Interface& src);

    /** \brief Open the Seek Thermal interface
      */
    virtual void open() = 0;
    /** \brief Close the Seek Thermal interface
      */
    virtual void close() = 0;

    /** \brief Transfer the specified request via the interface
      */
    virtual void transfer(Request& request) = 0;

    /** \brief Seek Thermal interface queries
      */
    virtual bool isOpen() const = 0;

    /** \brief Write the interface to the given stream
      */
    void write(std::ostream& stream) const;

    /** \brief Discover the Seek Thermal device connected by the interface
      */
    virtual Pointer<Device> discoverDevice() const = 0;
  protected:
    std::string address;
    double timeout;

    /** \brief Construct a Seek Thermal interface
      */
    Interface(const std::string& address = "", double timeout = 1e-2);
  };
};

std::ostream& operator<<(std::ostream& stream, const SeekThermal::Interface&
  interface);

#include <seekthermal/base/device.h>

#endif
