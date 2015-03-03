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

#ifndef SEEKTHERMAL_FACTORY_H
#define SEEKTHERMAL_FACTORY_H

/** \brief Templated Seek Thermal factory
  */

#include <map>

#include <seekthermal/base/object.h>
#include <seekthermal/base/pointer.h>
#include <seekthermal/base/exception.h>

namespace SeekThermal {
  template <class C> class Factory :
    public Object {
  public:
    /** \brief Types and non-static subclasses
      */
    class TypeError :
      public Exception {
    public:
      /** \brief Construct a factory type error
        */
      TypeError(const std::string& typeName);
    };

    /** \brief Construct a Seek Thermal factory
      */
    Factory();

    /** \brief Destroy a Seek Thermal factory
      */
    virtual ~Factory();

    /** \brief Access all registered prototypes
      */
    const std::map<std::string, Pointer<C> >& getPrototypes() const;
    /** \brief Access the prototype of the specified type
      */
    const C& getPrototype(const std::string& typeName) const;

    /** \brief Create an instance of the specified type
      */
    Pointer<C> create(const std::string& typeName) const;

    /**\brief  Register a prototype
      */
    void registerPrototype(const std::string& typeName, const
      Pointer<C>& prototype);

    /** \brief Unregister a prototype
      */
    void unregisterPrototype(const std::string& typeName);

    /** \brief Clear the Seek Thermal factory
      */
    void clear();

    /** \brief Seek Thermal factory queries
      */
    bool isRegistered(const std::string& typeName) const;
  protected:
    std::map<std::string, Pointer<C> > prototypes;
  };
};

#include <seekthermal/base/factory.tpp>

#endif
