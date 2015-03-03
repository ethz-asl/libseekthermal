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

#ifndef SEEKTHERMAL_MEMORY_H
#define SEEKTHERMAL_MEMORY_H

/** \brief Seek Thermal memory management
  */

#include "exception.h"

namespace SeekThermal {
  class Object;

  class Memory {
  public:
    /** \brief Types and non-static subclasses
      */
    class AllocationError :
      public Exception {
    public:
      /** \brief Construct a memory allocation error
        */
      AllocationError();
    };

    class DeallocationError :
      public Exception {
    public:
      /** \brief Construct a memory deallocation error
        */
      DeallocationError();
    };

    /** \brief Access the stack bottom
      */
    static void setStackBottom(const void* stackBottom);
    static const void* getStackBottom();
    /** \brief Access the memory frame size
      */
    static size_t getFrameSize();

    /** \brief Allocate memory on the heap
      */
    static void* allocate(size_t size);
    /** \brief Deallocate memory on the heap
      */
    static void deallocate(void* p);

    /** \brief Check if some pointer points into the stack
      */
    static bool inStack(const void* p);

    /** \brief Assert the destruction of an object
      */
    static void assertDestruction(const Object* object);
    /** \brief Garbage-collect an object
      */
    static void garbageCollect(Object* object);
  protected:
    static const void* stackBottom;

    /** \brief Construct a Seek Thermal memory management object
      */
    Memory();
    /** \brief Destroy a Seek Thermal memory management object
      */
    virtual ~Memory();

    /** \brief Compute the memory frame size
      */
    static size_t computeFrameSize(const char* x);
  };
};

#endif
