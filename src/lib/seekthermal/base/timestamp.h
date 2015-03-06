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

#ifndef SEEKTHERMAL_TIMESTAMP_H
#define SEEKTHERMAL_TIMESTAMP_H

/** \brief Seek Thermal timestamp
  */

#include <cstdint>

namespace SeekThermal {
  class Timestamp {
  public:
    /** \brief Construct a Seek Thermal object
      */
    Timestamp(int64_t nsSinceTheEpoch = -1);
    Timestamp(const Timestamp& src);
    
    /** \brief Access the timestamp as [ns] since the epoch
      */
    inline int64_t getNsSinceTheEpoch() const {
      return nsSinceTheEpoch;
    };
    
    /** \brief Seek Thermal timestamp assignments
      */
    Timestamp& operator=(const Timestamp& src);
    
    /** \brief Generate a timestamp as of now
      */
    static Timestamp now();
  protected:
    int64_t nsSinceTheEpoch;
  };
};

#endif
