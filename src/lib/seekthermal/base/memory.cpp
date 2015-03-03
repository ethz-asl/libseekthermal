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

#include <seekthermal/base/object.h>

#include "memory.h"

/*****************************************************************************/
/* Statics                                                                   */
/*****************************************************************************/

const void* SeekThermal::Memory::stackBottom = 0;

void __attribute__((constructor)) create() {
  char firstStackVariable;
  SeekThermal::Memory::setStackBottom(&firstStackVariable);
}

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

SeekThermal::Memory::AllocationError::AllocationError() :
  Exception("Memory allocation error") {
}

SeekThermal::Memory::DeallocationError::DeallocationError() :
  Exception("Memory deallocation error") {
}

SeekThermal::Memory::Memory() {
}

SeekThermal::Memory::~Memory() {
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

void SeekThermal::Memory::setStackBottom(const void* stackBottom) {
  SeekThermal::Memory::stackBottom = stackBottom;
}

const void* SeekThermal::Memory::getStackBottom() {
  return stackBottom;
}

size_t SeekThermal::Memory::getFrameSize() {
  char x = 0;
  return computeFrameSize(&x);
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void* SeekThermal::Memory::allocate(size_t size) {
  try {
    void* p = ::operator new(size);
  }
  catch (std::exception& exception) {
    throw AllocationError();
  }
}

void SeekThermal::Memory::deallocate(void* p) {
  try {
    ::operator delete(p);
  }
  catch (std::exception& exception) {
    throw DeallocationError();
  }
}

bool SeekThermal::Memory::inStack(const void* p) {
  if (stackBottom) {
    char x;
    const void* stackTop = &x;

    if (stackTop > stackBottom)
      return (p > stackBottom) && (p < stackTop);
    else
      return (p > stackTop) && (p < stackBottom);
  }
  else
    return false;
}

void SeekThermal::Memory::assertDestruction(const Object* object) {
  if (object->numReferences)
    throw Object::DestructionError();
}

void SeekThermal::Memory::garbageCollect(Object* object) {
  if (!object->numReferences)
    delete object;
}

size_t SeekThermal::Memory::computeFrameSize(const char* x) {
  char y = 0;
  return x-&y;
}
