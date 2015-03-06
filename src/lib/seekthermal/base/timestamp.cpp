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

#include <boost/chrono.hpp>

#include "timestamp.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::Timestamp::Timestamp(int64_t nsSinceTheEpoch) :
  nsSinceTheEpoch(nsSinceTheEpoch) {
}

SeekThermal::Timestamp::Timestamp(const Timestamp& src) :
  nsSinceTheEpoch(src.nsSinceTheEpoch) {
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

SeekThermal::Timestamp& SeekThermal::Timestamp::operator=(const Timestamp&
    src) {
  nsSinceTheEpoch = src.nsSinceTheEpoch;
  return *this;
}

SeekThermal::Timestamp SeekThermal::Timestamp::now() {
  boost::chrono::high_resolution_clock::time_point now =
    boost::chrono::high_resolution_clock::now();
  return boost::chrono::duration_cast<boost::chrono::nanoseconds>(
    now.time_since_epoch()).count();
}
