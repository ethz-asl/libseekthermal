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

#include "request.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::Request::Request() {
}

SeekThermal::Request::~Request() {
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void SeekThermal::Request::read(std::istream& stream) {
}

void SeekThermal::Request::write(std::ostream& stream) const {
}

std::istream& operator>>(std::istream& stream, SeekThermal::Request& request) {
  request.read(stream);
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const SeekThermal::Request&
    request) {
  request.write(stream);
  return stream;
}
