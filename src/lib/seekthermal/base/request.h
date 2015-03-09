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

#ifndef SEEKTHERMAL_REQUEST_H
#define SEEKTHERMAL_REQUEST_H

/** \brief Abstract Seek Thermal communication request
  */

#include <iostream>

#include <seekthermal/base/object.h>
#include <seekthermal/base/exception.h>

namespace SeekThermal {
  class Request :
    public Object {
  public:
    /** \brief Types and non-static subclasses
      */
    class ParameterRangeError :
      public Exception {
    public:
      /** \brief Construct a Seek Thermal request parameter range error
        */
      template <typename T, typename U, typename V> ParameterRangeError(
        const T& value, const U& minValue, const V& maxValue);

      /** \brief Seek Thermal request parameter range error assertion
        */
      template <typename T, typename U, typename V> static void assertValue(
        const T& value, const U& minValue, const V& maxValue);
    };

    /** \brief Construct a Seek Thermal request
      */
    Request();

    /** \brief Destroy a Seek Thermal request
      */
    virtual ~Request();

    /** \brief Clone the Seek Thermal request
      */
    virtual Request* clone() const = 0;

    /** \brief Read the request from the given stream
      */
    virtual void read(std::istream& stream);
    /** \brief Write the request to the given stream
      */
    virtual void write(std::ostream& stream) const;
  };
};

std::istream& operator>>(std::istream& stream, SeekThermal::Request& request);
std::ostream& operator<<(std::ostream& stream, const SeekThermal::Request&
  request);

#include <seekthermal/base/request.tpp>

#endif
