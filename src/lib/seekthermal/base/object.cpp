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

#include <seekthermal/base/memory.h>

#include "object.h"

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::Object::DestructionError::DestructionError() :
  Exception("Destructor called for referenced object") {
}

SeekThermal::Object::Object() :
  numReferences(Memory::inStack(this)) {
}

SeekThermal::Object::~Object() {
  if (Memory::inStack(this))
    --numReferences;

  Memory::assertDestruction(this);
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

size_t SeekThermal::Object::getNumReferences() const {
  return numReferences;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

bool SeekThermal::Object::inStack() const {
  return Memory::inStack(this);
}
