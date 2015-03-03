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

#ifndef SEEKTHERMAL_USB_ERROR_H
#define SEEKTHERMAL_USB_ERROR_H

/** \brief Seek Thermal USB error
  */

#include <map>

#include <seekthermal/base/exception.h>
#include <seekthermal/base/singleton.h>

namespace SeekThermal {
  namespace Usb {
    class Error :
      public Exception {
    public:
      /** Types and non-static subclasses
        */
      enum Code {
        success,
        io,
        parameter,
        access,
        device,
        entity,
        busy,
        timeout,
        overflow,
        pipe,
        interrupt,
        memory,
        support,
        other,
        unknown
      };

      class Codes :
        public std::map<int, Code> {
      public:
        /** \brief Construct a Seek Thermal USB error codes object
          */
        Codes();

        /** \brief Access the USB error code for the specified error
          */
        Code operator[](int error) const;
        using std::map<int, Code>::operator[];
      };

      class Descriptions :
        public std::map<int, std::string> {
      public:
        /** \brief Construct a Seek Thermal USB error descriptions object
          */
        Descriptions();

        /** \brief Access the USB error description for the specified error
          */
        std::string operator[](int error) const;
        using std::map<int, std::string>::operator[];
      };

      /** \brief Construct a Seek Thermal USB error
        */
      Error(int error);
      Error(const Error& error);

      /** \brief Seek Thermal USB error assignments
        */
      Error& operator=(const Error& src);

      /** \brief Seek Thermal USB error comparisons
        */
      bool operator==(Code code) const;
      bool operator!=(Code code) const;

      /** \brief Seek Thermal USB error assertion
        */
      static int assert(int error);
    protected:
      Code code;
    };
  };
};

#endif
