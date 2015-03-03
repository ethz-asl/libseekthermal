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

#ifndef SEEKTHERMAL_USB_PROTOCOL_H
#define SEEKTHERMAL_USB_PROTOCOL_H

/** \brief Seek Thermal USB protocol
  */

#include <map>

#include <seekthermal/base/protocol.h>
#include <seekthermal/base/pointer.h>

#include <seekthermal/usb/request.h>

namespace SeekThermal {
  namespace Usb {
    class Protocol :
      public SeekThermal::Protocol {
    public:
      /** \brief Construct a Seek Thermal USB protocol
        */
      Protocol();
      Protocol(const Protocol& src);

      /** \brief Destroy a Seek Thermal USB protocol
        */
      virtual ~Protocol();

      std::string getName() const;

      /** \brief Seek Thermal USB protocol assignments
        */
      Protocol& operator=(const Protocol& src);

      /** \brief Create a Seek Thermal USB request with the specified type
        *   name
        */
      Request* createRequest(const std::string& typeName) const;

      /** \brief Seek Thermal USB protocol queries
        */
      bool hasRequest(const std::string& typeName) const;

      /** \brief Write the USB protocol to the given stream
        */
      void write(std::ostream& stream) const;
    protected:
      std::map<std::string, Pointer<Request> > requests;
    };
  };
};

#endif
