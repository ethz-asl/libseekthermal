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

#ifndef SEEKTHERMAL_USB_REQUEST_H
#define SEEKTHERMAL_USB_REQUEST_H

/** \brief Seek Thermal USB request
  */

#include <vector>
#include <map>

#include <seekthermal/base/request.h>

struct libusb_device_handle;

namespace SeekThermal {
  namespace Usb {
    class Request :
      public SeekThermal::Request {
    friend class Interface;
    public:
      /** \brief Types and non-static subclasses
        */
      enum Type {
        typeStandard,
        typeClass,
        typeVendor,
        typeReserved
      };

      enum Recipient {
        recipientDevice,
        recipientInterface,
        recipientEndpoint,
        recipientOther
      };

      enum Direction {
        directionIn,
        directionOut
      };

      class TypeFlags :
        public std::map<Type, unsigned char> {
      public:
        /** \brief Construct a USB request type flags object
          */
        TypeFlags();
      };

      class RecipientFlags :
        public std::map<Recipient, unsigned char> {
      public:
        /** \brief Construct a USB request recipient flags object
          */
        RecipientFlags();
      };

      class DirectionFlags :
        public std::map<Direction, unsigned char> {
      public:
        /** \brief Construct a USB request direction flags object
          */
        DirectionFlags();
      };

      /** \brief Construct a Seek Thermal USB request
        */
      Request(Type type = typeStandard, Recipient recipient = recipientDevice,
        Direction direction = directionOut, unsigned char request = 0,
        unsigned short value = 0, unsigned short index = 0, const
        std::vector<unsigned char>& data = std::vector<unsigned char>());
      Request(Type type, unsigned char request, unsigned short value,
        unsigned short index, size_t inputDataSize);
      Request(Type type, unsigned char request, unsigned short value,
        unsigned short index, const std::vector<unsigned char>&
        outputData = std::vector<unsigned char>());
      Request(const Request& src);

      /** \brief Destroy a Seek Thermal USB request
        */
      virtual ~Request();

      /** \brief Access the Seek Thermal USB request's type
        */
      Type getType() const;
      void setType(Type type);
      /** \brief Access the Seek Thermal USB request's recipient
        */
      Recipient getRecipient() const;
      void setRecipient(Recipient recipient);
      /** \brief Access the Seek Thermal USB request's direction
        */
      Direction getDirection() const;
      void setDirection(Direction direction);

      /** \brief Access the Seek Thermal USB request's request type field
        */
      unsigned char getRequestType() const;
      /** \brief Access the Seek Thermal USB request's request field
        */
      unsigned char getRequest() const;
      void setRequest(unsigned char request);
      /** \brief Access the Seek Thermal USB request's value field
        */
      unsigned short getValue() const;
      void setValue(unsigned short value);
      /** \brief Access the Seek Thermal USB request's index field
        */
      unsigned short getIndex() const;
      void setIndex(unsigned short index);

      /** \brief Access the Seek Thermal USB request's data
        */
      const std::vector<unsigned char>& getData() const;
      void setData(const std::vector<unsigned char>& data);

      /** \brief Seek Thermal USB request assignments
        */
      Request& operator=(const Request& src);

      /** \brief Clone the Seek Thermal USB request
        */
      virtual Request* clone() const = 0;
    protected:
      Type type;
      Recipient recipient;
      Direction direction;

      unsigned char requestType;
      unsigned char request;
      unsigned short value;
      unsigned short index;

      std::vector<unsigned char> data;

      /** \brief Seek Thermal USB request operations
        */
      virtual void transfer(libusb_device_handle* handle,
        std::vector<unsigned char>& data, double timeout);

      static void transfer(libusb_device_handle* handle, unsigned char
        requestType, unsigned char request, unsigned short value,
        unsigned short index,  std::vector<unsigned char>& data,
        double timeout);
    };
  };
};

#endif
