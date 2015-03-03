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

#ifndef SEEKTHERMAL_SINGLETON_H
#define SEEKTHERMAL_SINGLETON_H

/** \brief Templated Seek Thermal singleton
  */

#include <seekthermal/base/exception.h>

namespace SeekThermal {
  template <class C> class Singleton {
  public:
    /** \brief Access the singleton instance
      */
    static C& getInstance();

    /** \brief Seek Thermal singleton queries
      */
    static bool exists();
  private:
    static C*& instance();

    /** \brief Construct a Seek Thermal singleton
      */
    Singleton();

    /** \brief Destroy a Seek Thermal singleton
      */
    virtual ~Singleton();
  };
};

#include <seekthermal/base/singleton.tpp>

#endif
