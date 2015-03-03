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

#ifndef SEEKTHERMAL_OBJECT_H
#define SEEKTHERMAL_OBJECT_H

/** \brief Seek Thermal object
  */

#include <seekthermal/base/pointer.h>
#include <seekthermal/base/exception.h>

namespace SeekThermal {
  class Object {
  friend class Memory;
  template <class> friend class Pointer;
  public:
    /** \brief Types and non-static subclasses
      */
    class DestructionError :
      public Exception {
    public:
      /** \brief Construct an object destruction error
        */
      DestructionError();
    };

    /** \brief Construct a Seek Thermal object
      */
    Object();

    /** \brief Destroy a Seek Thermal object
      */
    virtual ~Object();

    /** \brief Access the Seek Thermal object's number of references
      */
    size_t getNumReferences() const;

    /** \brief Seek Thermal object queries
      */
    bool inStack() const;
    template <class C> bool isA() const;
  private:
    size_t numReferences;
  };
};

#include <seekthermal/base/object.tpp>

#endif
