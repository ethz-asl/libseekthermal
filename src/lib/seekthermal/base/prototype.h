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

#ifndef SEEKTHERMAL_PROTOTYPE_H
#define SEEKTHERMAL_PROTOTYPE_H

/** \brief Templated Seek Thermal prototype
  */

#include <seekthermal/base/type.h>
#include <seekthermal/base/singleton.h>
#include <seekthermal/base/factory.h>
#include <seekthermal/base/pointer.h>

namespace SeekThermal {
  template <class C> class Prototype {
  public:
    /** \brief Construct a Seek Thermal prototype
      */
    Prototype();
    template <class D> Prototype(D* instance, const std::string&
      typeName = Type<D>::getName(), Factory<C>&
      factory = Singleton<Factory<C> >::getInstance());
    Prototype(const Prototype<C>& src);

    /** \brief Destroy a Seek Thermal prototype
      */
    virtual ~Prototype();

    /** \brief Seek Thermal prototype assignments
      */
    Prototype<C>& operator=(const Prototype<C>& src);
  protected:
    std::string typeName;
  private:
    Pointer<Factory<C> > factory;
    mutable Pointer<C> instance;
  };
};

#include <seekthermal/base/prototype.tpp>

#endif
