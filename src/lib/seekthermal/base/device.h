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

#ifndef SEEKTHERMAL_DEVICE_H
#define SEEKTHERMAL_DEVICE_H

/** \brief Abstract Seek Thermal device
  */

#include <map>
#include <iostream>

#include <seekthermal/base/object.h>
#include <seekthermal/base/exception.h>
#include <seekthermal/base/pointer.h>
#include <seekthermal/base/interface.h>
#include <seekthermal/base/protocol.h>
#include <seekthermal/base/frame.h>

namespace SeekThermal {
  class Device :
    public Object {
  public:
    /** \brief Types and non-static subclasses
      */
    class Protocols :
      public std::map<std::string, Pointer<Protocol> > {
    public:
      /** \brief Construct a Seek Thermal device protocols object
        */
      Protocols();

      /** \brief Access the Seek Thermal device protocol with the specified
        *   type name
        */
      const Protocol& operator[](const std::string& typeName) const;
      using std::map<std::string, Pointer<Protocol> >::operator[];

      /** \brief Write the device protocols to the given stream
        */
      void write(std::ostream& stream) const;
    };

    class InterfaceError :
      public Exception {
    public:
      /** \brief Construct a Seek Thermal device interface error
        */
      InterfaceError();
    };

    class ProtocolError :
      public Exception {
    public:
      /** \brief Construct a Seek Thermal device protocol error
        */
      ProtocolError(const std::string& typeName);
    };

    class ConnectionError :
      public Exception {
    public:
      /** \brief Construct a Seek Thermal device connection error
        */
      ConnectionError();
    };

    /** \brief Construct a Seek Thermal device
      */
    Device(const Device& src);

    /** \brief Destroy a Seek Thermal device
      */
    virtual ~Device();

    /** \brief Access the name of the Seek Thermal device
      */
    virtual std::string getName() const = 0;

    /** \brief Access the vendor ID of the Seek Thermal device
      */
    size_t getVendorId() const;
    /** \brief Access the product ID of the Seek Thermal device
      */
    size_t getProductId() const;

    /** \brief Access the interface of the Seek Thermal device
      */
    void setInterface(const Pointer<Interface>& interface);
    const Pointer<Interface>& getInterface() const;

    /** \brief Access the protocols of the Seek Thermal device
      */
    virtual const Protocols& getProtocols() const = 0;
    /** \brief Access the protocol with the specified type name
      */
    const Protocol& getProtocol(const std::string& typeName) const;

    /** \brief Seek Thermal device assignments
      */
    Device& operator=(const Device& src);

    /** \brief Clone the Seek Thermal device
      */
    virtual Device* clone() const = 0;

    /** \brief Connect the Seek Thermal device
      */
    void connect();
    /** \brief Initialize the Seek Thermal device
      */
    void initialize();
    /** \brief Disconnect the Seek Thermal device
      */
    void disconnect();

    /** \brief Capture frame from the Seek Thermal device
      */
    void capture(Frame& frame);

    /** \brief Send the specified request to the Seek Thermal device
      */
    void send(Request& request);

    /** \brief Seek Thermal device queries
      */
    bool isConnected();

    /** \brief Write the device to the given stream
      */
    void write(std::ostream& stream) const;
  protected:
    size_t vendorId;
    size_t productId;

    Pointer<Interface> interface;

    /** \brief Construct a Seek Thermal device
      */
    Device(size_t vendorId = 0, size_t productId = 0);
    
    /** \brief Perform Seek Thermal device initialization
      */
    virtual void doInitialize() = 0;
    
    /** \brief Perform Seek Thermal device frame capturing
      */
    virtual void doCapture(Frame& frame) = 0;
  };
};

std::ostream& operator<<(std::ostream& stream, const
  SeekThermal::Device& device);
std::ostream& operator<<(std::ostream& stream, const
  SeekThermal::Device::Protocols& protocols);

#endif
