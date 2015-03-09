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

#include <sstream>

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

template <typename T, typename U, typename V>
    SeekThermal::Request::ParameterRangeError::ParameterRangeError(const T&
    value, const U& minValue, const V& maxValue) {
  std::ostringstream stream;

  stream << "Parameter value out of range: " << value << " [" <<
    minValue << ", " << maxValue << "]";

  description = stream.str();
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

template <typename T, typename U, typename V>
    void SeekThermal::Request::ParameterRangeError::assertValue(const T& value,
    const U& minValue, const V& maxValue) {
  if ((value < minValue) || (value > maxValue))
    throw ParameterRangeError(value, minValue, maxValue);
}
