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

#include <typeinfo>
#include <cxxabi.h>

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

template <class C>
SeekThermal::Type<C>::Type() {
}

template <class C>
SeekThermal::Type<C>::~Type() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

template <class C>
const std::string& SeekThermal::Type<C>::getName() {
  static std::string name;

  if (name.empty()) {
    std::string mangled = typeid(C).name();

    size_t length;
    int status;
    name = abi::__cxa_demangle(mangled.c_str(), 0, &length, &status);

    if (status)
      name = mangled;
  }

  return name;
}
