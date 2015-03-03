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

#ifndef SEEKTHERMAL_AAA_DEVICE_H
#define SEEKTHERMAL_AAA_DEVICE_H

/** \brief Seek XX-AAA Thermal camera device
  */

#include <list>
#include <map>

#include <seekthermal/base/device.h>
#include <seekthermal/base/prototype.h>

namespace SeekThermal {
  namespace AAA {
    class Device :
      public SeekThermal::Device {
    public:
      /** \brief Types and non-static subclasses
        */
      enum Type {
        typeUW = 0x10,
      };

      class Types :
        public std::map<Type, std::string> {
      public:
        /** \brief Construct a Seek XX-AAA Thermal camera types object
          */
        Types();

        /** \brief Access the Seek XX-AAA Thermal camera type for the
          *   specified product ID
          */
        std::string operator[](Type productId) const;
        using std::map<Type, std::string>::operator[];
      };

      class Prototypes :
        public std::list<Prototype<SeekThermal::Device> > {
      public:
        /** \brief Construct a Seek XX-AAA Thermal camera prototypes object
          */
        Prototypes();
      };

      class Names :
        public std::map<Type, std::string> {
      public:
        /** \brief Construct a Seek XX-AAA Thermal camera names object
          */
        Names();

        /** \brief Access the Seek XX-AAA Thermal camera name for the
          *   specified product ID
          */
        std::string operator[](Type productId) const;
        using std::map<Type, std::string>::operator[];
      };

      class Protocols :
        public SeekThermal::Device::Protocols {
      public:
        /** \brief Construct a Seek XX-AAA Thermal camera protocols object
          */
        Protocols();
      };

      /** \brief Construct a Seek XX-AAA Thermal camera device
        */
      Device(Type productId);
      Device(const Device& src);

      /** \brief Destroy a Seek XX-AAA Thermal camera device
        */
      virtual ~Device();

      std::string getName() const;
      const Protocols& getProtocols() const;

      /** \brief Seek XX-AAA Thermal camera device assignments
        */
      Device& operator=(const Device& src);

      /** \brief Clone the Seek XX-AAA Thermal camera device
        */
      Device* clone() const;
    private:
      static Prototypes prototypes;
    };
  };
};

#endif
