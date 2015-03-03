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

#ifndef SEEKTHERMAL_SERIALIZABLE_H
#define SEEKTHERMAL_SERIALIZABLE_H

/** \brief Seek Thermal serializable
  */

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <seekthermal/base/object.h>
#include <seekthermal/base/exception.h>

namespace SeekThermal {
  template <typename T> class Serializable {
  public:
    /** \brief Types and non-static subclasses
      */
    class SerializationError :
      public Exception {
    public:
      /** \brief Construct a serializable serialization error
        */
      SerializationError();
    };

    class DeserializationError :
      public Exception {
    public:
      /** \brief Construct a serializable deserialization error
        */
      DeserializationError();
    };

    /** \brief Traits
      */
    template <typename U, size_t D = 0> struct Traits {
      static void read(std::istream& stream, U& value);
      static void write(std::ostream& stream, const U& value);
    };

    template <size_t D> struct Traits<std::string, D> {
      static void read(std::istream& stream, std::string& value);
      static void write(std::ostream& stream, const std::string& value);
    };

    template <size_t D> struct Traits<unsigned char, D> {
      static void read(std::istream& stream, unsigned char& value);
      static void write(std::ostream& stream, const unsigned char& value);
    };

    template <size_t D> struct Traits<std::vector<unsigned char>, D> {
      static void read(std::istream& stream,
        std::vector<unsigned char>& value);
      static void write(std::ostream& stream, const
        std::vector<unsigned char>& value);
    };

    template <typename K, typename V, size_t D>
        struct Traits<std::map<K, V>, D> {
      static void read(std::istream& stream, std::map<K, V>& value);
      static void write(std::ostream& stream, const std::map<K, V>& value);
    };

    /** \brief Construct a Seek Thermal serializable
      */
    Serializable(const T& value = T());
    Serializable(const Serializable<T>& src);

    /** \brief Destroy a Seek Thermal serializable
      */
    virtual ~Serializable();

    /** \brief Seek Thermal serializable assignments
      */
    Serializable<T>& operator=(const Serializable<T>& src);
    Serializable<T>& operator=(const T& value);

    /** \brief Read the serializable from the given stream
      */
    T& read(std::istream& stream);
    template <typename U> T& read(std::istream& stream, const
      std::map<U, std::string>& values);
    /** \brief Write the serializable to the given stream
      */
    void write(std::ostream& stream) const;
    template <typename U> void write(std::ostream& stream,
      const std::map<U, std::string>& values) const;

    /** \brief Seek Thermal serializable conversions
      */
    operator T&();
    operator const T&() const;
  protected:
    T value;
  };
};

template <typename T> std::istream& operator>>(std::istream& stream,
  SeekThermal::Serializable<T>& serializable);
template <typename T> std::ostream& operator<<(std::ostream& stream,
  const SeekThermal::Serializable<T>& serializable);

#include <seekthermal/base/serializable.tpp>

#endif
